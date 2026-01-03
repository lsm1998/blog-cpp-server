#pragma once

#include "model/article.hpp"
#include <cppkit/reflection/reflection.hpp>
#include <string>

namespace blogserver::model::dto
{
    struct ArticleItemDto : Article
    {
        std::vector<std::string> tagNames{};
        std::string date;

        void setInfo(const Article& article)
        {
            this->id = article.id;
            this->title = article.title;
            this->content = article.content;
            this->summary = article.summary;
            this->tags = article.tags;
            this->created_at = article.created_at;
            this->updated_at = article.updated_at;
            this->date = article.created_at;
            // 截取日期部分
            if (this->date.length() >= 10)
            {
                this->date = this->date.substr(0, 10);
            }
        }
    };

    struct ArticleDetailDto : ArticleItemDto
    {
    };

    REFLECT(blogserver::model::dto::ArticleItemDto, FIELD(id), FIELD(title),
            FIELD(summary), FIELD(created_at), FIELD(tagNames), FIELD(date))

    REFLECT(blogserver::model::dto::ArticleDetailDto, FIELD(id), FIELD(title),
            FIELD(summary), FIELD(created_at), FIELD(tagNames), FIELD(date), FIELD(content))
} // namespace blogserver::server
