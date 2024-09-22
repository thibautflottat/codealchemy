#ifndef PERFORM_ANALYSIS_HPP
#define PERFORM_ANALYSIS_HPP

#include "../mpi_handler.hpp"
#include "../trajectory_handler.hpp"
#include "analysis.hpp"

void perform_analysis(Analysis* analysis, TrajectoryHandler& trajectory_handler, MPIHandler& mpi_handler);

#endif // PERFORM_ANALYSIS_HPP