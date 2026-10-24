#include "terra/core/timer.h"

namespace terra {

Timer::Timer()
    : m_last(Clock::now())
    , m_deltaTime(0.0f)
    , m_totalTime(0.0f) {}

float Timer::tick() {
    auto now = Clock::now();
    m_deltaTime = Seconds(now - m_last).count();
    m_totalTime += m_deltaTime;
    m_last = now;
    return m_deltaTime;
}

} // namespace terra
