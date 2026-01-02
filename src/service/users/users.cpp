#include <cppkit/json/json_parser.hpp>
#include "service/users/users.hpp"

#include "cppkit/log/log.hpp"

namespace blogserver::service::users
{
    constexpr auto queryUserProfileSql =
        "SELECT id,username,nickname,email,headshot,description,position,area,github,telegram,linkedin,twitter FROM users WHERE username = $1";

    std::optional<model::User> UserService::getUserProfile(std::string_view username) const
    {
        try
        {
            // 从redis缓存中获取用户资料
            const auto redisKey = "user:profile:" + std::string(username);
            const auto cachedData = ctx.redisClient_->get(redisKey);
            if (cachedData.has_value())
            {
                auto user = cppkit::json::fromJson<model::User>(cachedData.value());
                return user;
            }
            // 从数据库中获取用户资料
            const auto result = this->ctx.dbConnPool_->execute(queryUserProfileSql, username);
            if (result.empty())
            {
                return std::nullopt;
            }
            const auto& row = result[0];
            model::User user;
            user.id = row["id"].as<int64_t>();
            user.username = row["username"].as<std::string>();
            user.nickname = row["nickname"].as<std::string>();
            user.headshot = row["headshot"].as<std::string>();
            user.email = row["email"].as<std::string>();
            user.description = row["description"].as<std::string>();
            user.position = row["position"].as<std::string>();
            user.area = row["area"].as<std::string>();
            user.github = row["github"].as<std::string>();
            user.telegram = row["telegram"].as<std::string>();
            user.linkedin = row["linkedin"].as<std::string>();
            user.twitter = row["twitter"].as<std::string>();
            // 将用户资料缓存到redis
            const auto userJson = cppkit::json::stringify(user);
            ctx.redisClient_->set(redisKey, userJson);
            ctx.redisClient_->expire(redisKey, std::chrono::minutes(10));
            return user;
        }
        catch (const std::exception& e)
        {
            CK_LOG_ERROR("Failed to get user profile for '{}': {}", username, e.what());
        }
        return std::nullopt;
    }
} // namespace blogserver::model
