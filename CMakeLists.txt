cmake_minimum_required(VERSION 3.28.1)
project(dictAttack VERSION 1
                   LANGUAGES CXX)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic")

add_subdirectory(loginMenu)
add_library(sharedComponents
  srcShared/io.h srcShared/io.cpp
  srcShared/io_file.h srcShared/io_file.cpp
  srcShared/openSSL_EVP.h srcShared/openSSL_EVP.cpp)
target_compile_features(sharedComponents PUBLIC cxx_std_23)

set(OPENSSL_USE_STATIC_LIBS true)
find_package(OpenSSL REQUIRED)

##Tests
find_package(Catch2 3 REQUIRED)
add_executable(tests 
  tests/evp_tests.cpp srcShared/openSSL_EVP.cpp
  tests/io_tests.cpp srcShared/io.cpp srcShared/io_file.cpp
  )
target_compile_features(tests PRIVATE cxx_std_23)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE OpenSSL::SSL)
target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}")
