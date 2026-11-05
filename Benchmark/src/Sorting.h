#pragma once

#include <algorithm>
#include <utility>
#include <vector>

namespace NBenchmark
{

template<typename T>
void BubbleSortClassic(std::vector<T>& arr)
{
    const size_t n = arr.size();
    if (n < 2)
    {
        return;
    }

    for (size_t i = 0; i < n - 1; ++i)
    {
        for (size_t j = 0; j < n - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

template<typename T>
void StdSort(std::vector<T>& arr)
{
    std::sort(arr.begin(), arr.end());
}

} // namespace NBenchmark