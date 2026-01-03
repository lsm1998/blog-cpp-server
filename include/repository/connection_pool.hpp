#pragma once

#include <tao/pq.hpp>
#include <semaphore>
#include <algorithm>
#include "cppkit/log/log.hpp"

namespace blogserver::repository
{
    constexpr std::ptrdiff_t MAX_POOL_CAPACITY = 1000;

    class ConnectionPool
    {
    public:
        explicit ConnectionPool(const std::string& uri, const int pool_size = 10,
                                const int socketTimeoutSec = 30,
                                const int waitTimeoutSec = 3) :
            semaphore_(std::clamp(pool_size, 1, static_cast<int>(MAX_POOL_CAPACITY))),
            waitTimeoutSec(waitTimeoutSec)
        {
            this->pool_ = tao::pq::connection_pool::create(uri);
            // 设置超时
            this->pool_->set_timeout(std::chrono::seconds(socketTimeoutSec));
        }

        ConnectionPool(const ConnectionPool&) = delete;

        ConnectionPool& operator=(const ConnectionPool&) = delete;

        struct SemaphoreGuard
        {
            std::counting_semaphore<MAX_POOL_CAPACITY>& sem;
            ~SemaphoreGuard() { sem.release(); }
        };

        void acquirePermit(const int timeout_seconds = 3)
        {
            if (!semaphore_.try_acquire_for(std::chrono::seconds(timeout_seconds)))
            {
                CK_LOG_ERROR("Database connection pool exhausted (timeout)");
                throw std::runtime_error("Database connection pool exhausted (timeout)");
            }
        }

        template <typename... As>
        auto execute(std::string_view sql, As&&... as)
        {
            acquirePermit(waitTimeoutSec);
            SemaphoreGuard guard{semaphore_};
            return pool_->execute(sql.data(), std::forward<As>(as)...);
        }

        template <typename F>
        void transaction(F&& function)
        {
            acquirePermit(waitTimeoutSec);
            SemaphoreGuard guard{semaphore_};

            // 开启事务
            const auto conn = pool_->connection();
            auto tx = conn->transaction();
            std::forward<F>(function)(tx);

            // 提交事务
            tx->commit();
        }

        // 动态参数执行辅助函数
        auto executeDynamic(const std::string_view sql, const std::vector<std::string>& args)
        {
            switch (args.size())
            {
            case 0: return this->execute(sql);
            case 1: return this->execute(sql, args[0].c_str());
            case 2: return this->execute(sql, args[0].c_str(), args[1].c_str());
            case 3: return this->execute(sql, args[0].c_str(), args[1].c_str(), args[2].c_str());
            case 4: return this->execute(sql, args[0].c_str(), args[1].c_str(), args[2].c_str(), args[3].c_str());
            case 5: return this->execute(sql, args[0].c_str(), args[1].c_str(), args[2].c_str(), args[3].c_str(),
                                         args[4].c_str());
            case 6: return this->execute(sql, args[0].c_str(), args[1].c_str(), args[2].c_str(), args[3].c_str(),
                                         args[4].c_str(), args[5].c_str());
            default: throw std::runtime_error("Too many dynamic parameters for helper.");
            }
        }

    private:
        // 数据库连接池
        std::shared_ptr<tao::pq::connection_pool> pool_;

        // 信号量用于限制并发连接数
        std::counting_semaphore<MAX_POOL_CAPACITY> semaphore_;

        // 超时设置
        int waitTimeoutSec{};
    };
}
