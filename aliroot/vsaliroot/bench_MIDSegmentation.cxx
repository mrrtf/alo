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
/// @author  Diego Stocco

#include "AliMpDetElement.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include "CommonBench.h"
#include "MIDBase/LegacyUtility.h"
#include "MIDBase/Mapping.h"
#include "MIDBase/MpArea.h"
#include "benchmark/benchmark.h"

#include <vector>
#include <algorithm>
#include <random>

namespace {
constexpr int NTESTPOINTS{1000};
}

using namespace o2::mid;

std::vector<std::pair<double, double>> generateTestPoints(int n, int deIndex) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distX(-127.5, 127.5);
  std::uniform_real_distribution<double> distY(-68., 68.);

  std::vector<std::pair<double, double>> testPoints(n);

  std::generate(testPoints.begin(), testPoints.end(), [&distX, &distY, &mt] {
    return std::make_pair<double, double>(distX(mt), distY(mt));
  });

  return testPoints;
}

static void segmentationList(benchmark::internal::Benchmark *bench) {
  for (int iboard = 1; iboard <= 16; ++iboard) {
    bench->Args({iboard, 0});
    bench->Args({iboard, 1});
  }
}

static void deList(benchmark::internal::Benchmark *bench) {
  for (int deId = 0; deId < 9; ++deId) {
    bench->Args({deId, 0});
    bench->Args({deId, 1});
  }
}

class BenchO2 : public benchmark::Fixture {};

BENCHMARK_DEFINE_F(BenchO2, stripByLoc)(benchmark::State &state) {

  int iboard = state.range(0);
  int icath = state.range(1);

  Mapping o2Segmentation;

  double num{0};

  for (auto _ : state) {
    for (int istrip = 0; istrip < 16; ++istrip) {
      MpArea o2Pad =
          o2Segmentation.stripByLocationInBoard(istrip, icath, iboard, 0, false);
      ++num;
    }
  }

  state.counters["num"] = benchmark::Counter(num, benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchO2, stripByLoc)
    ->Apply(segmentationList)
    ->Unit(benchmark::kNanosecond);

BENCHMARK_DEFINE_F(BenchAliRoot, stripByLocAliroot)(benchmark::State &state) {
  int iboard = state.range(0);
  int icath = state.range(1);

  int detElemId = ddlStore->GetDEfromLocalBoard(iboard, 10);
  const AliMpVSegmentation *seg =
      AliMpSegmentation::Instance()->GetMpSegmentation(
          detElemId, AliMp::GetCathodType(icath));

  double num{0};

  for (auto _ : state) {
    for (int istrip = 0; istrip < 16; ++istrip) {
      AliMpPad pad = seg->PadByLocation(iboard, istrip, false);
      ++num;
    }
  }

  state.counters["num"] = benchmark::Counter(num, benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchAliRoot, stripByLocAliroot)
    ->Apply(segmentationList)
    ->Unit(benchmark::kNanosecond);

BENCHMARK_DEFINE_F(BenchO2, stripByPos)(benchmark::State &state) {

  int deIndex = state.range(0);
  int icath = state.range(1);

  Mapping o2Segmentation;

  double num{0};
  auto testPoints = generateTestPoints(NTESTPOINTS, deIndex);

  for (auto _ : state) {
    for (const auto &tp : testPoints) {
      Mapping::MpStripIndex stripIndex = o2Segmentation.stripByPosition(
          tp.first, tp.second, icath, deIndex, false);
      // if (!stripIndex.isValid())
      //   continue;
      // MpArea o2Pad = o2Segmentation.stripByLocation(
      //     stripIndex.strip, icath, stripIndex.line, stripIndex.column, deIndex,
      //     false);
      ++num;
    }
  }

  state.counters["num"] = benchmark::Counter(num, benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchO2, stripByPos)
    ->Apply(deList)
    ->Unit(benchmark::kNanosecond);

BENCHMARK_DEFINE_F(BenchO2, stripByPosAliroot)(benchmark::State &state) {

  int deIndex = state.range(0);
  int icath = state.range(1);

  int detElemId = LegacyUtility::convertToLegacyDeId(deIndex);

  const AliMpVSegmentation *seg =
      AliMpSegmentation::Instance()->GetMpSegmentation(
          detElemId, AliMp::GetCathodType(icath));

  double num{0};
  auto testPoints = generateTestPoints(NTESTPOINTS, deIndex);

  for (auto _ : state) {
    for (const auto &tp : testPoints) {
      AliMpPad pad = seg->PadByPosition(tp.first, tp.second, false);
      ++num;
    }
  }

  state.counters["num"] = benchmark::Counter(num, benchmark::Counter::kIsRate);
}

BENCHMARK_REGISTER_F(BenchO2, stripByPosAliroot)
    ->Apply(deList)
    ->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
