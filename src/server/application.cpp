#include "server/application.hpp"
#include "cppkit/arg_parser.hpp"
#include "cppkit/log/log.hpp"
#include "cfg/cfg.hpp"
#include "cppkit/http/server/http_server.hpp"
#include "api/api.hpp"
#include <sw/redis++/redis++.h>

namespace blogserver::server
{
    void initLog(const cfg::LogCfg& logCfg)
    {
        auto& logger = cppkit::log::Logger::instance();
        if (logCfg.level == "trace")
            logger.setLevel(cppkit::log::Level::Trace);
        else if (logCfg.level == "debug")
            logger.setLevel(cppkit::log::Level::Debug);
        else if (logCfg.level == "info")
            logger.setLevel(cppkit::log::Level::Info);
        else if (logCfg.level == "warn")
            logger.setLevel(cppkit::log::Level::Warn);
        else if (logCfg.level == "error")
            logger.setLevel(cppkit::log::Level::Error);
        else if (logCfg.level == "fatal")
            logger.setLevel(cppkit::log::Level::Fatal);

        // 解析 maxSize 字符串，转换为字节数
        std::uintmax_t maxSize{};
        try
        {
            if (const std::string& sizeStr = logCfg.maxSize; sizeStr.back() == 'K' || sizeStr.back() == 'k')
                maxSize = std::stoull(sizeStr.substr(0, sizeStr.size() - 1)) * 1024;
            else if (sizeStr.back() == 'M' || sizeStr.back() == 'm')
                maxSize = std::stoull(sizeStr.substr(0, sizeStr.size() - 1)) * 1024 * 1024;
            else if (sizeStr.back() == 'G' || sizeStr.back() == 'g')
                maxSize = std::stoull(sizeStr.substr(0, sizeStr.size() - 1)) * 1024 * 1024 * 1024;
            else
                maxSize = std::stoull(sizeStr);
        }
        catch (...)
        {
            // 默认 10 MB
            maxSize = 10 * 1024 * 1024;
        }
        logger.setRotationSize(maxSize);
        logger.setRotation(cppkit::log::Rotation::Daily);
        logger.setMaxFiles(logCfg.maxFiles);
        logger.setFileNamePattern(logCfg.fileNamePattern);
        CK_LOG_INFO("logger initialized completed");
        logger.flush();
    }

    void Application::init()
    {
        // 解析命令行参数
        cppkit::ArgParser parser;
        parser.addOption("c", "配置文件路径", "cfg/app.yaml");
        parser.parse(argc_, argv_);

        // 加载配置文件
        auto& cfg = cfg::Cfg::getInstance();
        cfg.loadFromFile(parser.get<std::string>("c"));

        // 创建日志系统
        initLog(cfg.getLogCfg());

        // 创建数据库连接
        context_.initializeDBPool(cfg.getDatabaseCfg());

        // 创建 Redis 连接
        context_.initializeRedisClient(cfg.getRedisCfg());
    }

    void Application::run()
    {
        CK_LOG_INFO("application started");
        const auto& cfg = cfg::Cfg::getInstance();

        cppkit::http::server::HttpServer server(cfg.getServerCfg().host, cfg.getServerCfg().port);

        // 注册路由和中间件
        api::registerApiRoutes(*this, server);

        server.start();
    }
} // namespace blogserver::server
