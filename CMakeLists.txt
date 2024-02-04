cmake_minimum_required(VERSION 3.28.1)
project(dictAttack VERSION 1
                   LANGUAGES CXX)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -Werror -pedantic")

find_package(OpenSSL REQUIRED)
find_package(Catch2 3 REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_subdirectory(loginMenu)
add_subdirectory(srcShared)
add_subdirectory(tests)
add_subdirectory(pwCracker)
