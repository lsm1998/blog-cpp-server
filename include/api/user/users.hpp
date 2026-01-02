#pragma once

#include <cppkit/http/server/http_request.hpp>
#include <cppkit/http/server/http_response.hpp>
#include "service/users/users.hpp"

namespace blogserver::api::users
{
    using namespace cppkit::http::server;

    class UserHandler
    {
    public:
        explicit UserHandler(server::ServerContext& ctx) : _userService(ctx)
        {
        }

        ~UserHandler() = default;

        // 获取用户资料
        void profile(const HttpRequest&, HttpResponseWriter&) const;

    private:
        service::users::UserService _userService;
    };
} // namespace blogserver::model
