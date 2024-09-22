#ifndef MSD_ANALYSIS_HPP
#define MSD_ANALYSIS_HPP

#include "analysis.hpp"

class MSDAnalysis : public Analysis {
public:
    void analyze_frame(const chemfiles::Frame& frame, size_t frame_number) override;
};

#endif // MSD_ANALYSIS_HPP