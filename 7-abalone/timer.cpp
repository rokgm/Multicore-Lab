#include "timer.h"

Timer::Timer(std::chrono::milliseconds timeLimit)
    : m_timeLimit(timeLimit), m_startTime(std::chrono::high_resolution_clock::now())
{
}

bool Timer::timeUp() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - m_startTime) >= m_timeLimit;
}

void Timer::resetStartTime(std::chrono::milliseconds timeLimit)
{
    m_timeLimit = timeLimit;
    m_startTime = std::chrono::high_resolution_clock::now();
}