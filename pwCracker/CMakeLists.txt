cmake_minimum_required(VERSION 3.28.1)
add_executable(pwCracker 
  main.cpp
  )
target_include_directories(pwCracker PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(pwCracker PUBLIC cxx_std_23)
target_link_libraries(pwCracker PRIVATE OpenSSL::SSL)
target_link_libraries(pwCracker PRIVATE sharedComponents)
target_link_libraries(pwCracker PRIVATE Qt6::Core)
target_link_libraries(pwCracker PRIVATE Qt6::Widgets)
