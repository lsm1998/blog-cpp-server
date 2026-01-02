#include "service/article/article.hpp"

namespace blogserver::service::article
{
    constexpr auto queryArticleByIdSql = "SELECT id,title,content,tags FROM article WHERE id = $1";

    constexpr auto listArticlesSql = "SELECT id,title,content,tags FROM article ORDER BY created_at DESC";

    constexpr auto insertArticleSql = "INSERT INTO article (title, content) VALUES ($1, $2) RETURNING id";

    std::vector<model::Article> ArticleService::ArticleList() const
    {
        std::vector<model::Article> articles;
        const auto result = this->ctx.dbConnPool_->execute(listArticlesSql);
        for (const auto& row : result)
        {
            model::Article article;
            article.id = row["id"].as<int64_t>();
            article.title = row["title"].as<std::string>();
            article.content = row["content"].as<std::string>();
            article.tags = row["tags"].as<std::vector<std::int64_t>>();
            articles.push_back(article);
        }
        return articles;
    }

    std::optional<model::Article> ArticleService::GetArticleById(int articleId) const
    {
        const auto result = this->ctx.dbConnPool_->execute(queryArticleByIdSql, articleId);
        if (result.empty())
        {
            return std::nullopt;
        }
        const auto& row = result[0];
        model::Article article;
        article.id = row["id"].as<int64_t>();
        article.title = row["title"].as<std::string>();
        article.content = row["content"].as<std::string>();
        article.tags = row["tags"].as<std::vector<std::int64_t>>();
        return article;
    }

    int64_t ArticleService::CreateArticle(const model::Article& article) const
    {
        const auto result = this->ctx.dbConnPool_->execute(insertArticleSql,
                                                           article.title,
                                                           article.content);
        if (!result.empty())
        {
            const auto& row = result[0];
            const auto newId = row["id"].as<int64_t>();
            return newId;
        }
        return 0;
    }

    void ArticleService::UpdateArticle(const model::Article& article)
    {
    }

    void ArticleService::DeleteArticle(std::vector<int64_t> articleId)
    {
    }
}
