#pragma once

#include <chrono>

namespace terra {

// High-resolution frame timer for profiling and delta-time computation.
class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using Seconds = std::chrono::duration<float>;

    Timer();

    // Mark the start of a new frame, returns delta time in seconds since
    // the previous tick.
    float tick();

    float deltaTime() const { return m_deltaTime; }
    float totalTime() const { return m_totalTime; }

private:
    Clock::time_point m_last;
    float m_deltaTime;
    float m_totalTime;
};

} // namespace terra
