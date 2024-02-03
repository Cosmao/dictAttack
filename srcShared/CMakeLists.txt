cmake_minimum_required(VERSION 3.28.1)
add_library(sharedComponents
   io.cpp
   io_file.cpp
   openSSL_EVP.cpp)
target_compile_features(sharedComponents PUBLIC cxx_std_23)
