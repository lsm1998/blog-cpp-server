#include "api/api.hpp"
#include "api/common.hpp"
#include "server/application.hpp"
#include "api/tag/tag.hpp"
#include "api/article/article.hpp"
#include "api/user/user.hpp"
#include <functional>
#include <cppkit/http/server/http_response.hpp>
#include <cppkit/http/server/router_group.hpp>

namespace blogserver::api
{
    using namespace cppkit::http::server;

    void authMiddleware(const HttpRequest& req, HttpResponseWriter& res, const NextFunc& next)
    {
        const auto authHeader = req.getHeader("Authorization");
        const auto accessKey = req.getHeader("X-Access-Key");
        if (authHeader.empty() || accessKey.empty())
        {
            res.setStatusCode(cppkit::http::HTTP_UNAUTHORIZED);
            res.setContentType("application/json");
            res.write(R"({"error":"Unauthorized"})");
            return;
        }

        // 获取密钥
        // const std::string expectedAccessKey = "my_secret_key";
        // req.readBody();
        next();
    }


    void registerApiRoutes(server::Application& app, HttpServer& server)
    {
        // 跨域中间件
        server.addMiddleware("/", [](HttpRequest&, HttpResponseWriter& res, const NextFunc& next)
        {
            res.setHeader("Access-Control-Allow-Origin", "*");
            res.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            next();
        });

        // 健康检查接口
        server.Get("/api/health", [](const HttpRequest&, HttpResponseWriter& res)
        {
            ok(res, "OK");
        });

        {
            // 注册文章相关的路由
            const auto article = server.group("/api/article");
            // 权限中间件
            article.use(authMiddleware);
            auto handler = std::make_shared<article::ArticleHandler>(app.context_);
            article.Get("/", [handler](const auto& req, auto& res) { handler->getList(req, res); });
            article.Get("/:id", [handler](const auto& req, auto& res) { handler->getArticleById(req, res); });
            article.Post("/", [handler](const auto& req, auto& res) { handler->createArticle(req, res); });
            article.Put("/", [handler](const auto& req, auto& res) { handler->updateArticle(req, res); });
            article.Delete("/", [handler](const auto& req, auto& res) { handler->deleteArticle(req, res); });
        }

        {
            // 注册tag相关的路由
            const auto tag = server.group("/api/tag");
            // 权限中间件
            tag.use(authMiddleware);
            auto handler = std::make_shared<tag::TagHandler>(app.context_);
            tag.Get("/", [handler](const auto& req, auto& res) { handler->tagList(req, res); });
            tag.Post("/", [handler](const auto& req, auto& res) { handler->createTag(req, res); });
        }

        {
            // 注册用户相关的路由
            const auto user = server.group("/api/user");
            // 权限中间件
            user.use(authMiddleware);
            auto handler = std::make_shared<user::UserHandler>(app.context_);
            user.Get("/:username", [handler](const auto& req, auto& res) { handler->profile(req, res); });
        }
    }
} // namespace blogserver::api
