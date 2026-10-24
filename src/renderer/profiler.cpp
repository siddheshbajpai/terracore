#include "terra/renderer/profiler.h"

#include <chrono>

namespace terra {

Profiler::Profiler() {}

int Profiler::beginSample(const std::string& name) {
    ActiveSample s;
    s.name = name;
    s.startSeconds =
        std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    m_active.push_back(s);
    return (int)m_active.size() - 1;
}

void Profiler::endSample(int handle) {
    if (handle < 0 || handle >= (int)m_active.size()) return;
    double endSeconds =
        std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    Sample sample;
    sample.name = m_active[handle].name;
    sample.milliseconds =
        (float)((endSeconds - m_active[handle].startSeconds) * 1000.0);
    m_samples.push_back(sample);
    m_active.erase(m_active.begin() + handle);
}

void Profiler::endFrame() { m_samples.clear(); }

void Profiler::clear() { m_active.clear(); m_samples.clear(); }

} // namespace terra
