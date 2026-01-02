#include "service/tag/tag.hpp"
#include "cppkit/log/log.hpp"

namespace blogserver::service::tag
{
    // 获取标签列表
    std::vector<model::Tag> TagService::tagList() const
    {
        try
        {
            std::vector<model::Tag> tags;

            const auto result = this->ctx.dbConnPool_->execute("SELECT id,name,count,path FROM tag order by count desc");
            for (const auto& row : result)
            {
                model::Tag tag;
                tag.id = row["id"].as<int64_t>();
                tag.name = row["name"].as<std::string>();
                tag.count = row["count"].as<int64_t>();
                tag.path = row["path"].as<std::string>();
                tags.push_back(tag);
            }
            return tags;
        }
        catch (const std::exception& e)
        {
            CK_LOG_ERROR("Failed to fetch tag list: {}", e.what());
            return {};
        }
    }

    int64_t TagService::createTag(const model::Tag& tag) const
    {
        try
        {
            const auto insertSql = "INSERT INTO tag (name, path) VALUES ($1, $2) RETURNING id";
            const auto result = this->ctx.dbConnPool_->execute(insertSql, tag.name, tag.path);
            if (!result.empty())
            {
                const auto& row = result[0];
                return row["id"].as<int64_t>();
            }
        }
        catch (const std::exception& e)
        {
            CK_LOG_ERROR("Failed to create tag: {}", e.what());
        }
        return 0;
    }
} // namespace blogserver::service::tag
