cmake_minimum_required(VERSION 3.28.1)
project(dictAttack VERSION 1
                   LANGUAGES CXX)

file(GLOB src_files CONFIGURE_DEPENDS "src/*.cpp")
add_executable(loginProgram ${src_files})
target_include_directories(loginProgram PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(loginProgram PUBLIC cxx_std_23)
set_target_properties(loginProgram PROPERTIES OUTPUT_NAME loginProgram.o) 
list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic")

set(OPENSSL_USE_STATIC_LIBS true)
find_package(OpenSSL REQUIRED)
target_link_libraries(loginProgram PRIVATE OpenSSL::applink)
target_link_libraries(loginProgram PRIVATE OpenSSL::SSL)

##Tests
find_package(Catch2 3 REQUIRED)
add_executable(tests 
  tests/evp_tests.cpp src/openSSL_EVP.cpp
  tests/io_tests.cpp src/io.cpp src/io_file.cpp
  )
target_compile_features(tests PRIVATE cxx_std_23)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE OpenSSL::SSL)
target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}")
