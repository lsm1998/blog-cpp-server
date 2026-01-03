#include "service/article/article.hpp"
#include "cppkit/array.hpp"
#include "cppkit/strings.hpp"

namespace blogserver::service::article
{
    constexpr auto queryArticleByIdSql = "SELECT id,title,content,tags,created_at FROM article WHERE id = $1";

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

    std::tuple<std::vector<model::dto::ArticleItemDto>, int> ArticleService::getArticleList(
        ArticleListParams& params) const
    {
        const auto offset = (params.page - 1) * params.pageSize;

        if (!params.tagNames.empty())
        {
            params.tagIds = getTagIdsByNames(this->ctx, params.tagNames);
            if (params.tagIds.empty()) // 没有匹配的标签，直接返回空列表
            {
                return {{}, 0};
            }
        }

        std::vector<model::dto::ArticleItemDto> articles;
        std::vector<std::string> args{};

        std::ostringstream whereSql;
        int paramIdx = 1;
        bool hasWhere = false;

        auto appendCondition = [&](const std::string& condition)
        {
            if (!hasWhere)
            {
                whereSql << " WHERE ";
                hasWhere = true;
            }
            else
            {
                whereSql << " AND ";
            }
            whereSql << condition;
        };

        if (!params.tagIds.empty())
        {
            std::ostringstream cond;
            cond << "tags && $" << paramIdx++;
            appendCondition(cond.str());

            const auto tagIdsStr = cppkit::join(cppkit::arrayMap(params.tagIds, [&](const int64_t tagId)
            {
                return std::to_string(tagId);
            }), ",");
            args.emplace_back("{" + tagIdsStr + "}");
        }

        if (!params.keyword.empty())
        {
            std::ostringstream cond;
            cond << "(title ILIKE $" << paramIdx << " OR summary ILIKE $" << paramIdx << ")";
            appendCondition(cond.str());

            paramIdx++;
            args.emplace_back("%" + params.keyword + "%");
        }

        std::string countSqlStr = "SELECT COUNT(*) FROM article" + whereSql.str();

        int total = 0;
        const auto countResult = this->ctx.dbConnPool_->executeDynamic(countSqlStr, args);
        if (!countResult.empty())
        {
            total = countResult[0]["count"].as<int>();
        }

        // 如果总数为 0，没必要查列表了，直接返回
        if (total == 0)
        {
            return {articles, 0};
        }

        std::ostringstream listSql;
        listSql << "SELECT id, title, tags, created_at, summary FROM article"
            << whereSql.str()
            << " ORDER BY created_at DESC";

        listSql << " LIMIT $" << paramIdx++;
        args.push_back(std::to_string(params.pageSize));

        listSql << " OFFSET $" << paramIdx++;
        args.push_back(std::to_string(offset));

        const auto result = this->ctx.dbConnPool_->executeDynamic(listSql.str(), args);

        for (const auto& row : result)
        {
            model::Article article;
            article.id = row["id"].as<int64_t>();
            article.summary = row["summary"].as<std::string>();
            article.title = row["title"].as<std::string>();
            article.tags = row["tags"].as<std::vector<std::int64_t>>();
            article.created_at = row["created_at"].as<std::string>();

            model::dto::ArticleItemDto articleDto;
            articleDto.setInfo(article);
            articles.push_back(articleDto);
        }

        return {articles, total};
    }

    std::optional<model::dto::ArticleDetailDto> ArticleService::getArticleById(int articleId) const
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
        article.created_at = row["created_at"].as<std::string>();
        model::dto::ArticleDetailDto articleDto;
        articleDto.setInfo(article);
        return articleDto;
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
