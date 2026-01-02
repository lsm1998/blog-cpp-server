BUILD_DIR = build
BUILD_TYPE = Debug
CONAN_FLAGS = --build=missing --output-folder=$(BUILD_DIR)
CMAKE_FLAGS = -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(BUILD_DIR)/conan_toolchain.cmake

.PHONY: all build build-release clean run init install help

all: build

# 0. 帮助信息
help:
	@echo "可用命令:"
	@echo "  make init          - 初始化 Conan Profile (仅运行一次)"
	@echo "  make install       - 安装依赖 (当 conanfile.txt 变更时运行)"
	@echo "  make build         - 编译 Debug 版本 (日常开发用)"
	@echo "  make build-release - 编译 Release 版本 (发布用)"
	@echo "  make run           - 编译并运行程序"
	@echo "  make clean         - 清理构建文件"

init:
	conan profile detect

install:
	conan install . $(CONAN_FLAGS) -s build_type=$(BUILD_TYPE)

#install:
#	conan install . $(CONAN_FLAGS) \
#	-c tools.cmake.cmaketoolchain:extra_variables="{'CMAKE_POLICY_VERSION_MINIMUM':'3.5'}"

build:
	@mkdir -p $(BUILD_DIR)
	cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) -- -j4

build-release:
	@mkdir -p $(BUILD_DIR)
	cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) -- -j4

run: build
	./$(BUILD_DIR)/blog-cpp-server

clean:
	rm -rf $(BUILD_DIR)

clean-all: clean
	conan remove "*" -f