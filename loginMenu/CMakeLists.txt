cmake_minimum_required(VERSION 3.28.1)
add_executable(loginProgram 
  main.cpp
  userMenu.cpp
  )
find_package(OpenSSL REQUIRED)
target_include_directories(loginProgram PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(loginProgram PUBLIC cxx_std_23)
target_link_libraries(loginProgram PRIVATE OpenSSL::SSL)
target_link_libraries(loginProgram PRIVATE sharedComponents)
