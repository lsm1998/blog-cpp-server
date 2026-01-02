#pragma once

#include <cppkit/http/server/http_request.hpp>
#include <cppkit/http/server/http_response.hpp>
#include "server/server_context.hpp"
#include "service/tag/tag.hpp"

namespace blogserver::api::tag
{
    using namespace cppkit::http::server;

    class TagHandler
    {
    public:
        explicit TagHandler(server::ServerContext& ctx) : ctx(ctx), tagService_(ctx)
        {
        }

        ~TagHandler() = default;

        // 获取标签列表
        void tagList(const HttpRequest&, HttpResponseWriter&) const;

        void createTag(const HttpRequest&, HttpResponseWriter&) const;

    private:
        server::ServerContext& ctx;
        service::tag::TagService tagService_;
    };
}
