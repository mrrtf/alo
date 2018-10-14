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
#include "MCHMappingInterface/Segmentation.h"
#include "MCHContour/Contour.h"
#include "AliMpDetElement.h"
#include "AliMpVSegmentation.h"
#include "AliMpSegmentation.h"
#include "CommonBench.h"
#include "MCHContour/ContourCreator.h"
#include "MCHMappingSegContour/SegmentationContours.h"
#include "GenerateTestPoints.h"
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

BENCHMARK_DEFINE_F(BenchAliRoot, PadNeighbours)(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);

  AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);
  auto seg = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

  std::unique_ptr<AliMpVPadIterator> it{seg->CreateIterator()};
  std::vector<AliMpPad> pads;

  it->First();
  while (!it->IsDone()) {
    pads.push_back(it->CurrentItem());
    it->Next();
  }

  int nn{0};

  TObjArray neighbours;

  for (auto _ : state) {
    nn = 0;
    for (auto pad: pads) {
      seg->GetNeighbours(pad, neighbours);
      nn += neighbours.GetEntries();
    }
  }

  state.counters["nofNeighbours"] = benchmark::Counter(nn);
}

BENCHMARK_DEFINE_F(BenchAliRoot, PadAllNeighbours)(benchmark::State &state)
{
  o2::mch::mapping::forOneDetectionElementOfEachSegmentationType([&state](int detElemId) {
    for (auto isBendingPlane : {true, false}) {
      AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);
      auto seg = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
        isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

      std::unique_ptr<AliMpVPadIterator> it{seg->CreateIterator()};
      std::vector<AliMpPad> pads;

      it->First();
      while (!it->IsDone()) {
        pads.push_back(it->CurrentItem());
        it->Next();
      }

      TObjArray neighbours;

      for (auto _ : state) {
        for (auto pad: pads) {
          seg->GetNeighbours(pad, neighbours);
        }
      }

    }
  });
}

BENCHMARK_REGISTER_F(BenchAliRoot, PadNeighbours)->Apply(segmentationList)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchAliRoot, PadAllNeighbours)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(BenchO2, PadNeighbours)(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);

  o2::mch::mapping::Segmentation seg{detElemId, isBendingPlane};

  auto bbox = o2::mch::mapping::getBBox(seg);

  std::vector<int> paduids;
  seg.forEachPadInArea(bbox.xmin(), bbox.ymin(), bbox.xmax(), bbox.ymax(), [&paduids](int paduid) {
    paduids.push_back(paduid);
  });

  int nn{0};

  for (auto _ : state) {
    nn = 0;
    for (auto paduid: paduids) {
      seg.forEachNeighbouringPad(paduid, [&nn, &seg](int nuid) {
        ++nn;
        // to be fair with AliRoot, access all the pad data
        benchmark::DoNotOptimize(seg.padPositionX(nuid) + seg.padPositionY(nuid)
                                 + seg.padDualSampaId(nuid) + seg.padDualSampaChannel(nuid)
                                 + seg.padSizeY(nuid) + seg.padSizeX(nuid));
      });
    }
  }

  state.counters["nofNeighbours"] = benchmark::Counter(nn);
}

BENCHMARK_DEFINE_F(BenchO2, PadAllNeighbours)(benchmark::State &state)
{
  o2::mch::mapping::forOneDetectionElementOfEachSegmentationType([&state](int detElemId) {
    for (auto isBendingPlane : {true, false}) {

      o2::mch::mapping::Segmentation seg{detElemId, isBendingPlane};

      auto bbox = o2::mch::mapping::getBBox(seg);

      std::vector<int> paduids;
      seg.forEachPadInArea(bbox.xmin(), bbox.ymin(), bbox.xmax(), bbox.ymax(), [&paduids](int paduid) {
        paduids.push_back(paduid);
      });


      for (auto _ : state) {
        for (auto paduid: paduids) {
          seg.forEachNeighbouringPad(paduid, [&seg](int nuid) {
            // to be fair with AliRoot, access all the pad data
            benchmark::DoNotOptimize(seg.padPositionX(nuid) + seg.padPositionY(nuid)
                                     + seg.padDualSampaId(nuid) + seg.padDualSampaChannel(nuid)
                                     + seg.padSizeY(nuid) + seg.padSizeX(nuid));
          });
        }
      }

    }
  });
}

BENCHMARK_REGISTER_F(BenchO2, PadNeighbours)->Apply(segmentationList)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchO2, PadAllNeighbours)->Unit(benchmark::kMillisecond);

