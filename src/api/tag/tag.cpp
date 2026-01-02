#include <cppkit/json/json_parser.hpp>
#include "api/tag/tag.hpp"
#include "api/common.hpp"

namespace blogserver::api::tag
{
    // 获取标签列表
    void TagHandler::tagList(const HttpRequest& req, HttpResponseWriter& writer) const
    {
        const auto tags = this->tagService_.tagList();
        ok(writer, "Success", tags);
    }

    void TagHandler::createTag(const HttpRequest& req, HttpResponseWriter& writer) const
    {
        auto body = req.readBody();
        const auto tag = cppkit::json::fromJson<model::Tag>(std::string(body.begin(), body.end()));
        const auto result = this->tagService_.createTag(tag);
        ok(writer, result);
    }
} // namespace blogserver::api::tag
