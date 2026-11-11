cmake_minimum_required(VERSION 3.16)
project(Lab2Testing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Подключение зависимостей ---
include(FetchContent)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Google Benchmark
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# --- Сборка библиотеки структур ---
add_library(structures STATIC
    structures/array.cpp
    structures/slist.cpp
    structures/dlist.cpp
    structures/queue.cpp
    structures/stack.cpp
    structures/hashSet.cpp
    structures/cuckooHash.cpp
    structures/blackredtree.cpp
)
target_include_directories(structures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/structures)

# --- Флаги покрытия ТОЛЬКО для наших структур ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(structures PRIVATE -O0 -g --coverage)
    target_link_options(structures PRIVATE --coverage)
endif()

# --- Тесты (Google Test) ---
enable_testing()
add_executable(unit_tests tests/test_structures.cpp)
target_link_libraries(unit_tests PRIVATE structures GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_tests)

# --- Флаги покрытия для тестов (чтобы gcovr видел их) ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(unit_tests PRIVATE -O0 -g --coverage)
    target_link_options(unit_tests PRIVATE --coverage)
endif()

# --- Бенчмарки (Google Benchmark) ---
add_executable(benchmarks benchmarks/bench_structures.cpp)
target_link_libraries(benchmarks PRIVATE structures benchmark::benchmark benchmark::benchmark_main)

# Флаг покрытия для бенчмарков (нужен, чтобы линковщик нашел __gcov_init)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_options(benchmarks PRIVATE --coverage)
endif()