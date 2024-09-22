#include <iostream>
#include <array>
#include <cmath>
#include <mdspan/mdspan.hpp>
#include <cassert>
#include <vector>

// Function to compute the inverse of a 3x3 matrix
std::array<std::array<double, 3>, 3> inverse(const std::array<std::array<double, 3>, 3>& mat) {
    std::array<std::array<double, 3>, 3> inv;
    double det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
               - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
               + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

    assert(det != 0 && "Matrix determinant is zero, inverse does not exist.");
    
    double invDet = 1.0 / det;

    inv[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) * invDet;
    inv[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * invDet;
    inv[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * invDet;

    inv[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * invDet;
    inv[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * invDet;
    inv[1][2] = (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) * invDet;

    inv[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) * invDet;
    inv[2][1] = (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) * invDet;
    inv[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) * invDet;

    return inv;
}

// Function to transpose a 3x3 matrix
std::array<std::array<double, 3>, 3> transpose(const std::array<std::array<double, 3>, 3>& mat) {
    std::array<std::array<double, 3>, 3> transposed;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            transposed[i][j] = mat[j][i];
    return transposed;
}

// Function to compute the norm of a 3D vector (row)
double norm(const std::array<double, 3>& vec) {
    return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// Comparator to sort based on q_distances
bool comparator(const std::pair<std::array<double, 3>, double>& a, const std::pair<std::array<double, 3>, double>& b) {
    return a.second < b.second;
}

void get_spherical_qpoints(const std::array<std::array<double, 3>, 3>& cell, double q_max) {
    // inv(A.T) == inv(A).T
    auto transposed_cell = transpose(cell);
    auto rec_cell = inverse(transposed_cell);

    // Multiply by 2 * pi
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rec_cell[i][j] *= 2 * M_PI;
        }
    }

    // Transpose the reciprocal cell
    auto transposed_rec_cell = transpose(rec_cell);

    // Compute inverse of the transposed reciprocal cell (inv_rec_cell)
    auto inv_rec_cell = inverse(transposed_rec_cell);

    // Compute h = 1 / norm(inv_rec_cell, axis=1)
    std::array<double, 3> h;
    for (int i = 0; i < 3; ++i) {
        double row_norm = norm(inv_rec_cell[i]);
        h[i] = 1.0 / row_norm;
    }

    // Compute N = ceil(q_max / h)
    std::array<int, 3> N;
    for (int i = 0; i < 3; ++i) {
        N[i] = static_cast<int>(std::ceil(q_max / h[i]));
    }

    // Create q-points in the first octant
    std::vector<std::array<int, 3>> lattice_points;
    for (int x = 0; x <= N[0]; ++x) {
        for (int y = 0; y <= N[1]; ++y) {
            for (int z = 0; z <= N[2]; ++z) {
                lattice_points.push_back({x, y, z});
            }
        }
    }

    // Perform q_points = lattice_points @ rec_cell (matrix multiplication)
    std::vector<std::array<double, 3>> q_points;
    for (const auto& point : lattice_points) {
        std::array<double, 3> q;
        for (int i = 0; i < 3; ++i) {
            q[i] = point[0] * rec_cell[i][0] + point[1] * rec_cell[i][1] + point[2] * rec_cell[i][2];
        }
        q_points.push_back(q);
    }

    // Calculate distances for pruning: q_distances = np.linalg.norm(q_points, axis=1)
    std::vector<double> q_distances;
    for (const auto& q : q_points) {
        q_distances.push_back(norm(q));
    }

    // Pair q_points and q_distances together for sorting
    std::vector<std::pair<std::array<double, 3>, double>> points_and_distances;
    for (size_t i = 0; i < q_points.size(); ++i) {
        points_and_distances.push_back({q_points[i], q_distances[i]});
    }

    // Sort points and distances based on q_distances
    std::sort(points_and_distances.begin(), points_and_distances.end(),
              [](const std::pair<const std::array<double, 3>, const double>& a, 
                 const std::pair<const std::array<double, 3>, const double>& b) {
                  return a.second < b.second;
              });

    // Unpack sorted points and distances
    q_points.clear();
    q_distances.clear();
    for (const auto& pair : points_and_distances) {
        q_points.push_back(pair.first);
        q_distances.push_back(pair.second);
    }

    // Prune based on distances: q_points = q_points[q_distances <= q_max]
    std::vector<std::array<double, 3>> pruned_q_points;
    std::vector<double> pruned_q_distances;

    for (size_t i = 0; i < q_distances.size(); ++i) {
        if (q_distances[i] <= q_max) {
            pruned_q_points.push_back(q_points[i]);
            pruned_q_distances.push_back(q_distances[i]);
        }
    }

    // Print pruned q_points and q_distances
    std::cout << "Pruned q_points (where q_distances <= q_max):" << std::endl;
    for (const auto& q : pruned_q_points) {
        std::cout << "(" << q[0] << ", " << q[1] << ", " << q[2] << ") ";
    }
    std::cout << std::endl;

    std::cout << "Pruned q_distances:" << std::endl;
    for (const auto& dist : pruned_q_distances) {
        std::cout << dist << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::array<std::array<double, 3>, 3> cell = {{{10, 0, 0}, {0, 10, 0}, {0, 0, 10}}};
    double q_max = 2.0;

    get_spherical_qpoints(cell, q_max);

    return 0;
}