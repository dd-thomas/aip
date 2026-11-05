cmake_minimum_required(VERSION 3.10)

project(BoostUT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

if(NOT DEFINED ENV{WORKSDK})
    message(FATAL_ERROR "Environment variable WORKSDK is not set.")
endif()

# ---------------------------------------------------------------------------
# Boost
#
# Проекту нужна компилируемая библиотека Boost.Test (unit_test_framework),
# поэтому Boost должен быть собран через b2 (см. SDK/manual/sdk.md):
#   b2 stage --with-test address-model=64 link=static runtime-link=shared threading=multi variant=debug,release
# ---------------------------------------------------------------------------
set(BOOST_VERSION "1.80.0")
set(BOOST_ROOT "$ENV{WORKSDK}/boost_1_80_0")
file(TO_CMAKE_PATH "${BOOST_ROOT}" BOOST_ROOT)

set(BOOST_COMPONENTS
    unit_test_framework
)

set(Boost_USE_STATIC_LIBS ON) # Линковка со статическими библиотеками (libboost_*.lib / libboost_*.a)

find_package(Boost ${BOOST_VERSION} EXACT CONFIG QUIET
    COMPONENTS ${BOOST_COMPONENTS}
    HINTS
        "${BOOST_ROOT}/stage" # После "b2 stage"
        "${BOOST_ROOT}"       # После "b2 install --prefix=..."
    NO_DEFAULT_PATH
)

if(NOT Boost_FOUND)
    message(FATAL_ERROR
        "Boost ${BOOST_VERSION} with components (${BOOST_COMPONENTS}) not found in ${BOOST_ROOT}.\n"
        "Build Boost.Test with b2, see SDK/manual/sdk.md and BoostUT/manual/boost_ut.md")
endif()

message(STATUS "BoostUT: Boost ${Boost_VERSION} found: ${Boost_DIR}")

# MSVC: Debug-сборка требует debug-вариант библиотек Boost (иначе LNK2038 mismatch detected for 'RuntimeLibrary')
if(MSVC)
    get_target_property(BOOST_UT_CONFIGS Boost::unit_test_framework IMPORTED_CONFIGURATIONS)
    if(NOT "DEBUG" IN_LIST BOOST_UT_CONFIGS)
        message(WARNING
            "BoostUT: debug variant of Boost.Test not found (${BOOST_UT_CONFIGS}). Debug build will fail with LNK2038.\n"
            "Rebuild Boost with variant=debug,release, see SDK/manual/sdk.md")
    endif()
endif()

# ---------------------------------------------------------------------------
# Тестируемая библиотека
# ---------------------------------------------------------------------------
set(BoostUT_HDRS
    include/TimeUtils.h
)

set(BoostUT_SRCS
    src/TimeUtils.cpp
)

add_library(BoostUT STATIC
    ${BoostUT_HDRS}
    ${BoostUT_SRCS}
)

target_include_directories(BoostUT
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# PUBLIC: заголовок TimeUtils.h включает заголовки Boost, поэтому они нужны и потребителям библиотеки
target_link_libraries(BoostUT
    PUBLIC
        Boost::headers
)

# ---------------------------------------------------------------------------
# Тестовый раннер
# ---------------------------------------------------------------------------
set(BoostUT_TEST_SRCS
    tests/testTimeUtils.cpp
)

add_executable(BoostUTRunner
    ${BoostUT_TEST_SRCS}
)

target_link_libraries(BoostUTRunner
    PRIVATE
        BoostUT
        Boost::unit_test_framework
)

add_test(NAME BoostUTTests COMMAND BoostUTRunner)
