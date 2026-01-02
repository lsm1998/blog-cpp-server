#pragma once

#include "cppkit/reflection/reflection.hpp"
#include <string>

namespace blogserver::model
{
    struct Article
    {
        int64_t id{};
        std::string title{};
        std::string content{};
        std::string author{};
        std::vector<int64_t> tags{};
        std::string created_at{};
        std::string updated_at{};
    };

    REFLECT(blogserver::model::Article, FIELD(id), FIELD(title), FIELD(content),
            FIELD(author), FIELD(created_at), FIELD(updated_at), FIELD(tags))
} // namespace blogserver::server
