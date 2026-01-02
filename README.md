# blog-cpp-server

一个基于 C++20 构建的高性能博客服务器，[cppkit](https://github.com/lsm1998/cppkit)库的一次实践项目。

## 技术栈

- **语言**: C++20
- **核心库**: cppkit (https://github.com/lsm1998/cppkit)
- **数据库**: PostgreSQL (taopq 客户端)
- **缓存**: Redis (redis++ 客户端)
- **配置**: YAML (yaml-cpp)
- **构建工具**: CMake
- **依赖管理**: Conan/FetchContent

## 项目结构

```
blog-cpp-server/
├── include/          # 头文件目录
│   ├── api/          # API 层 (HTTP 请求处理)
│   ├── cfg/          # 配置管理
│   ├── model/        # 数据模型
│   ├── repository/   # 数据访问层
│   ├── server/       # 服务器核心
│   └── service/      # 业务逻辑层
├── src/              # 源文件目录
│   ├── api/
│   ├── cfg/
│   ├── repository/
│   ├── server/
│   ├── service/
│   └── main.cpp      # 项目入口
├── build/            # 构建目录
├── cfg/              # 配置文件目录
├── CMakeLists.txt    # CMake 构建文件
├── conanfile.txt     # Conan 依赖配置
└── README.md         # 项目说明文档
```

## 功能模块

### 1. 用户模块 (Users)
- 用户资料管理
- (更多功能待实现...)

### 2. 文章模块 (Article)
- 文章的增删改查
- (更多功能待实现...)

### 3. 标签模块 (Tag)
- 标签的增删改查
- (更多功能待实现...)

## 构建与运行

### 环境要求
- C++20 编译器 (支持 GCC 10+, Clang 11+, MSVC 2019+)
- CMake 3.25+
- PostgreSQL 14+
- Redis 6+

### 构建步骤

项目提供了三种构建方式供选择：

#### 方式一: 使用 Makefile (推荐，提供快捷命令)

项目根目录下的 `Makefile` 包含了常用的构建命令：

```bash
# 初始化 Conan Profile (仅首次运行)
make init

# 安装依赖
make install

# 编译 Debug 版本
make build

# 编译 Release 版本
make build-release

# 编译并运行
make run

# 清理构建文件
make clean
```

#### 方式二: 使用 CMake 和 FetchContent

1. 克隆项目:
   ```bash
   git clone https://github.com/your-repo/blog-cpp-server.git
   cd blog-cpp-server
   ```

2. 创建并进入构建目录:
   ```bash
   mkdir build && cd build
   ```

3. 使用 CMake 配置和构建:
   ```bash
   cmake ..
   make
   ```

#### 方式三: 使用 Conan 依赖管理

1. 克隆项目并进入目录

2. 创建并进入构建目录:
   ```bash
   mkdir build && cd build
   ```

3. 安装 Conan 依赖:
   ```bash
   conan install .. --output-folder=. --build=missing
   ```

4. 使用 CMake 配置和构建:
   ```bash
   cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
   make
   ```

### 运行步骤

1. **配置服务器**
   项目提供了开发和生产环境的配置文件模板：
   ```bash
   # 开发环境配置
   cfg/app.dev.yaml

   # 生产环境配置
   cfg/app.prod.yaml
   ```
   需要根据实际环境修改数据库、Redis、服务器端口等配置项。

2. 运行服务器:
   ```bash
   ./blog-cpp-server -c cfg/app.dev.yaml
   ```

## 开发

### 代码风格
- 遵循 C++ Core Guidelines
- 使用现代 C++ 特性

### 依赖管理
项目使用 cppkit 库，通过 FetchContent 自动从 GitHub 拉取。

## 部署

项目提供了Docker部署支持，相关文件位于 `deploy/` 目录：

```bash
# Dockerfile - 用于构建 Docker 镜像
deploy/Dockerfile

# table.sql - 数据库表结构初始化脚本
deploy/table.sql

# Makefile - 部署相关的快捷命令
deploy/Makefile
```

## 未来计划

- 完整的用户认证与授权系统
- 文章分类与标签功能
- 评论系统
- 文件上传与下载
- 后台管理界面
- 性能监控与日志系统

