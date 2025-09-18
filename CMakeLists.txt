cmake_minimum_required(VERSION 3.14)
project(Numcpp LANGUAGES CXX)

# 设置C++标准（支持模板类特性）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 消除下载时间戳警告
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# 1. 编译主程序（直接包含头文件，无需静态库）
add_executable(numcpp_demo
    src/main.cpp
)
# 让主程序找到 src 目录下的头文件
target_include_directories(numcpp_demo
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 2. 配置测试（Google Test）
enable_testing()

# 下载Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 编译测试程序（直接包含头文件）
add_executable(numcpp_tests
    tests/test_matrix.cpp
)
# 测试程序依赖：头文件目录 + Google Test库
target_include_directories(numcpp_tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(numcpp_tests
    PRIVATE GTest::gtest_main
)

# 注册测试到CTest
include(GoogleTest)
gtest_discover_tests(
    numcpp_tests
    TEST_PREFIX "Numcpp:"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# 编译警告配置（可选，提升代码质量）
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(numcpp_demo PRIVATE -Wall -Wextra -Wno-unused-parameter)
    target_compile_options(numcpp_tests PRIVATE -Wall -Wextra -Wno-unused-parameter)
elseif(MSVC)
    target_compile_options(numcpp_demo PRIVATE /W4 /wd4100)
    target_compile_options(numcpp_tests PRIVATE /W4 /wd4100)
endif()