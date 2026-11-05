# aip

Лабораторные работы по C++: окружение, сборка с CMake, модульное тестирование, измерение производительности, сторонние библиотеки.

## Порядок выполнения работ

Работы выполняются строго по порядку: каждая следующая опирается на навыки и окружение, полученные в предыдущих.

| № | Работа | Мануал | Что нужно для сборки |
|---|---|---|---|
| 1 | **EnvironmentTest** -- настройка окружения | [environment.md](EnvironmentTest/manual/environment.md) | vscode, CMake, компилятор C++, Git |
| 2 | **BasicUT** -- базовое модульное тестирование | [basic_ut.md](BasicUT/manual/basic_ut.md) | то же, что для EnvironmentTest |
| 3 | **Benchmark** -- измерение производительности | [benchmark.md](Benchmark/manual/benchmark.md) | то же, что для EnvironmentTest |
| 4 | **SDK** -- подключение сторонних библиотек (Boost) | [sdk.md](SDK/manual/sdk.md) | + переменная окружения **WORKSDK**, распакованный Boost 1.80.0 |
| 5 | **BoostUT** -- модульное тестирование с Boost.Test | [boost_ut.md](BoostUT/manual/boost_ut.md) | + Boost.Test, собранный через b2 (см. SDK) |
| 6 | **Serialization** -- сериализация данных с Boost.Serialization | [serialization.md](Serialization/manual/serialization.md) | + Boost.Serialization, собранный через b2 (см. SDK) |

## Структура репозитория

Каждая работа -- **самостоятельный CMake-проект** в своей папке. Общего `CMakeLists.txt` в корне нет: проекты конфигурируются и собираются независимо друг от друга. Поэтому первые работы (EnvironmentTest, BasicUT, Benchmark) можно выполнять без Boost и без переменной **WORKSDK** -- они понадобятся только начиная с работы SDK.

```
aip
    EnvironmentTest
    BasicUT
    Benchmark
    SDK
    BoostUT
    Serialization
```

## Сборка проекта

В vscode открыть **папку конкретной работы** (например, `aip/BasicUT`), а не корень репозитория, и выполнить Configure и Build в панели CMake.

Из терминала (из папки работы):

```
cmake -S . -B build
cmake --build build
```

Каталог `build` создается внутри папки работы и исключен из-под контроля версий (`.gitignore`).
