gfortran -O3 -fopenmp -march=native -ffast-math -funroll-loops -pg -c pairwise_module.f90
gfortran -O3 -fopenmp -march=native -ffast-math -funroll-loops main.f90 pairwise_module.o -o pdists
# ifx -O3 -qopenmp -xHost -fp-model fast=2 -funroll-loops -qopt-report=5 -qopt-report-phase=vec -pg -c pairwise_module.f90
# ifx -O3 -qopenmp -xHost -fp-model fast=2 -funroll-loops -qopt-report=5 -qopt-report-phase=vec main.f90 pairwise_module.o -o pdists