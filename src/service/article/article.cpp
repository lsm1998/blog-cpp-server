#include "service/article/article.hpp"
#include "cppkit/array.hpp"
#include "cppkit/strings.hpp"

namespace blogserver::service::article
{
    constexpr auto queryArticleByIdSql = "SELECT id,title,content,tags FROM article WHERE id = $1";

    constexpr auto insertArticleSql = "INSERT INTO article (title, content) VALUES ($1, $2) RETURNING id";

    std::vector<int64_t> getTagIdsByNames(const server::ServerContext& ctx, const std::vector<std::string>& tagNames)
    {
        std::vector<std::string> args;
        std::vector<int64_t> tagIds;
        if (tagNames.empty())
        {
            return tagIds;
        }

        std::ostringstream sql;
        sql << "SELECT id FROM tag WHERE name IN (";
        for (size_t i = 0; i < tagNames.size(); ++i)
        {
            sql << "$" << (i + 1);
            args.emplace_back(tagNames[i]);
            if (i < tagNames.size() - 1)
            {
                sql << ", ";
            }
        }
        sql << ");";
        const auto result = ctx.dbConnPool_->executeDynamic(sql.str(), args);
        for (const auto& row : result)
        {
            tagIds.push_back(row["id"].as<int64_t>());
        }
        return tagIds;
    }

    std::tuple<std::vector<model::Article>, int> ArticleService::getArticleList(ArticleListParams& params) const
    {
        const auto offset = (params.page - 1) * params.pageSize;

        std::vector<model::Article> articles;
        std::vector<std::string> args{};

        // 先根据tagNames获取tagIds
        if (!params.tagNames.empty())
        {
            params.tagIds = getTagIdsByNames(this->ctx, params.tagNames);
            if (params.tagIds.empty())
            {
                return {};
            }
        }

        std::ostringstream listSql;
        std::ostringstream countSql;
        listSql << "SELECT id, title, content, tags FROM article";
        countSql << "SELECT COUNT(*) FROM article";

        if (!params.tagIds.empty())
        {
            listSql << " WHERE tags && $" << 1 + args.size();
            countSql << " WHERE tags && $" << 1 + args.size();
            const auto tagIdsStr = cppkit::join(cppkit::arrayMap(params.tagIds, [&](const int64_t tagId)
            {
                return std::to_string(tagId);
            }), ",");
            args.emplace_back("{" + tagIdsStr + "}");
        }

        // 获取总数
        int total = 0;
        const auto countResult = this->ctx.dbConnPool_->executeDynamic(countSql.str(), args);
        if (!countResult.empty())
        {
            total = countResult[0]["count"].as<int>();
        }

        listSql << " ORDER BY created_at DESC LIMIT $" << 1 + args.size() << " OFFSET $" << 2 + args.size() << ";";

        args.push_back(std::to_string(params.pageSize));
        args.push_back(std::to_string(offset));
        const auto result = this->ctx.dbConnPool_->executeDynamic(listSql.str(), args);
        for (const auto& row : result)
        {
            model::Article article;
            article.id = row["id"].as<int64_t>();
            article.title = row["title"].as<std::string>();
            article.content = row["content"].as<std::string>();
            article.tags = row["tags"].as<std::vector<std::int64_t>>();
            articles.push_back(article);
        }
        return {articles, total};
    }

    std::optional<model::Article> ArticleService::getArticleById(int articleId) const
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

    int64_t ArticleService::createArticle(const model::Article& article) const
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

    void ArticleService::updateArticle(const model::Article& article)
    {
    }

    void ArticleService::deleteArticle(std::vector<int64_t> articleId)
    {
    }
}
