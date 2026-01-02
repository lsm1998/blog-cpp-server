#include "api/article/article.hpp"
#include "api/common.hpp"
#include "cppkit/json/json_parser.hpp"
#include "cppkit/log/log.hpp"

namespace blogserver::api::article
{
    void ArticleHandler::getList(const HttpRequest& req, HttpResponseWriter& writer) const
    {
        try
        {
            const auto articles = this->articleService_.ArticleList();
            ok(writer, articles);
        }
        catch (const std::exception& e)
        {
            CK_LOG_ERROR("Failed to get article list: {}", e.what());
            error(writer, "Failed to get article list", cppkit::http::HTTP_INTERNAL_SERVER_ERROR);
        }
    }

    void ArticleHandler::getArticleById(const HttpRequest& req, HttpResponseWriter& writer) const
    {
        const auto id = req.getParam("id");
        try
        {
            auto opt = this->articleService_.GetArticleById(static_cast<int>(std::stoull(id)));
            if (opt.has_value())
            {
                ok(writer, "Success", opt.value());
            }
            else
            {
                ok(writer);
            }
        }
        catch (const std::exception& e)
        {
            CK_LOG_ERROR("Failed to get article by id: {}", e.what());
            error(writer, "Invalid article ID", cppkit::http::HTTP_BAD_REQUEST);
        }
    }

    void ArticleHandler::createArticle(const HttpRequest& req, HttpResponseWriter& writer) const
    {
        auto body = req.readBody();
        const auto article = cppkit::json::fromJson<model::Article>(std::string(body.begin(), body.end()));
        const auto result = this->articleService_.CreateArticle(article);
        ok(writer, result);
    }

    void ArticleHandler::updateArticle(const HttpRequest&, HttpResponseWriter&)
    {
    }

    void ArticleHandler::deleteArticle(const HttpRequest&, HttpResponseWriter&)
    {
    }
}
