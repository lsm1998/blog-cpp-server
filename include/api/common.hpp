#pragma once

#include "cppkit/http/server/http_server.hpp"
#include "cppkit/json/json.hpp"

namespace blogserver::api
{
    using namespace cppkit::http::server;

    template <typename T>
    void ok(HttpResponseWriter& writer, const std::string& message = "Success", const T& data = T())
    {
        // 判断data是否为空对象
        if constexpr (std::is_same_v<T, std::nullptr_t>)
        {
            writer.setStatusCode(cppkit::http::HTTP_OK);
            writer.setHeader("Content-Type", "application/json");
            writer.write(R"({"message":")" + message + R"(", "code":200})");
        }
        else
        {
            writer.setStatusCode(cppkit::http::HTTP_OK);
            writer.setHeader("Content-Type", "application/json");
            writer.write(
                R"({"message":")" + message + R"(", "code":200,"data":)" + cppkit::json::stringify(data) + R"(})");
        }
    }

    inline void ok(HttpResponseWriter& writer, const std::string& message = "Success")
    {
        ok(writer, message, nullptr);
    }


    template <typename T>
    void ok(HttpResponseWriter& writer, const T& data = T())
    {
        ok(writer, "Success", data);
    }


    inline void error(HttpResponseWriter& writer, const std::string& message = "Error",
                      const int code = cppkit::http::HTTP_INTERNAL_SERVER_ERROR)
    {
        writer.setStatusCode(code);
        writer.setHeader("Content-Type", "application/json");
        writer.write(R"({"message":")" + message + R"(", "code":)" + std::to_string(code) + R"(})");
    }
}
