#include "server/application.hpp"

int main(const int argc, char* argv[])
{
    using namespace blogserver::server;
    Application app(argc, argv);

    // 初始化资源
    app.init();

    // 运行应用
    app.run();
}
