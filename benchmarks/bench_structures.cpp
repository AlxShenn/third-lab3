#include <benchmark/benchmark.h>
#include "../structures/array.h"
#include "../structures/slist.h"
#include "../structures/hashSet.h"

static void BM_ArrayPushBack(benchmark::State& state) {
    for (auto _ : state) {
        Array arr(10);
        for (int i = 0; i < state.range(0); ++i) {
            arr.addEnd("test");
        }
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_ArrayPushBack)->Range(8, 8<<10);

static void BM_SlistPushHead(benchmark::State& state) {
    for (auto _ : state) {
        Slist list;
        for (int i = 0; i < state.range(0); ++i) {
            list.pushHead("test");
        }
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_SlistPushHead)->Range(8, 8<<10);

static void BM_HashSetInsert(benchmark::State& state) {
    for (auto _ : state) {
        HashSet<std::string> set;
        for (int i = 0; i < state.range(0); ++i) {
            set.add(std::to_string(i));
        }
        benchmark::DoNotOptimize(set);
    }
}
BENCHMARK(BM_HashSetInsert)->Range(8, 8<<10);