#ifndef RMSD_ANALYSIS_HPP
#define RMSD_ANALYSIS_HPP

#include "analysis.hpp"

class RMSDAnalysis : public Analysis {
public:
    // Updated to accept the frame number as a second parameter
    void analyze_frame(const chemfiles::Frame& frame, size_t frame_number) override;
};

#endif // RMSD_ANALYSIS_HPP