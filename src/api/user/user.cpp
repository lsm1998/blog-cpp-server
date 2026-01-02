#include "api/user/user.hpp"

#include "api/common.hpp"

namespace blogserver::api::user
{
    void UserHandler::profile(const HttpRequest& req, HttpResponseWriter& writer) const
    {
        const auto username = req.getParam("username");
        const auto user = this->_userService.getUserProfile(username);
        if (user.has_value())
        {
            ok(writer, user.value());
            return;
        }
        error(writer, "User not found");
    }
} // namespace blogserver::api::user
