#include "qpoints.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <cassert>

using namespace Eigen;
using namespace std;

// Function to calculate the prune distance
double get_prune_distance(
    int max_points,
    double q_max,
    double q_vol) {

    double Q = q_max;
    double V = q_vol;
    int N = max_points;

    // Coefficients for cubic equation
    double a = 1.0;
    double b = -3.0 / 2.0 * Q;
    double c = 0.0;
    double d = 3.0 / 2.0 * V * N / (4.0 * M_PI);

    // Define the original equation
    auto original_eq = [&](double x) {
        return a * x * x * x + b * x * x + c * x + d;
    };

    // Discriminant
    double p = (3 * a * c - b * b) / (3 * a * a);
    double q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);

    double D_t = -(4 * p * p * p + 27 * q * q);
    if (D_t < 0) {
        return q_max;
    }

    double x = Q * (cos(1.0 / 3.0 * acos(1 - 4 * d / pow(Q, 3)) - 2 * M_PI / 3.0) + 0.5);

    assert(abs(original_eq(x)) < 1e-6 && "Equation did not converge properly.");

    return x;
}

// Function to calculate solid angle
double calculate_solid_angle(const Matrix3d &cell) {
    assert(cell.determinant() > 0 && "Determinant must be positive");

    Vector3d A = cell.row(0);
    Vector3d B = cell.row(1);
    Vector3d C = cell.row(2);

    double a = A.norm();
    double b = B.norm();
    double c = C.norm();

    double ac = acos(A.dot(B) / (a * b));
    double ab = acos(A.dot(C) / (a * c));
    double aa = acos(B.dot(C) / (b * c));

    double s = (aa + ab + ac) / 2.0;
    double tan_sigma_over_4 = sqrt(tan(s / 2) * tan((s - aa) / 2) * tan((s - ab) / 2) * tan((s - ac) / 2));
    double sigma = atan(tan_sigma_over_4) * 4.0;

    return sigma;
}

// Function to get spherical q-points
MatrixXd get_spherical_qpoints(
    const Matrix3d &cell,
    double q_max,
    int max_points,
    int seed) {

    Matrix3d rec_cell = cell.inverse().transpose() * 2 * M_PI;
    Matrix3d inv_rec_cell = rec_cell.inverse().transpose();
    Vector3d h = inv_rec_cell.rowwise().norm().cwiseInverse();
    Vector3i N = (q_max / h.array()).ceil().cast<int>();  // Corrected to use element-wise division

    // Create lattice points
    vector<Vector3i> lattice_points;
    for (int i = 0; i <= N(0); ++i) {
        for (int j = 0; j <= N(1); ++j) {
            for (int k = 0; k <= N(2); ++k) {
                lattice_points.emplace_back(Vector3i(i, j, k));
            }
        }
    }

    MatrixXd q_points(lattice_points.size(), 3);
    for (size_t i = 0; i < lattice_points.size(); ++i) {
        q_points.row(i) = lattice_points[i].cast<double>().transpose() * rec_cell;
    }

    VectorXd q_distances = q_points.rowwise().norm();

    // Sort distances and q-points
    vector<int> indices(q_points.rows());
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) {
        return q_distances(i) < q_distances(j);
    });

    MatrixXd sorted_q_points(indices.size(), 3);
    for (size_t i = 0; i < indices.size(); ++i) {
        sorted_q_points.row(i) = q_points.row(indices[i]);
    }

    q_points = sorted_q_points;
    q_distances = q_distances(indices).eval();

    vector<int> keep;
    for (int i = 0; i < q_points.rows(); ++i) {
        if (q_distances(i) <= q_max) {
            keep.push_back(i);
        }
    }

    q_points = q_points(keep, Eigen::all).eval();
    q_distances = q_distances(keep).eval();

    // Pruning based on max_points
    if (max_points > 0 && max_points < q_points.rows()) {
        double solid_angle = calculate_solid_angle(rec_cell);
        double angle_factor = solid_angle / (4 * M_PI);
        double q_vol = rec_cell.determinant();

        double q_prune = get_prune_distance(max_points / angle_factor, q_max, q_vol);

        if (q_prune < q_max) {
            cout << "Pruning q-points from the range " << q_prune << " < |q| < " << q_max << endl;

            VectorXd p = VectorXd::Ones(q_points.rows());
            p.tail(q_points.rows() - 1) = (q_prune / q_distances.tail(q_distances.rows() - 1).array()).square();  // Corrected to use element-wise division

            mt19937 gen(seed);
            uniform_real_distribution<double> dis(0.0, 1.0);

            vector<int> final_keep;
            for (int i = 0; i < p.size(); ++i) {
                if (dis(gen) <= p(i)) {
                    final_keep.push_back(i);
                }
            }

            q_points = q_points(final_keep, Eigen::all).eval();
            cout << "Pruned from " << q_distances.size() << " q-points to " << q_points.rows() << endl;
        }
    }

    return q_points;
}