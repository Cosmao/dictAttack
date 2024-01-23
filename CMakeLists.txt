cmake_minimum_required(VERSION 3.28.1)
project(dictAttack)
include_directories(${PROJECT_SOURCE_DIR})
set(OPENSSL_USE_STATIC_LIBS true)
find_package(OpenSSL REQUIRED)
set(SOURCE_FILES 
  src/main.cpp
  src/io_file.cpp
  # src/myMD5.cpp
  src/openSSL_MD5.cpp
  src/userMenu.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES}) 
set(CMAKE_CXX_STANDARD 23)
list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic -Wno-deprecated-declarations")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::applink)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.o) 
