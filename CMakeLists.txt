cmake_minimum_required(VERSION 3.28.1)
project(dictAttack VERSION 1
                   LANGUAGES CXX)

file(GLOB src_files CONFIGURE_DEPENDS "src/*.cpp")
add_executable(loginProgram ${src_files})
target_include_directories(loginProgram PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(loginProgram PUBLIC cxx_std_23)

set(OPENSSL_USE_STATIC_LIBS true)
find_package(OpenSSL REQUIRED)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic")
target_link_libraries(loginProgram PRIVATE OpenSSL::applink)
target_link_libraries(loginProgram PRIVATE OpenSSL::SSL)
set_target_properties(loginProgram PROPERTIES OUTPUT_NAME loginProgram.o) 
