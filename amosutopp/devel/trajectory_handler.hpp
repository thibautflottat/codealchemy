#ifndef TRAJECTORY_HANDLER_HPP
#define TRAJECTORY_HANDLER_HPP

#include <chemfiles.hpp>

class TrajectoryHandler {
public:
    TrajectoryHandler(const std::string& trajectory_file, const std::string& topology_file);

    size_t get_n_frames() const;
    chemfiles::Frame read_frame(size_t i);

private:
    chemfiles::Trajectory trajectory;
    size_t n_frames;
};

#endif // TRAJECTORY_HANDLER_HPP