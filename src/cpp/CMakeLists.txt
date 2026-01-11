cmake_minimum_required(VERSION 3.10)

project(NanoBrainKernel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Assume ggml is available in a 'ggml' subdirectory or installed
# If ggml source is in local folder:
# include_directories(ggml/include)
# add_subdirectory(ggml)

# For now, we assume the user has ggml library available.
# Set GGML_PATH to the root of llama.cpp or ggml repo
if(NOT DEFINED GGML_PATH)
    message(STATUS "GGML_PATH not defined, assuming ./ggml")
    set(GGML_PATH "./ggml")
endif()

include_directories(${GGML_PATH}/include)
include_directories(${GGML_PATH}) # sometimes ggml.h is in root

# Add the kernel library
add_library(nanobrain_kernel nanobrain_kernel.cpp)

# Link ggml - this assumes libggml.a or similar exists or is built
# target_link_libraries(nanobrain_kernel ggml) 

# Example executable
add_executable(nanobrain_test main.cpp)
target_link_libraries(nanobrain_test nanobrain_kernel ggml)
