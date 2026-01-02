#pragma once

#include "server_context.hpp"

namespace blogserver::server
{
    class Application
    {
    public:
        ServerContext context_{};

        explicit Application(const int argc = 0, char** argv = nullptr)
            : argc_(argc), argv_(argv)
        {
        }

        ~Application() = default;

        void init();

        void run();

    private:
        int argc_;
        char** argv_;
    };
}
