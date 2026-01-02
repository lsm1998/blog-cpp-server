#pragma once

namespace blogserver::repository::article
{
    constexpr auto queryArticleByIdSql = "SELECT id, title, content FROM articles WHERE id = $1";

    class ArticleRepo
    {
    public:
        explicit ArticleRepo(server::ServerContext& ctx) : ctx(ctx)
        {
        }

        ~ArticleRepo() = default;

    private:
        server::ServerContext& ctx;
    };
} // namespace blogserver::repository::article
