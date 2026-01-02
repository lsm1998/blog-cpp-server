#pragma once

#include <cppkit/http/server/http_request.hpp>
#include <cppkit/http/server/http_response.hpp>
#include "service/user/user.hpp"

namespace blogserver::api::user
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
        service::user::UserService _userService;
    };
} // namespace blogserver::model
