cmake_minimum_required(VERSION 3.28.1)
project(dictAttack)
include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME} src/main.cpp
  src/io.cpp
  src/md5.cpp
  src/userMenu.cpp)
set(CMAKE_CXX_STANDARD 23)
list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.o) 
