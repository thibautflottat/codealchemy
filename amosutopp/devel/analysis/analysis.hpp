#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include <chemfiles.hpp>

class Analysis {
public:
    // Update the analyze_frame function to accept both frame and frame_number
    virtual void analyze_frame(const chemfiles::Frame& frame, size_t frame_number) = 0;
    virtual ~Analysis() = default;
};

#endif // ANALYSIS_HPP