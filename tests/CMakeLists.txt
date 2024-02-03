cmake_minimum_required(VERSION 3.28.1)
add_executable(tests 
  evp_tests.cpp 
  io_tests.cpp  
  )
target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}")
target_compile_features(tests PRIVATE cxx_std_23)
target_link_libraries(tests PRIVATE sharedComponents)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE OpenSSL::SSL)
