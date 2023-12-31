cmake_minimum_required(VERSION 3.20)

project(iu_server LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(LibUring 2.0 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(server "")
target_sources(server PRIVATE main.cpp)
add_subdirectory(io_uring)
add_subdirectory(thread_infra)

target_include_directories(server PRIVATE io_uring thread_infra)

find_package(TBB)
target_link_libraries(server TBB::tbb)
target_link_libraries(server uring)

file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)
