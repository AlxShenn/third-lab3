cmake_minimum_required(VERSION 3.16)
project(Lab2Testing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем системные библиотеки
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

# Флаги покрытия (только для наших структур и тестов)
add_compile_options(-O0 -g --coverage)
add_link_options(--coverage)

# Сборка библиотеки структур
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

# Тесты
enable_testing()
add_executable(unit_tests tests/test_structures.cpp)
target_link_libraries(unit_tests PRIVATE structures GTest::gtest_main)

# Бенчмарки
add_executable(benchmarks benchmarks/bench_structures.cpp)
target_link_libraries(benchmarks PRIVATE structures benchmark::benchmark benchmark::benchmark_main)