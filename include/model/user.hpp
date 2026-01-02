#pragma once

#include <string>
#include <cppkit/reflection/reflection.hpp>

namespace blogserver::model
{
    struct User
    {
        int64_t id{};
        std::string username{};
        std::string nickname{};
        std::string email{};
        std::string headshot{};
        std::string description{};
        std::string position{};
        std::string area{};
        std::string github{};
        std::string telegram{};
        std::string linkedin{};
        std::string twitter{};
    };

    REFLECT(blogserver::model::User, FIELD(id), FIELD(username), FIELD(nickname), FIELD(email),
            FIELD(headshot), FIELD(description), FIELD(position), FIELD(area),
            FIELD(github), FIELD(telegram), FIELD(linkedin), FIELD(twitter))
} // namespace blogserver::model
