#include "trajectory_handler.hpp"

TrajectoryHandler::TrajectoryHandler(const std::string& trajectory_file, const std::string& topology_file)
    : trajectory(trajectory_file) {
    trajectory.set_topology(topology_file, "LAMMPS Data");
    n_frames = trajectory.nsteps();
}

size_t TrajectoryHandler::get_n_frames() const {
    return n_frames;
}

chemfiles::Frame TrajectoryHandler::read_frame(size_t i) {
    return trajectory.read_step(i);
}