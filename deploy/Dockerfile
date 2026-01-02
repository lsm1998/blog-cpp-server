# 阶段 1编译环境
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    libpq-dev \
    libssl-dev \
    pipx \
    && rm -rf /var/lib/apt/lists/*

# 2. 安装 Conan
RUN pipx ensurepath && pipx install conan
ENV PATH="/root/.local/bin:${PATH}"

# 3. 初始化 Conan Profile
RUN conan profile detect --force

# 4. 设置工作目录
WORKDIR /app

RUN ls -la /app

COPY conanfile.txt .

RUN conan install . --build=missing --output-folder=build -s build_type=Release

COPY . .

WORKDIR /app
RUN cmake -S . -B build \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release -j$(nproc)

RUN mkdir -p /app/libs_collected && \
    find /app/build -name "libcppkit.so*" -exec cp -d {} /app/libs_collected/ \; && \
    ls -l /app/libs_collected

# 阶段2 运行环境
FROM ubuntu:24.04 AS runner

# 安装运行时依赖
RUN apt-get update && apt-get install -y \
    libpq5 \
    libssl3 \
    ca-certificates \
    tzdata \
    && rm -rf /var/lib/apt/lists/*

# 设置时区
ENV TZ=Asia/Shanghai

# 创建非 root 用户
RUN useradd -m -s /bin/bash cppuser
USER cppuser
WORKDIR /home/cppuser

# 复制编译好的二进制文件
COPY --from=builder /app/build/bin/blog-cpp-server .

# 复制配置文件
COPY --from=builder /app/cfg/app.prod.yaml .

# 复制共享库文件
COPY --from=builder /app/libs_collected/* /usr/lib/

# 暴露端口
EXPOSE 8888

# 启动
ENTRYPOINT ["./blog-cpp-server", "-c" , "app.prod.yaml"]