#include <iostream>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include "../src/Timer.h"
#include "../src/Sorting.h"

std::vector<int> GenerateIntVector(size_t size, int minVal, int maxVal)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minVal, maxVal);

    std::vector<int> result(size);
    for (auto& val : result)
    {
        val = dist(gen);
    }

    return result;
}

std::vector<double> GenerateDoubleVector(size_t size, double minVal, double maxVal)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(minVal, maxVal);

    std::vector<double> result(size);
    for (auto& val : result)
    {
        val = dist(gen);
    }

    return result;
}

std::vector<std::string> GenerateStringVector(size_t size, int minStringSize, int maxStringSize)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> len_dist(minStringSize, maxStringSize);
    std::uniform_int_distribution<> char_dist(97, 122);

    std::vector<std::string> result(size);
    for (auto& str : result)
    {
        size_t len = len_dist(gen);
        str.resize(len);
        for (auto& c : str)
        {
            c = char_dist(gen);
        }
    }

    return result;
}

class CTestCase
{
public:
    explicit CTestCase(const std::string& name, std::function<int()> test) :
        m_name(name),
        m_test(std::move(test))
    {
    }

    const std::string& GetName() const
    {
        return m_name;
    }

    int operator()() const
    {
        return m_test();
    }

private:
    const std::string m_name;
    const std::function<int()> m_test;
};


class CFixture
{
public:
    void AddTest(const CTestCase& test)
    {
        m_tests.emplace_back(std::move(test));
    }

    void Run()
    {
        std::cout << "Running " << m_tests.size() << " tests..." << std::endl;

        for (const auto& test : m_tests)
        {
            try
            {
                std::cout << "--------------------------------" << std::endl;
                std::cout << "Test " << test.GetName() << std::endl;
                std::cout << "--------------------------------" << std::endl;
                const auto result = test();
                if (result == 0)
                {
                    m_passed++;
                    std::cout << "[PASS] Test " << test.GetName() << std::endl;
                }
                else
                {
                    m_failed++;
                    std::cout << "[FAIL] Test " << test.GetName() << ": " << result << std::endl;
                }
            }
            catch (const std::exception& e)
            {
                m_failed++;
                std::cout << "[FAIL] Test " << test.GetName() << ": " << e.what() << std::endl;
            }
        }

        std::cout << "Results: " << m_passed << " passed, " << m_failed << " failed\n";
    }

private:
    std::vector<CTestCase> m_tests;
    int m_passed = 0;
    int m_failed = 0;
};

int SortIntVector(size_t size, int minVal, int maxVal)
{
    {
        auto vec = GenerateIntVector(size, minVal, maxVal);
        NBenchmark::CTimer timer;
        NBenchmark::BubbleSortClassic(vec);
    }

    {
        auto vec = GenerateIntVector(size, minVal, maxVal);
        NBenchmark::CTimer timer;
        NBenchmark::StdSort(vec);
    }

    return 0;
}

int Test1()
{
    return SortIntVector(1000, 0, 1000);
}

int Test2()
{
    return SortIntVector(5000, 0, 1000);
}

int Test3()
{
    return SortIntVector(100, 10'000'000, 100'000'000);
}

// #1 Поддержать тестовые случаи для std::string
// #2 Найти модификацию BubbleSort, которая поможет снизить сложность исходного алгоритма и добавить ее в тесты
// #3 Использовать сортировку другого класса. Например, вставками
// #4 сохранять результаты тестов в файл (опционально)
int main()
{
    CFixture f;

    f.AddTest(CTestCase("Test1 (size: 1000, minVal: 0, maxVal: 1000)", Test1));
    f.AddTest(CTestCase("Test2 (size: 5000, minVal: 0, maxVal: 1000)", Test2));
    f.AddTest(CTestCase("Test3 (size: 100, minVal: 10'000'000, maxVal: 100'000'000)", Test3));

    f.Run();

    return 0;
}