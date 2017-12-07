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


#include <random>
#include "benchmark/benchmark.h"
#include "Segmentation.h"
#include "Contour.h"
#include "AliMpDetElement.h"
#include "AliMpVSegmentation.h"
#include "AliMpSegmentation.h"
#include "CommonBench.h"
#include "ContourCreator.h"
#include "SegmentationContours.h"
#include "GenerateTestPoints.h"

namespace {
constexpr int NTESTPOINTS{1000};
}

using o2::mch::mapping::generateTestPoints;

static void segmentationListWithExtent(benchmark::internal::Benchmark *b)
{
  o2::mch::mapping::forOneDetectionElementOfEachSegmentationType([&b](int detElemId) {
    for (auto bending : {true, false}) {
      for (auto extent : {0, 10}) {
        b->Args({detElemId, bending, extent});
      }
    }
  });
}

class BenchO2 : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(BenchO2, hasPadByPosition)(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);
  int extent = state.range(2);

  o2::mch::mapping::Segmentation seg{detElemId, isBendingPlane};

  auto bbox = getBBox(seg);

  auto testPoints = generateTestPoints(NTESTPOINTS, bbox.xmin(), bbox.ymin(), bbox.xmax(), bbox.ymax(), extent);

  int nin{0};
  int n{0};

  for (auto _ : state) {
    for (const auto &tp: testPoints) {
      n++;
      nin += seg.findPadByPosition(tp.first, tp.second);
    }
  }

  state.counters["nin"] = benchmark::Counter(nin, benchmark::Counter::kIsRate);
  state.counters["n"] = benchmark::Counter(n, benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchO2, hasPadByPosition)->Apply(segmentationListWithExtent)->Unit(benchmark::kMicrosecond);

BENCHMARK_DEFINE_F(BenchAliRoot, PadByPosition)(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);
  int extent = state.range(2);

  AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);
  auto seg = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

  auto testPoints = generateTestPoints(NTESTPOINTS,
                                       seg->GetPositionX() - seg->GetDimensionX(),
                                       seg->GetPositionY() - seg->GetDimensionY(),
                                       seg->GetPositionX() + seg->GetDimensionX(),
                                       seg->GetPositionY() + seg->GetDimensionY(),
                                       extent);

  double nin{0};
  double n{0};

  for (auto _ : state) {
    for (const auto &tp: testPoints) {
      nin += seg->PadByPosition(tp.first, tp.second, false).IsValid();
      n++;
    }
  }

  state.counters["nin"] = benchmark::Counter(nin, benchmark::Counter::kIsRate);
  state.counters["n"] = benchmark::Counter(n, benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchAliRoot, PadByPosition)->Apply(segmentationListWithExtent)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();