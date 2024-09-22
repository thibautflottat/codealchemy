# g++-14 read_trj.cpp -o read_trj -I/usr/local/include -lchemfiles -L/usr/local/lib -L/opt/homebrew/Cellar/open-mpi/5.0.3_1/lib -lmpi -I/opt/homebrew/Cellar/open-mpi/5.0.3_1/include


# Compile the individual cpp files
g++-14 -c mpi_handler.cpp -o mpi_handler.o -L/opt/homebrew/Cellar/open-mpi/5.0.3_1/lib -lmpi -I/opt/homebrew/Cellar/open-mpi/5.0.3_1/include
g++-14 -c trajectory_handler.cpp -o trajectory_handler.o -I/usr/local/include -lchemfiles -L/usr/local/lib
cd analysis
g++-14 -c perform_analysis.cpp -o perform_analysis.o -I/usr/local/include -lchemfiles -L/usr/local/lib
g++-14 -c rdf_analysis.cpp -o rdf_analysis.o -I/usr/local/include -lchemfiles -L/usr/local/lib
g++-14 -c msd_analysis.cpp -o msd_analysis.o -I/usr/local/include -lchemfiles -L/usr/local/lib
g++-14 -c rmsd_analysis.cpp -o rmsd_analysis.o -I/usr/local/include -lchemfiles -L/usr/local/lib -L/opt/homebrew/Cellar/open-mpi/5.0.3_1/lib -lmpi -I/opt/homebrew/Cellar/open-mpi/5.0.3_1/include
cd ..
g++-14 -c main.cpp -o main.o -I/usr/local/include -lchemfiles -L/usr/local/lib

# Link all the object files into an executable
g++-14 -o analysis_test main.o mpi_handler.o trajectory_handler.o analysis/perform_analysis.o analysis/rdf_analysis.o analysis/msd_analysis.o analysis/rmsd_analysis.o -I/usr/local/include -lchemfiles -L/usr/local/lib -L/opt/homebrew/Cellar/open-mpi/5.0.3_1/lib -lmpi -I/opt/homebr