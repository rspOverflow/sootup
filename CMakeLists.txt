cmake_minimum_required(VERSION 4.2.3)
project(sootup)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(CURL REQUIRED)
find_package(libzip REQUIRED)


add_library(soot STATIC
  sootup/args/args.cc
  sootup/app/app.cc
  sootup/commands/install/install.cc
  sootup/commands/uninstall/uninstall.cc
  sootup/commands/list/list.cc
)
target_include_directories(soot PRIVATE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  $<INSTALL_INTERFACE:.>
)
target_link_libraries(soot PRIVATE
  CURL::libcurl
  libzip::zip
)


add_executable(${CMAKE_PROJECT_NAME} 
  sootup/main.cc
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  $<INSTALL_INTERFACE:.>
)
target_link_libraries(${CMAKE_PROJECT_NAME}
  soot
)
