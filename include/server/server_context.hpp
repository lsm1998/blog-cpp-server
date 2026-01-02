#pragma once

#include <tao/pq.hpp>
#include <sw/redis++/redis++.h>
#include <memory>
#include "cfg/cfg.hpp"
#include "repository/connection_pool.hpp"

namespace blogserver::server
{
    class Application;

    class ServerContext
    {
        friend Application;

    public:
        std::shared_ptr<repository::ConnectionPool> dbConnPool_;

        std::shared_ptr<sw::redis::Redis> redisClient_;

    private:
        void initializeDBPool(const cfg::DatabaseCfg& dbCfg);

        void initializeRedisClient(const cfg::RedisCfg& redisCfg);
    };
} // namespace blogserver::server
