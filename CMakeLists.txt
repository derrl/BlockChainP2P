cmake_minimum_required(VERSION 3.13)
project(myBlockChainP2P)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(include/rpclib)
include_directories(include/rpclib/include)
include_directories(include/rpclib/dependencies/include)

add_executable(myBlockChain-peer-star main.cpp block.cpp block.h peer.cpp peer.h include/sha256.cpp include/sha256.h)
add_dependencies(myBlockChain-peer-star rpc)
target_link_libraries(myBlockChain-peer-star rpc)

add_executable(myBlockChain-peer-2dmesh main-2dmesh.cpp block.cpp block.h peer.cpp peer.h include/sha256.cpp include/sha256.h)
add_dependencies(myBlockChain-peer-2dmesh rpc)
target_link_libraries(myBlockChain-peer-2dmesh rpc)

#target_link_libraries(myBlockChain include/rpclib/librpc.so)
#target_link_libraries(myBlockChain ${CMAKE_BINARY_DIR}/include/rpclib/librpc.a)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(myBlockChain-peer-star Threads::Threads)
target_link_libraries(myBlockChain-peer-2dmesh Threads::Threads)
