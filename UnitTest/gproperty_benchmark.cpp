#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <limits>
#include <array>
#include <GFramework/GVariant/GObject.h>
#include <GFramework/GVariant/GVariant.h>
#include <GFramework/GVariant/GProperty.h>
#include <benchmark/benchmark.h>

using namespace std;
using namespace GFramework;

int int_value = 123;
int direct_value = 0;
GVariant variant_value;
static void direct_value_assign(benchmark::State& state) {
    for (auto _ : state)
    {
        direct_value = int_value++;
    }
}

static void variant_value_assign(benchmark::State& state) {
    for (auto _ : state)
    {
        variant_value = int_value++;
    }
}

BENCHMARK(direct_value_assign);
BENCHMARK(variant_value_assign);

