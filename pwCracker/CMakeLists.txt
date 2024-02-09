cmake_minimum_required(VERSION 3.28.1)

add_executable(pwCrackerGUI 
  mainGUI.cpp
  )
target_include_directories(pwCrackerGUI PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(pwCrackerGUI PUBLIC cxx_std_23)
target_link_libraries(pwCrackerGUI PRIVATE OpenSSL::SSL)
target_link_libraries(pwCrackerGUI PRIVATE sharedComponents)
target_link_libraries(pwCrackerGUI PRIVATE Qt6::Core)
target_link_libraries(pwCrackerGUI PRIVATE Qt6::Widgets)

add_executable(pwCrackerCLI
  mainCLI.cpp
  cracker.cpp
  argcHandler.cpp
  )
target_include_directories(pwCrackerCLI PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(pwCrackerCLI PUBLIC cxx_std_23)
target_link_libraries(pwCrackerCLI PRIVATE OpenSSL::SSL)
target_link_libraries(pwCrackerCLI PRIVATE sharedComponents)
