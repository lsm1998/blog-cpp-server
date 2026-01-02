#pragma once

#include <string>
#include <cppkit/reflection/reflection.hpp>

namespace blogserver::model
{
    struct Tag
    {
        int64_t id{};
        std::string name;
        uint32_t count{};
        std::string path;
    };

    REFLECT(blogserver::model::Tag, FIELD(id), FIELD(name), FIELD(count), FIELD(path))
} // namespace blogserver::model
