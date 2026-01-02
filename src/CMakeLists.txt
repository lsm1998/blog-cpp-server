# 导入include目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 获取目录下所有的源文件
file(GLOB_RECURSE SRC_FILES "*.cpp" "*.c" "*.h" "*.hpp")

add_executable(blog-cpp-server ${SRC_FILES})

# 设置可执行文件的输出目录
set_target_properties(blog-cpp-server PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)