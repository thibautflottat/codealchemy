cmake_minimum_required(VERSION 3.10)
project(MdspanExample)

set(CMAKE_CXX_STANDARD 17)

find_package(mdspan REQUIRED)

add_executable(qpoints main.cpp)
target_link_libraries(qpoints std::mdspan)
