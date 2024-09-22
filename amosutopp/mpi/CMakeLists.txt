cmake_minimum_required(VERSION 3.10)
project(BabekApp LANGUAGES CXX)

# Set the C and C++ compilers
set(CMAKE_C_COMPILER "/opt/homebrew/bin/gcc-14")
set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-14")

# Find MPI package
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Find CLI11 and Chemfiles
find_package(CLI11 REQUIRED)
find_package(Chemfiles REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Set the C++ standard to 11
set(CMAKE_CXX_STANDARD 11)

# Add the source files
add_executable(babek
    main.cpp
    src/base.cpp
    src/command3.cpp
    src/qpoints.cpp
    # src/command1.cpp
    # src/command2.cpp
)

# Link the libraries
target_link_libraries(babek ${MPI_LIBRARIES} CLI11::CLI11 chemfiles Eigen3::Eigen)

# Specify the installation path for the executable
install(TARGETS babek DESTINATION /usr/local/bin)

# Optionally, if there are any headers to install (e.g., if you need to install the project as a library)
install(DIRECTORY src/ DESTINATION /usr/local/include/babek
        FILES_MATCHING PATTERN "*.hpp")

# If you have shared libraries, you can install them similarly:
# install(TARGETS mylibrary DESTINATION /usr/local/lib)