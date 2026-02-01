# CMake configuration for Python bindings
# Can be used instead of setup.py for more complex builds

cmake_minimum_required(VERSION 3.16)
project(nanobrain_python)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python and pybind11
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)

if(NOT pybind11_FOUND)
    # Fallback to fetching pybind11
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.11.1
    )
    FetchContent_MakeAvailable(pybind11)
endif()

# GGML path
set(GGML_PATH "" CACHE PATH "Path to ggml/llama.cpp")
if(GGML_PATH)
    include_directories(${GGML_PATH}/include ${GGML_PATH})
    link_directories(${GGML_PATH}/build)
endif()

# NanoBrain C++ sources
set(CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/cpp)
include_directories(${CPP_DIR})

# Find all C++ source files
file(GLOB NANOBRAIN_SOURCES ${CPP_DIR}/*.cpp)

# Create the Python module
pybind11_add_module(_core 
    bindings.cpp
    ${NANOBRAIN_SOURCES}
)

# Link ggml
target_link_libraries(_core PRIVATE ggml)

# Set output directory
set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/nanobrain
)

# Install target
install(TARGETS _core
    LIBRARY DESTINATION nanobrain
)
