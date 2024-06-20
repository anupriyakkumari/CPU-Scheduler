cmake_minimum_required(VERSION 3.10)
project(cpu_scheduling)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpu_scheduling main.cpp)

target_include_directories(cpu_scheduling PRIVATE ../thirdparty /usr/local/Cellar/asio/1.30.2/include)

target_link_libraries(cpu_scheduling PRIVATE pthread)
