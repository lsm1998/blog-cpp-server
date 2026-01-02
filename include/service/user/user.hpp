#pragma once

#include "server/server_context.hpp"
#include "model/user.hpp"

namespace blogserver::service::user
{
    class UserService
    {
    public:
        explicit UserService(server::ServerContext& ctx) : ctx(ctx)
        {
        }

        ~UserService() = default;

        // 获取用户资料
        [[nodiscard]] std::optional<model::User> getUserProfile(std::string_view username) const;

    private:
        server::ServerContext& ctx;
    };
} // namespace blogserver::model
