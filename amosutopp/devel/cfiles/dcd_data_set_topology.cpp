#include <chrono>
#include <iostream>
#include <chemfiles.hpp>

using namespace chemfiles;

int main() {
    size_t n_iters = 1;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t n = 0; n < n_iters; n++) {
        auto file = Trajectory("static.dcd");
        file.set_topology("structure.data", "LAMMPS Data");

        for (size_t i = 0; i < file.nsteps(); i++) {
            auto frame = file.read();
        }
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    std::cout << "Average time to read: " << duration_ms / n_iters / 1e3 << " ms" << std::endl;

    return 0;
}