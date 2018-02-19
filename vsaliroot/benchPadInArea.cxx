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
#include "segmentation.h"
#include "contour.h"
#include "AliMpDetElement.h"
#include "AliMpVSegmentation.h"
#include "AliMpSegmentation.h"
#include "commonBench.h"
#include "contourCreator.h"
#include "segmentationContours.h"
#include "generateTestPoints.h"
#include "AliMpVPadIterator.h"

static void segmentationList(benchmark::internal::Benchmark *b)
{
  o2::mch::mapping::forOneDetectionElementOfEachSegmentationType([&b](int detElemId) {
    for (auto bending : {true, false}) {
      b->Args({detElemId, bending});
    }
  });
}

class BenchO2 : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(BenchAliRoot, PadInArea)(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);

  AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);
  auto seg = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

  std::unique_ptr<AliMpVPadIterator> it{seg->CreateIterator()};

  int npads{0};

  for (auto _ : state) {
    npads = 0;
    it->First();
    while (!it->IsDone()) {
      npads++;
      it->Next();
    }
  }

  state.counters["npads"] = benchmark::Counter(npads);
}

BENCHMARK_REGISTER_F(BenchAliRoot, PadInArea)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);

BENCHMARK_DEFINE_F(BenchO2, PadInArea)(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);

  o2::mch::mapping::Segmentation seg{detElemId, isBendingPlane};

  int npads{0};

  auto bbox = o2::mch::mapping::getBBox(seg);

  for (auto _ : state) {
    npads = 0;
    seg.forEachPadInArea(bbox.xmin(), bbox.ymin(), bbox.xmax(), bbox.ymax(), [&npads](int paduid) {
      ++npads;
    });
  }

  state.counters["npads"] = benchmark::Counter(npads);
}

BENCHMARK_REGISTER_F(BenchO2, PadInArea)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);

