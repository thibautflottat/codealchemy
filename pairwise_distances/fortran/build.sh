gfortran -O3 -fopenmp -march=native -ffast-math -funroll-loops -c pairwise_module.f90
gfortran -O3 -fopenmp -march=native -ffast-math -funroll-loops main.f90 pairwise_module.o -o pdists
