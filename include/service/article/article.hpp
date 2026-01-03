#pragma once

#include <vector>
#include <optional>
#include "server/server_context.hpp"
#include "model/article.hpp"
#include "repository/article/article_repo.h"

namespace blogserver::service::article
{
    struct ArticleListParams
    {
        int page{1};
        int pageSize{10};
        std::vector<std::string> tagNames{};
        std::vector<int64_t> tagIds{};
    };

    class ArticleService
    {
    public:
        explicit ArticleService(server::ServerContext& ctx) : ctx(ctx)
        {
        }

        ~ArticleService() = default;

        // 获取文章列表
        [[nodiscard]] std::tuple<std::vector<model::Article>, int> getArticleList(ArticleListParams& params) const;

        // 获取单篇文章
        [[nodiscard]] std::optional<model::Article> getArticleById(int articleId) const;

        // 创建新文章
        [[nodiscard]] int64_t createArticle(const model::Article& article) const;

        // 更新文章
        void updateArticle(const model::Article& article);

        // 删除文章
        void deleteArticle(std::vector<int64_t> articleId);

    private:
        server::ServerContext& ctx;
    };
} // namespace blogserver::service::article
