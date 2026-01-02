#pragma once

#include <string>

namespace blogserver::cfg
{
    class ServerCfg
    {
    public:
        std::string host;
        int port;
    };

    class RedisCfg
    {
    public:
        std::string host;
        int port;
        std::string password;
        int db;
        int poolSize;
    };

    class DatabaseCfg
    {
    public:
        std::string type;
        std::string host;
        int port;
        std::string username;
        std::string password;
        std::string dbname;
        int poolSize;
    };

    class LogCfg
    {
    public:
        std::string level;
        std::string fileNamePattern;
        std::string maxSize;
        int maxFiles;
    };

    class Cfg
    {
    public:
        ~Cfg() = default;

        [[nodiscard]] const ServerCfg& getServerCfg() const { return serverCfg_; }
        [[nodiscard]] const RedisCfg& getRedisCfg() const { return redisCfg_; }
        [[nodiscard]] const DatabaseCfg& getDatabaseCfg() const { return databaseCfg_; }
        [[nodiscard]] const LogCfg& getLogCfg() const { return logCfg_; }

        static Cfg& getInstance()
        {
            static Cfg g_cfg;
            return g_cfg;
        }

        void loadFromFile(const std::string& filePath);

    private:
        Cfg() = default;

        ServerCfg serverCfg_{};
        RedisCfg redisCfg_{};
        DatabaseCfg databaseCfg_{};
        LogCfg logCfg_{};
    };
} // namespace blogserver::cfg
