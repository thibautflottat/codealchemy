#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/complex.h>
#include <nanobind/ndarray.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"

namespace nb = nanobind;

void rho_q(const nb::ndarray<double, nb::shape<-1, 3>>& x,
           const nb::ndarray<double, nb::shape<-1, 3>>& q,
           nb::ndarray<std::complex<double>, nb::shape<-1>>& rho) {
    size_t Nx = x.shape(0);
    size_t Nq = q.shape(0);

    #pragma omp parallel for 
    for (size_t i = 0; i < Nq; ++i) {
        std::complex<double> rho_value(0.0, 0.0);

        for (size_t j = 0; j < Nx; ++j) {
            double alpha = x(j, 0) * q(i, 0) + x(j, 1) * q(i, 1) + x(j, 2) * q(i, 2);
            rho_value += std::exp(std::complex<double>(0.0, alpha)); // exp(i*alpha)
        }

        rho(i) = rho_value;
    }
}

NB_MODULE(rho_q_module, m) {
    // Explicitly specify the function signature
    m.def("rho_q", [](const nb::ndarray<double, nb::shape<-1, 3>>& x,
                      const nb::ndarray<double, nb::shape<-1, 3>>& q,
                      nb::ndarray<std::complex<double>, nb::shape<-1>>& rho) {
        rho_q(x, q, rho);
    }, "A function to compute rho_q", nb::arg("x"), nb::arg("q"), nb::arg("rho"));
}