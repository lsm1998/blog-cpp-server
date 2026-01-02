#pragma once

#include "cppkit/http/server/http_server.hpp"
#include "api/article/article.hpp"

namespace blogserver::api
{
    using namespace cppkit::http::server;

    void registerApiRoutes(server::Application& app, HttpServer& server);
} // namespace blogserver::server
