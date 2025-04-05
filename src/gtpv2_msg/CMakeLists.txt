add_library(libgtpv2 STATIC gtpv2.cpp)
target_include_directories(libgtpv2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(libgtpv2 PRIVATE -Wall -Wextra -pedantic -Werror)
