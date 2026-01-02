#pragma once

#include <tao/pq.hpp>
#include <sw/redis++/redis++.h>
#include <memory>
#include "cfg/cfg.hpp"

namespace blogserver::server
{
    class Application;

    class ServerContext
    {
        friend Application;

    public:
        std::shared_ptr<tao::pq::connection_pool> dbConnPool_;

        std::shared_ptr<sw::redis::Redis> redisClient_;

    private:
        void initializeDBPool(const cfg::DatabaseCfg& dbCfg);

        void initializeRedisClient(const cfg::RedisCfg& redisCfg);
    };
} // namespace blogserver::server
