#pragma once

#include <cppkit/http/server/http_request.hpp>
#include <cppkit/http/server/http_response.hpp>
#include "server/server_context.hpp"
#include "service/article/article.hpp"

namespace blogserver::api::article
{
    using namespace cppkit::http::server;

    class ArticleHandler
    {
    public:
        explicit ArticleHandler(server::ServerContext& ctx) : ctx(ctx), articleService_(ctx)
        {
        }

        ~ArticleHandler() = default;

        // 获取文章列表
        void getList(const HttpRequest&, HttpResponseWriter&) const;

        // 获取单篇文章
        void getArticleById(const HttpRequest&, HttpResponseWriter&) const;

        // 创建新文章
        void createArticle(const HttpRequest&, HttpResponseWriter&) const;

        // 更新文章
        void updateArticle(const HttpRequest&, HttpResponseWriter&);

        // 删除文章
        void deleteArticle(const HttpRequest&, HttpResponseWriter&);

    private:
        server::ServerContext& ctx;
        service::article::ArticleService articleService_;
    };
} // namespace blogserver::server
