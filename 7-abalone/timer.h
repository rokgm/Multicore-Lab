#pragma once

#include <chrono>

class Timer
{
public:
    Timer(std::chrono::milliseconds timeLimit);
    void resetStartTime(std::chrono::milliseconds timeLimit = std::chrono::milliseconds(300));
    bool timeUp() const;

private:
    std::chrono::milliseconds m_timeLimit;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};