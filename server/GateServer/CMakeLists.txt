cmake_minimum_required(VERSION 3.16)

# 指定vcpkg的路径
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(GateServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# jsoncpp
set(JSONCPP_DIR "E:/File/Library/Cpp/jsoncpp-1.9.7")

# boost
set(Boost_DIR "E:/File/Library/Cpp/boost_1_89_0")

# gRPC
find_package(gRPC CONFIG REQUIRED)

# redis++
find_package(redis++ CONFIG REQUIRED)

add_executable(GateServer 
    # 源文件
    asioIOContextPool.cpp
    configManager.cpp
    CServer.cpp
    GateServer.cpp
    httpConnection.cpp
    logicSystem.cpp
    utils.cpp
    varifyGrpcClient.cpp
    message.grpc.pb.cc
    message.pb.cc

    # 头文件
    asioIOContextPool.h
    configManager.h
    CServer.h
    httpConnection.h
    logicSystem.h
    utils.h
    singleton.h
    const.h
    message.grpc.pb.h
    message.pb.h
    varifyGrpcClient.h
)

target_include_directories(GateServer
    PRIVATE
        "${JSONCPP_DIR}/vs2022_release/include"
        "${Boost_DIR}"
)

target_link_libraries(GateServer
    PRIVATE 
        # jsoncpp
        optimized "${JSONCPP_DIR}/vs2022_release/lib/jsoncpp_static.lib"
        debug "${JSONCPP_DIR}/vs2022_debug/lib/jsoncpp_static.lib"
        # gRPC
        gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts
        # redis++
        redis++::redis++
)