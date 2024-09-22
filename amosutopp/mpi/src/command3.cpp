#include "command3.hpp"
#include <iostream>
#include "qpoints.hpp"

Command3::Command3(CLI::App& app) : extra_arg3{} {
    parse_common_args(app);  // Parse common arguments
    // app.add_option("--extra3", extra_arg3, "Extra argument 1 for Command2")->required();
}

void Command3::execute() {
    // Initialize MPI
    // MPI_Init(nullptr, nullptr);
    // MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    init_universe();  // Initialize Chemfiles trajectory and topology

    size_t n_frames = trajectory->nsteps();
    auto box_vol = trajectory->read().cell().volume();
    float side_length = 1.0; //pow(box_vol,(1.0/3));

    Eigen::Matrix3d cell;
    cell << 10, 0, 0,
            0, 10, 0,
            0, 0, 10;

    double q_max = 2.0;
    int max_points = 1000;

    Eigen::MatrixXd qpoints = get_spherical_qpoints(cell, q_max, max_points);

    std::cout << "Generated q-points:\n";
    for (int i = 0; i < qpoints.rows(); ++i) {
        const Eigen::Vector3d point = qpoints.row(i);
        std::cout << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")\n";
    }

    // Output the q-points to console
    // std::cout << "Generated q-points:\n";
    // for (const auto& point : qpoints) {
    //     std::cout << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")\n";
    // }

    // for (size_t frame_index = 0; frame_index < n_frames; ++frame_index) {
    //     chemfiles::Frame frame = trajectory->read_step(frame_index);

    // }
}