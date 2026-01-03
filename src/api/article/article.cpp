#include "api/article/article.hpp"
#include "api/common.hpp"
#include "cppkit/json/json_parser.hpp"
#include "cppkit/json/json.hpp"
#include "cppkit/log/log.hpp"

namespace blogserver::api::article
{
    void ArticleHandler::getList(const HttpRequest& req, HttpResponseWriter& writer) const
    {
        try
        {
            service::article::ArticleListParams params;
            const auto page = req.getQuery("page");
            const auto pageSize = req.getQuery("pageSize");
            const auto tag = req.getQuery("tag");
            if (!page.empty())
            {
                params.page = static_cast<int>(std::stoull(page));
            }
            if (!pageSize.empty())
            {
                params.pageSize = static_cast<int>(std::stoull(pageSize));
            }
            if (!tag.empty())
            {
                params.tagNames.push_back(tag);
            }
            const auto articles = this->articleService_.getArticleList(params);
            auto [articleList, total] = articles;

            auto tagList = this->tagService_.tagList();
            std::map<int64_t, std::string> tagMap;
            for (const auto& v : tagList)
            {
                tagMap[v.id] = v.name;
            }
            // 填充tagNames
            for (auto& article : articleList)
            {
                article.tagNames.clear();
                for (const auto& tagId : article.tags)
                {
                    if (tagMap.contains(tagId))
                    {
                        article.tagNames.push_back(tagMap[tagId]);
                    }
                }
            }

            auto result = cppkit::json::Json();
            result["list"] = articleList;
            result["total"] = total;
            ok(writer, result);
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
            auto opt = this->articleService_.getArticleById(static_cast<int>(std::stoull(id)));
            if (opt.has_value())
            {
                // 返回文章内容时，同时返回标签名称
                auto tagList = this->tagService_.tagList();
                std::map<int64_t, std::string> tagMap;
                for (const auto& v : tagList)
                {
                    tagMap[v.id] = v.name;
                }
                // 填充tagNames
                std::vector<std::string> tagNames;
                for (const auto& tagId : opt->tags)
                {
                    if (tagMap.contains(tagId))
                    {
                        tagNames.push_back(tagMap[tagId]);
                    }
                }
                opt.value().tagNames = tagNames;
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
        const auto result = this->articleService_.createArticle(article);
        ok(writer, result);
    }

    void ArticleHandler::updateArticle(const HttpRequest&, HttpResponseWriter&)
    {
    }

    void ArticleHandler::deleteArticle(const HttpRequest&, HttpResponseWriter&)
    {
    }
}
