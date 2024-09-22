#ifndef RDF_ANALYSIS_HPP
#define RDF_ANALYSIS_HPP

#include "analysis.hpp"

class RDFAnalysis : public Analysis {
public:
    void analyze_frame(const chemfiles::Frame& frame, size_t frame_number) override;
};

#endif // RDF_ANALYSIS_HPP