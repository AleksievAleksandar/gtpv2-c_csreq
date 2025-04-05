cmake_minimum_required(VERSION 3.16)

project(gtp-c-csr VERSION 0.1.0 LANGUAGES CXX )

add_subdirectory(src/gtpv2_msg)

add_executable(gtp-c-csr src/main.cpp)
target_link_libraries(gtp-c-csr PUBLIC libgtpv2)