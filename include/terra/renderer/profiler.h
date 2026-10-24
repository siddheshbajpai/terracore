#pragma once

#include <string>
#include <vector>

namespace terra {

// Frame-time profiler. Records named timing buckets and exposes the most
// recent frame measurements. Backed by std::chrono (CPU side); GPU timestamp
// queries can be added in a later milestone.
class Profiler {
public:
    struct Sample {
        std::string name;
        float milliseconds;
    };

    Profiler();

    // Begin a named sample. Returns a handle used to end it.
    int beginSample(const std::string& name);

    void endSample(int handle);

    // Call at the end of each frame to finalize samples.
    void endFrame();

    const std::vector<Sample>& samples() const { return m_samples; }

    void clear();

private:
    struct ActiveSample {
        std::string name;
        double startSeconds;
    };
    std::vector<ActiveSample> m_active;
    std::vector<Sample> m_samples;
};

} // namespace terra
