#pragma once

#include <vector>
#include <optional>
#include "server/server_context.hpp"
#include "model/article.hpp"
#include "repository/article/article_repo.h"

namespace blogserver::service::article
{
    class ArticleService
    {
    public:
        explicit ArticleService(server::ServerContext& ctx) : ctx(ctx)
        {
        }

        ~ArticleService() = default;

        // 获取文章列表
        std::vector<model::Article> ArticleList() const;

        // 获取单篇文章
        [[nodiscard]] std::optional<model::Article> GetArticleById(int articleId) const;

        // 创建新文章
        [[nodiscard]] int64_t CreateArticle(const model::Article& article) const;

        // 更新文章
        void UpdateArticle(const model::Article& article);

        // 删除文章
        void DeleteArticle(std::vector<int64_t> articleId);

    private:
        server::ServerContext& ctx;
    };
} // namespace blogserver::service::article
