gcc-14 -o rho_q_inline rho_q_inline.c -Ofast -march=native -ffast-math -funroll-loops -fopenmp -lm
gcc-14 -o rho_q rho_q.c -Ofast -march=native -ffast-math -funroll-loops -fopenmp -lm
gcc-14 -o rho_q_avx rho_q_avx256.c -Ofast -march=native -ffast-math -funroll-loops -fopenmp -lm -mavx2