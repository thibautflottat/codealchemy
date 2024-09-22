cmake_minimum_required(VERSION 3.18)
project(RDFAnalysis LANGUAGES CXX CUDA)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CUDA settings (optional)
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 14)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

# Find CUDA (this ensures you have CUDA or alternatives like HIP)
find_package(CUDA REQUIRED)

# Add include directories
include_directories(include)

# Define source files for CPU and GPU
set(CPU_SOURCES
    src/rdf_cpu.cpp
    src/main.cpp
)

set(GPU_SOURCES
    src/rdf_gpu.cu
)

# Create executable and link CPU sources
add_executable(rdf_cpu ${CPU_SOURCES})

# Create executable for GPU and link GPU sources
add_executable(rdf_gpu ${CPU_SOURCES} ${GPU_SOURCES})

# Link CUDA libraries to GPU executable
target_link_libraries(rdf_gpu CUDA::cudart)

# Optional: set GPU architecture (use appropriate one for your GPU)
set_target_properties(rdf_gpu PROPERTIES CUDA_ARCHITECTURES 75)

# Install the executables
install(TARGETS rdf_cpu rdf_gpu DESTINATION bin)