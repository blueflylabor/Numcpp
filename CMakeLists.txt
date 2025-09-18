cmake_minimum_required(VERSION 3.14)
project(MultiArrayProject LANGUAGES CXX)

# 1. 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. 【核心】将核心功能编译为静态库（libmul_array_core.a）
# 包含核心函数实现（src/mul_array.cpp），但排除主程序的main函数
add_library(mul_array_core STATIC
    src/mul_array.cpp
)
# 让库能找到自己的头文件
target_include_directories(mul_array_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 3. 编译主程序（仅包含main函数，链接核心库）
add_executable(mul_array
    src/main.cpp  # 新增：将原mul_array.cpp的main函数抽离到src/main.cpp
)
# 主程序链接核心库，获取所有核心功能
target_link_libraries(mul_array
    PRIVATE mul_array_core
)

# 4. 配置CTest和Google Test
enable_testing()

# 4.1 下载Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 4.2 编译测试程序（链接核心库和gtest）
add_executable(mul_array_tests
    tests/test_mul_array.cpp
)
# 测试程序链接核心库（获取compute_strides、add等函数实现）
target_link_libraries(mul_array_tests
    PRIVATE mul_array_core
    PRIVATE GTest::gtest_main
)
# 测试程序找到头文件
target_include_directories(mul_array_tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 4.3 注册测试到CTest
include(GoogleTest)
gtest_discover_tests(
    mul_array_tests
    TEST_PREFIX "MultiArray:"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# 5. 编译警告配置
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mul_array_core PRIVATE -Wall -Wextra -Wno-unused-parameter)
    target_compile_options(mul_array PRIVATE -Wall -Wextra -Wno-unused-parameter)
    target_compile_options(mul_array_tests PRIVATE -Wall -Wextra -Wno-unused-parameter)
elseif(MSVC)
    target_compile_options(mul_array_core PRIVATE /W4 /wd4100)
    target_compile_options(mul_array PRIVATE /W4 /wd4100)
    target_compile_options(mul_array_tests PRIVATE /W4 /wd4100)
endif()