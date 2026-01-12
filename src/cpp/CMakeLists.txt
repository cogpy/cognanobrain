cmake_minimum_required(VERSION 3.10)

project(NanoBrainKernel VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# ================================================================
# GGML / llama.cpp Configuration
# ================================================================

# Set GGML_PATH to the root of llama.cpp or ggml repo
if(NOT DEFINED GGML_PATH)
    message(STATUS "GGML_PATH not defined, assuming ./ggml")
    set(GGML_PATH "./ggml")
endif()

# Check for llama.cpp structure (ggml under llama.cpp)
if(EXISTS "${GGML_PATH}/ggml/include")
    # llama.cpp style (ggml as submodule)
    set(GGML_INCLUDE_DIRS 
        ${GGML_PATH}/ggml/include
        ${GGML_PATH}/ggml/src
        ${GGML_PATH}/include
    )
    set(GGML_LIB_NAME ggml)
elseif(EXISTS "${GGML_PATH}/include")
    # Standalone ggml
    set(GGML_INCLUDE_DIRS ${GGML_PATH}/include ${GGML_PATH})
    set(GGML_LIB_NAME ggml)
else()
    # Fallback: assume headers in root
    set(GGML_INCLUDE_DIRS ${GGML_PATH})
    set(GGML_LIB_NAME ggml)
endif()

include_directories(${GGML_INCLUDE_DIRS})
message(STATUS "GGML Include Dirs: ${GGML_INCLUDE_DIRS}")

# ================================================================
# NanoBrain Library Sources
# ================================================================
# Add the kernel library with all NanoBrain modules
add_library(nanobrain_kernel STATIC
    nanobrain_kernel.cpp
    nanobrain_encoder.cpp
    nanobrain_encoder_full.cpp
    nanobrain_reasoning.cpp
    nanobrain_attention.cpp
    nanobrain_time_crystal.cpp
    nanobrain_metacognitive.cpp
    nanobrain_unified.cpp
    nanobrain_atomese.cpp
    nanobrain_persistence.cpp
    nanobrain_serialization.cpp
    nanobrain_llm_bridge.cpp
)

set(NANOBRAIN_HEADERS
    nanobrain_kernel.h
    nanobrain_encoder.h
    nanobrain_encoder_full.h
    nanobrain_types.h
    nanobrain_time_crystal.h
    nanobrain_reasoning.h
    nanobrain_attention.h
    nanobrain_metacognitive.h
    nanobrain_unified.h
    nanobrain_persistence.h
    nanobrain_serialization.h
    nanobrain_llm_bridge.h
    nanobrain_atomese.h
)

# ================================================================
# Build Library Configuration
# ================================================================

target_include_directories(nanobrain_kernel PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GGML_INCLUDE_DIRS}
)

# Optional: Link against ggml if building as part of llama.cpp
# target_link_libraries(nanobrain_kernel PUBLIC ${GGML_LIB_NAME})

# ================================================================
# Test Executable
# ================================================================

add_executable(nanobrain_test main.cpp)
target_link_libraries(nanobrain_test nanobrain_kernel ${GGML_LIB_NAME})

# ================================================================
# Time Crystal Demo Executable
# ================================================================

add_executable(time_crystal_demo time_crystal_demo.cpp)
target_link_libraries(time_crystal_demo nanobrain_kernel ${GGML_LIB_NAME})

# ================================================================
# Unified Kernel Demo Executable
# ================================================================

add_executable(unified_demo unified_demo.cpp)
target_link_libraries(unified_demo nanobrain_kernel ${GGML_LIB_NAME})

# ================================================================
# Compiler Warnings and Optimizations
# ================================================================

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(nanobrain_kernel PRIVATE 
        -Wall -Wextra -Wpedantic -Wno-unused-parameter
    )
    
    # Release optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(nanobrain_kernel PRIVATE -O3 -march=native)
    endif()
elseif(MSVC)
    target_compile_options(nanobrain_kernel PRIVATE 
        /W4 /permissive- /wd4100
    )
    
    # Release optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(nanobrain_kernel PRIVATE /O2)
    endif()
endif()

# ================================================================
# Installation Rules
# ================================================================

install(TARGETS nanobrain_kernel
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${NANOBRAIN_HEADERS}
    DESTINATION include/nanobrain
)

# ================================================================
# Summary
# ================================================================

message(STATUS "")
message(STATUS "NanoBrain Kernel Configuration:")
message(STATUS "  - C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  - GGML Path: ${GGML_PATH}")
message(STATUS "  - Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
