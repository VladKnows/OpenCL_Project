cmake_minimum_required(VERSION 3.10)
project(OpenCLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(opencl_app ${SRC_FILES})

target_include_directories(opencl_app PRIVATE ${OpenCL_INCLUDE_DIRS})
target_link_libraries(opencl_app PRIVATE ${OpenCL_LIBRARIES})
