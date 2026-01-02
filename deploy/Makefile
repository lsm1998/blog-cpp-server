.PHONY: build run build-run clean stop logs

APP_NAME := blog-cpp-server
APP_VERSION := 1.0.0

# 构建Docker镜像
build:
	docker build -f Dockerfile -t $(APP_NAME):$(APP_VERSION) ..

build-no-cache:
	docker build --no-cache -f Dockerfile -t $(APP_NAME):$(APP_VERSION) ..

# 运行容器
run:
	docker run -it --name $(APP_NAME) -p 8888:8888 -d $(APP_NAME):$(APP_VERSION)

# 构建并运行
build-run: build run

# 清理Docker镜像
clean:
	docker rmi $(APP_NAME):$(APP_VERSION) || true

# 停止运行中的容器
stop:
	docker stop $(APP_NAME) || true

# 查看容器日志
logs:
	docker logs -f $(APP_NAME)