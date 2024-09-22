#include <cmath>
#include <vector>
#include <array>
#include <cassert>
#include <iostream>

// Function to solve the cubic equation for pruning distance in q-space
double get_prune_distance(int max_points, double q_max, double q_vol) {
    double Q = q_max;
    double V = q_vol;
    int N = max_points;

    // Coefficients for the cubic equation
    double a = 1.0;
    double b = -3.0 / 2.0 * Q;
    double c = 0.0;
    double d = 3.0 / 2.0 * V * N / (4.0 * M_PI);

    // Function to represent the original equation
    auto original_eq = [&](double x) {
        return a * x * x * x + b * x * x + c * x + d;
    };

    // Calculate discriminant
    double p = (3 * a * c - b * b) / (3 * a * a);
    double q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);

    // Discriminant term
    double D_t = -(4 * p * p * p + 27 * q * q);

    // If discriminant is negative, return max q value
    if (D_t < 0) {
        return q_max;
    }

    // Solve the cubic equation
    double x = Q * (std::cos(1.0 / 3.0 * std::acos(1.0 - 4.0 * d / (Q * Q * Q)) - 2.0 * M_PI / 3.0) + 0.5);

    // Assert that the solution is correct (debugging purpose)
    assert(std::abs(original_eq(x)) < 1e-6);

    return x;
}


// Function to calculate the solid angle subtended by three vectors forming a tetrahedron
double calculate_solid_angle(const std::array<std::array<double, 3>, 3>& cell) {
    // Ensure the determinant is positive
    double det = cell[0][0] * (cell[1][1] * cell[2][2] - cell[2][1] * cell[1][2])
                 - cell[0][1] * (cell[1][0] * cell[2][2] - cell[1][2] * cell[2][0])
                 + cell[0][2] * (cell[1][0] * cell[2][1] - cell[2][0] * cell[1][1]);
    assert(det > 0);

    // Cell vectors
    const auto& A = cell[0];
    const auto& B = cell[1];
    const auto& C = cell[2];

    // Vector lengths (norms)
    double a = std::sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
    double b = std::sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);
    double c = std::sqrt(C[0] * C[0] + C[1] * C[1] + C[2] * C[2]);

    // Angles between the vectors
    double ac = std::acos((A[0] * B[0] + A[1] * B[1] + A[2] * B[2]) / (a * b));  // angle a-o-b
    double ab = std::acos((A[0] * C[0] + A[1] * C[1] + A[2] * C[2]) / (a * c));  // angle b-o-c
    double aa = std::acos((B[0] * C[0] + B[1] * C[1] + B[2] * C[2]) / (b * c));  // angle b-o-c

    // Use the spherical excess formula
    double s = (aa + ab + ac) / 2.0;
    double tan_sigma_over_4 = std::sqrt(std::tan(s / 2.0) * std::tan((s - aa) / 2.0) * 
                                        std::tan((s - ab) / 2.0) * std::tan((s - ac) / 2.0));

    // Solid angle
    double sigma = 4.0 * std::atan(tan_sigma_over_4);

    return sigma;
}