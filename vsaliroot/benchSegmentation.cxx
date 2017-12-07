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
#include "segmentationInterface.h"
#include "contour.h"
#include "AliMpDetElement.h"
#include "AliMpVSegmentation.h"
#include "AliMpSegmentation.h"
#include "commonBench.h"
#include "contourCreator.h"
#include "segmentationContours.h"

namespace {
constexpr int NTESTPOINTS{1000};
}

std::vector<std::pair<double, double>> generateTestPoints(int n, int deIndex)
{
  int segTypeIndex = o2::mch::mapping::getSegTypeIndexFromDetElemIndex(deIndex);
  auto seg = o2::mch::mapping::getSegmentation(segTypeIndex, true);
  auto bbox = o2::mch::contour::getBBox(getEnvelop(o2::mch::mapping::getSampaContours(*(seg.get()))));

  double offset{0.0};

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distX{bbox.xmin() - offset, bbox.xmax() + offset};
  std::uniform_real_distribution<double> distY{bbox.ymin() - offset, bbox.ymax() + offset};

  std::vector<std::pair<double, double>> testPoints(n);

  std::generate(testPoints.begin(), testPoints.end(),
                [&distX, &distY, &mt] { return std::make_pair<double, double>(distX(mt), distY(mt)); });

  return testPoints;
}

static void segmentationList(benchmark::internal::Benchmark *b)
{
  for (auto i  : {0, 8, 16, 17, 18, 19, 20, 34, 35, 36, 52, 53, 54, 55, 56, 57, 58, 106, 107, 108, 109}) {
    b->Args({i, true});
    b->Args({i, false});
  }
}

class BenchO2 : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(BenchO2, hasPadByPosition)(benchmark::State &state)
{
  int deIndex = state.range(0);
  bool isBendingPlane = state.range(1);

  int segTypeIndex = o2::mch::mapping::getSegTypeIndexFromDetElemIndex(deIndex);
  auto seg = o2::mch::mapping::getSegmentation(segTypeIndex, isBendingPlane);

  auto testPoints = generateTestPoints(NTESTPOINTS, deIndex);

  double nin{0};
  double n{0};

  for (auto _ : state) {
    for (const auto &tp: testPoints) {
      n++;
      nin += seg->hasPadByPosition(tp.first, tp.second);
    }
  }

  state.counters["nin"] = benchmark::Counter(nin,benchmark::Counter::kIsRate);
  state.counters["n"] = benchmark::Counter(n,benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchO2, hasPadByPosition)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);

BENCHMARK_DEFINE_F(BenchAliRoot, PadByPosition)(benchmark::State &state)
{
  int deIndex = state.range(0);
  bool isBendingPlane = state.range(1);

  int deId = o2::mch::mapping::getDetElemIdFromDetElemIndex(deIndex);
  AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(deId);
  auto seg = mseg->GetMpSegmentation(deId, detElement->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

  auto testPoints = generateTestPoints(NTESTPOINTS, deIndex);

  double nin{0};
  double n{0};

  for (auto _ : state) {
    for (const auto &tp: testPoints) {
      nin += seg->PadByPosition(tp.first, tp.second, false).IsValid();
      n++;
    }
  }

  state.counters["nin"] = benchmark::Counter(nin,benchmark::Counter::kIsRate);
  state.counters["n"] = benchmark::Counter(n,benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchAliRoot, PadByPosition)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();