# Compiler
CXX = g++-14

# Compiler flags
CXXFLAGS = -Wall -O2

# Include directories
INCLUDES = -I/usr/local/include \
           -I/opt/homebrew/Cellar/open-mpi/5.0.3_1/include

# Library directories
LDFLAGS = -L/usr/local/lib \
          -L/opt/homebrew/Cellar/open-mpi/5.0.3_1/lib

# Linked libraries
LIBS = -lchemfiles -lmpi

# Source files
SRC = main.cpp mpi_handler.cpp trajectory_handler.cpp perform_analysis.cpp rdf_analysis.cpp msd_analysis.cpp rmsd_analysis.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output executable
EXEC = analysis

# Default target
all: $(EXEC)

# Rule to link the object files into the final executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS) $(LIBS)

# Rule to compile the .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(EXEC)