#include "Timer.h"

#include <iostream>

namespace NBenchmark
{

CTimer::CTimer()
{
    m_startTime = std::chrono::high_resolution_clock::now();
}

CTimer::~CTimer()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime).count();
    std::cout << "Duration: " << duration << " microseconds" << std::endl;
}

} // namespace NBenchmark