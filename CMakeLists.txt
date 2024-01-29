cmake_minimum_required(VERSION 3.28.1)
project(dictAttack)
include_directories(${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 23)
set(OPENSSL_USE_STATIC_LIBS true)
find_package(OpenSSL REQUIRED)

set(SOURCE_FILES 
  src/main.cpp
  src/io.cpp
  src/io_file.cpp
  src/openSSL_EVP.cpp
  src/userMenu.cpp)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if(CMAKE_CXX_CPPCHECK)
  list(APPEND CMAKE_CXX_CPPCHECK
    "--dump"
    "--quiet"
    "--enable=all"
    "--suppress=missingIncludeSystem"
    "--project=compile_commands.json"
    )
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES}) 
list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::applink)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)
# target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.o) 
