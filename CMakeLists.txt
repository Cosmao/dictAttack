cmake_minimum_required(VERSION 3.28.1)
project(dictAttack)
include_directories(${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 23)
set(OPENSSL_USE_STATIC_LIBS true)
find_package(OpenSSL REQUIRED)

file(GLOB src_files CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${src_files}) 
list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::applink)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.o) 
