project(SDK)

if(NOT DEFINED ENV{WORKSDK})
    message(FATAL_ERROR "Environment variable WORKSDK is not set.")
endif()

# ---------------------------------------------------------------------------
# Boost
#
# BOOST_ROOT -- каталог, в который распакован архив с boost.org
# (https://www.boost.org/users/download/), например $WORKSDK/boost_1_80_0.
#
# Поддерживаются два варианта:
#   1. Библиотеки собраны командой "b2 stage" (или установлены "b2 install") --
#      CMake находит BoostConfig.cmake и доступны все цели Boost::<library>.
#   2. Архив только распакован -- доступны лишь header-only библиотеки
#      (цель Boost::headers).
# ---------------------------------------------------------------------------
set(BOOST_VERSION "1.80.0")
set(BOOST_ROOT "$ENV{WORKSDK}/boost_1_80_0")
file(TO_CMAKE_PATH "${BOOST_ROOT}" BOOST_ROOT)

# Компилируемые библиотеки Boost, которые нужны проекту (например: program_options filesystem).
# Для каждой из них становится доступна цель Boost::<library>.
set(BOOST_COMPONENTS
)

set(Boost_USE_STATIC_LIBS ON) # Линковка со статическими библиотеками (libboost_*.lib / libboost_*.a)

find_package(Boost ${BOOST_VERSION} EXACT CONFIG QUIET
    COMPONENTS ${BOOST_COMPONENTS}
    HINTS
        "${BOOST_ROOT}/stage" # После "b2 stage"
        "${BOOST_ROOT}"       # После "b2 install --prefix=..."
    NO_DEFAULT_PATH
)

if(Boost_FOUND)
    message(STATUS "Boost ${Boost_VERSION} found: ${Boost_DIR}")
elseif(NOT BOOST_COMPONENTS AND EXISTS "${BOOST_ROOT}/boost/version.hpp")
    message(STATUS "Boost compiled libraries not found, header-only mode: ${BOOST_ROOT}")
    add_library(Boost::headers INTERFACE IMPORTED)
    target_include_directories(Boost::headers INTERFACE "${BOOST_ROOT}")
else()
    message(FATAL_ERROR
        "Boost ${BOOST_VERSION} not found in ${BOOST_ROOT}.\n"
        "Download boost_1_80_0 archive from https://www.boost.org/users/download/ and unpack it to $ENV{WORKSDK}.\n"
        "Compiled components (${BOOST_COMPONENTS}) must be built with b2, see SDK/manual/sdk.md")
endif()

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
set(SDK_SOURCES
    src/main.cpp
)

add_executable(SDK
    ${SDK_SOURCES}
)

target_link_libraries(SDK
    PRIVATE
        Boost::headers
)
