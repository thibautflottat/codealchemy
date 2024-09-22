#ifndef QPOINTS_HPP
#define QPOINTS_HPP

#include <Eigen/Dense>
#include <vector>

double get_prune_distance(int max_points, double q_max, double q_vol);
double calculate_solid_angle(const Eigen::Matrix3d &cell);
Eigen::MatrixXd get_spherical_qpoints(const Eigen::Matrix3d &cell, double q_max, int max_points = -1, int seed = 42);

#endif // QPOINTS_HPP