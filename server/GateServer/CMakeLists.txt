cmake_minimum_required(VERSION 3.16)

### 指定vcpkg的路径
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()

project(GateServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### 硬编码设置包路径
# jsoncpp
set(JSONCPP_DIR "E:/File/Library/Cpp/jsoncpp-1.9.7")

# boost
set(Boost_DIR "E:/File/Library/Cpp/boost_1_89_0")

### 自动查找包
# gRPC
find_package(gRPC CONFIG REQUIRED)

# redis++
find_package(redis++ CONFIG REQUIRED)

# libpqxx
find_package(libpqxx CONFIG REQUIRED)

### 自动查找文件
# src下所有的.cpp和.h文件 (递归)
file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# generated下所有的.cpp和.h文件
file(GLOB GENERATED_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.h"
)

### 按物理目录结构
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCES} ${GENERATED_SOURCES})

### 添加可执行程序及相关代码
add_executable(GateServer
    ${PROJECT_SOURCES}
    ${GENERATED_SOURCES}
)

### 目标可执行程序相关头文件
target_include_directories(GateServer
    PRIVATE
        # 本地头文件
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/generated
        # 第三方
        "${JSONCPP_DIR}/vs2022_release/include"
        "${Boost_DIR}"
)

### 目标可执行程序链接库
target_link_libraries(GateServer
    PRIVATE
        # jsoncpp
        optimized "${JSONCPP_DIR}/vs2022_release/lib/jsoncpp_static.lib"
        debug "${JSONCPP_DIR}/vs2022_debug/lib/jsoncpp_static.lib"
        # gRPC
        gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts
        # redis++
        redis++::redis++
        # libpqxx
        libpqxx::pqxx
)
