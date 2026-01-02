#include "server/server_context.hpp"
#include <cppkit/log/log.hpp>

namespace blogserver::server
{
    void ServerContext::initializeDBPool(const cfg::DatabaseCfg& dbCfg)
    {
        if (dbCfg.type != "postgresql")
        {
            CK_LOG_FATAL("unsupported database type: {}", dbCfg.type);
            throw std::runtime_error("unsupported database type: " + dbCfg.type);
        }

        const std::string dbUri = std::format("dbname={} user={} password={} host={} port={}",
                                              dbCfg.dbname,
                                              dbCfg.username,
                                              dbCfg.password,
                                              dbCfg.host,
                                              dbCfg.port);
        try
        {
            // 创建连接池
            const int pool_size = dbCfg.poolSize > 0 ? dbCfg.poolSize : 10;
            this->dbConnPool_ = tao::pq::connection_pool::create(dbUri);
            // 设置10秒超时
            this->dbConnPool_->set_timeout(std::chrono::seconds(10));
            CK_LOG_INFO("database connection pool created with size {}", pool_size);
        }
        catch (const std::exception& ex)
        {
            CK_LOG_FATAL("failed to create database connection pool: {}", ex.what());
            throw;
        }
    }

    void ServerContext::initializeRedisClient(const cfg::RedisCfg& redisCfg)
    {
        try
        {
            // 1. 配置连接选项
            sw::redis::ConnectionOptions connection_options;
            connection_options.host = redisCfg.host;
            connection_options.port = redisCfg.port;
            connection_options.db = redisCfg.db;
            if (!redisCfg.password.empty())
            {
                connection_options.password = redisCfg.password;
            }

            // 自动重连
            connection_options.keep_alive = true;

            // 2. 配置连接池选项 (这里就是你要的 pool_size)
            sw::redis::ConnectionPoolOptions pool_options;
            pool_options.size = redisCfg.poolSize; // 最大连接数
            pool_options.wait_timeout = std::chrono::milliseconds(100);
            this->redisClient_ = std::make_shared<sw::redis::Redis>(connection_options, pool_options);
            if (!redisCfg.password.empty())
            {
                this->redisClient_->auth(redisCfg.password);
            }

            if (this->redisClient_->ping() != "PONG")
            {
                throw std::runtime_error("Redis PING failed: " + redisCfg.host + ":" + std::to_string(redisCfg.port));
            }
            CK_LOG_INFO("connected to Redis at {}:{}", redisCfg.host, redisCfg.port);
        }
        catch (const std::exception& ex)
        {
            CK_LOG_FATAL("failed to connect to Redis: {}", ex.what());
            throw;
        }
    }
} // namespace blogserver::server
