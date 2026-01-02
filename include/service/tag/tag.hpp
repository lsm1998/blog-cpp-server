#pragma once

#include "server/server_context.hpp"
#include "model/tag.hpp"

namespace blogserver::service::tag
{
    class TagService
    {
    public:
        explicit TagService(server::ServerContext& ctx) : ctx(ctx)
        {
        }

        ~TagService() = default;

        // 获取标签列表
        [[nodiscard]] std::vector<model::Tag> tagList() const;

        [[nodiscard]] int64_t createTag(const model::Tag& tag) const;

    private:
        server::ServerContext& ctx;
    };
}
