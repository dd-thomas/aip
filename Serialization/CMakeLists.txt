cmake_minimum_required(VERSION 3.10)

project(Serialization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

if(NOT DEFINED ENV{WORKSDK})
    message(FATAL_ERROR "Environment variable WORKSDK is not set.")
endif()

# ---------------------------------------------------------------------------
# Boost
#
# Проекту нужны компилируемые библиотеки Boost.Serialization и Boost.Test,
# поэтому Boost должен быть собран через b2 (см. SDK/manual/sdk.md):
#   b2 stage --with-serialization --with-test address-model=64 link=static runtime-link=shared threading=multi variant=debug,release
# ---------------------------------------------------------------------------
set(BOOST_VERSION "1.80.0")
set(BOOST_ROOT "$ENV{WORKSDK}/boost_1_80_0")
file(TO_CMAKE_PATH "${BOOST_ROOT}" BOOST_ROOT)

set(BOOST_COMPONENTS
    serialization
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
        "Build Boost.Serialization and Boost.Test with b2, see SDK/manual/sdk.md and Serialization/manual/serialization.md")
endif()

message(STATUS "Serialization: Boost ${Boost_VERSION} found: ${Boost_DIR}")

# MSVC: Debug-сборка требует debug-вариант библиотек Boost (иначе LNK2038 mismatch detected for 'RuntimeLibrary')
if(MSVC)
    get_target_property(SERIALIZATION_BOOST_CONFIGS Boost::serialization IMPORTED_CONFIGURATIONS)
    if(NOT "DEBUG" IN_LIST SERIALIZATION_BOOST_CONFIGS)
        message(WARNING
            "Serialization: debug variant of Boost.Serialization not found (${SERIALIZATION_BOOST_CONFIGS}). Debug build will fail with LNK2038.\n"
            "Rebuild Boost with variant=debug,release, see SDK/manual/sdk.md")
    endif()
endif()

# ---------------------------------------------------------------------------
# Библиотека сериализации
# ---------------------------------------------------------------------------
set(Serialization_HDRS
    include/HttpServerSettings.h
    include/Serialization.h
)

set(Serialization_SRCS
    src/Serialization.cpp
)

add_library(Serialization STATIC
    ${Serialization_HDRS}
    ${Serialization_SRCS}
)

target_include_directories(Serialization
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# PUBLIC: заголовок HttpServerSettings.h включает заголовки Boost.Serialization,
# поэтому они нужны и потребителям библиотеки (раннеру, SerializationDemo)
target_link_libraries(Serialization
    PUBLIC
        Boost::serialization
)

target_compile_features(Serialization PUBLIC cxx_std_17)

# ---------------------------------------------------------------------------
# Тестовый раннер
# ---------------------------------------------------------------------------
set(Serialization_TEST_SRCS
    test/testHttpServerSettings.cpp
)

add_executable(SerializationUTRunner
    ${Serialization_TEST_SRCS}
)

target_link_libraries(SerializationUTRunner
    PRIVATE
        Serialization
        Boost::unit_test_framework
)

# Путь к эталонным XML-файлам разных версий
target_compile_definitions(SerializationUTRunner
    PRIVATE
        SERIALIZATION_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test/assets"
)

add_test(NAME SerializationTests COMMAND SerializationUTRunner)

# ---------------------------------------------------------------------------
# Демонстрационное приложение
# ---------------------------------------------------------------------------
set(SerializationDemo_SRCS
    demo/main.cpp
)

add_executable(SerializationDemo
    ${SerializationDemo_SRCS}
)

# Путь include и Boost::serialization приложение получает транзитивно от Serialization
target_link_libraries(SerializationDemo
    PRIVATE
        Serialization
)
