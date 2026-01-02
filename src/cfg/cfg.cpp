#include "cfg/cfg.hpp"
#include <yaml-cpp/yaml.h>

namespace blogserver::cfg
{
    void Cfg::loadFromFile(const std::string& filePath)
    {
        try
        {
            YAML::Node config = YAML::LoadFile(filePath);

            // 解析 ServerCfg
            if (config["server"])
            {
                const auto& serverNode = config["server"];
                serverCfg_.host = serverNode["host"].as<std::string>();
                serverCfg_.port = serverNode["port"].as<int>();
            }

            // 解析 RedisCfg
            if (config["redis"])
            {
                const auto& redisNode = config["redis"];
                redisCfg_.host = redisNode["host"].as<std::string>();
                redisCfg_.port = redisNode["port"].as<int>();
                redisCfg_.password = redisNode["password"].as<std::string>();
                redisCfg_.db = redisNode["db"].as<int>();
                redisCfg_.poolSize = redisNode["pool_size"].as<int>();
            }

            // 解析 DatabaseCfg
            if (config["database"])
            {
                const auto& databaseNode = config["database"];
                databaseCfg_.type = databaseNode["type"].as<std::string>();
                databaseCfg_.host = databaseNode["host"].as<std::string>();
                databaseCfg_.port = databaseNode["port"].as<int>();
                databaseCfg_.username = databaseNode["username"].as<std::string>();
                databaseCfg_.password = databaseNode["password"].as<std::string>();
                databaseCfg_.dbname = databaseNode["dbname"].as<std::string>();
                databaseCfg_.poolSize = databaseNode["pool_size"].as<int>();
            }

            // 解析 LogCfg
            if (config["log"])
            {
                const auto& logNode = config["log"];
                logCfg_.level = logNode["level"].as<std::string>();
                logCfg_.fileNamePattern = logNode["file_name_pattern"].as<std::string>();
                logCfg_.maxSize = logNode["max_size"].as<std::string>();
                logCfg_.maxFiles = logNode["max_files"].as<int>();
            }
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error(std::string("加载配置文件失败: ") + e.what());
        }
    }
} // namespace blogserver::cfg
