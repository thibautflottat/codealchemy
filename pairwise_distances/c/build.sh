gcc -c -O3 -fopenmp -march=native -ffast-math -funroll-loops pdists.c -o pdists.o
gcc -c -O3 -fopenmp -march=native -ffast-math -funroll-loops main.c -o main.o
gcc -O3 -fopenmp -march=native -ffast-math -funroll-loops pdists.o main.o -o pdists