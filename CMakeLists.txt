cmake_minimum_required(VERSION 3.25)
project(blog-cpp-server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators")

# 包含 conan 工具链文件
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)

# 通过git 导入 cppkit 库
include(FetchContent)
FetchContent_Declare(
        cppkit
        GIT_REPOSITORY
        https://github.com/lsm1998/cppkit
        GIT_TAG main
)

# 设置构建静态库
# set(BUILD_SHARED_LIBS OFF)

# 启用 cppkit 库
FetchContent_MakeAvailable(cppkit)

add_subdirectory(src)

# 查找 postgresql 和 hiredis 包
find_package(taopq REQUIRED)
find_package(redis++ REQUIRED)
find_package(yaml-cpp REQUIRED)

target_link_libraries(blog-cpp-server PRIVATE
        cppkit
        taocpp::taopq
        redis++::redis++_static
        yaml-cpp::yaml-cpp)