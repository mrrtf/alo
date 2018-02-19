//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche

#include "benchmark/benchmark.h"
#include "segmentationFactory.h"

static void detElemIdList(benchmark::internal::Benchmark *b)
{
  for (auto detElemId : o2::mch::mapping::impl1::getOneDetElemIdPerSegmentation()) {
    for (auto bending : {true, false}) {
      b->Args({detElemId, bending});
    }
  }
}

static void benchSegmentationFactoryOnce(benchmark::State &state)
{
  auto v{o2::mch::mapping::impl1::getOneDetElemIdPerSegmentation()};
  for (auto _ : state) {
    for (auto detElemId: v) {
      for (auto bending : {true, false}) {
        auto seg = o2::mch::mapping::impl1::getSegmentation(detElemId, bending);
        benchmark::DoNotOptimize(seg->getId());
      }
    }
  }
}

static void benchSegmentationFactoryByDetElemId(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool bending = state.range(1);

  for (auto _ : state) {
    auto seg = o2::mch::mapping::impl1::getSegmentation(detElemId, bending);
    benchmark::DoNotOptimize(seg->getId());
  }
}

static void benchGetOneDetElemIdPerSegmentation(benchmark::State &state)
{
  for (auto _ : state) {
    auto v = o2::mch::mapping::impl1::getOneDetElemIdPerSegmentation();
    benchmark::DoNotOptimize(v.size());
  }
}

BENCHMARK(benchSegmentationFactoryByDetElemId)->Apply(detElemIdList)->Unit(benchmark::kMillisecond);
BENCHMARK(benchGetOneDetElemIdPerSegmentation)->Unit(benchmark::kMillisecond);
BENCHMARK(benchSegmentationFactoryOnce)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN()