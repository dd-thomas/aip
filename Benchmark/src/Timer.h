#pragma once

#include <chrono>

namespace NBenchmark
{

class CTimer
{
    public:
        CTimer();

        ~CTimer();

    private:
        std::chrono::high_resolution_clock::time_point m_startTime;
};

} // namespace NBenchmark