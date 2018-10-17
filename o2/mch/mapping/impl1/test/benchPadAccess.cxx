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
// A stupid benchmark to see if there is a time difference in getting pad information
// in two ways :
// - get the pad struct and then use the struct to access its members
// - wrap the struct access by methods (which opens the possibility
// to control access, access information in a different place, etc... but then this
// is not what we are benchmarking here anyway...)
//

#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

struct Pad
{
    double x1, x2, y1, y2;
};

struct Segmentation
{

    Segmentation(int n) : mPads(n), mPads2(n)
    {
      std::random_device rd;
      std::mt19937 mt(rd());
      std::uniform_real_distribution<double> distX{0, 100};
      std::generate(mPads.begin(), mPads.end(),
                    [&distX, &mt] { return Pad{distX(mt), distX(mt), distX(mt), distX(mt)}; });
      std::generate(mPads2.begin(), mPads2.end(),
                    [&distX, &mt] { return Pad{distX(mt), distX(mt), distX(mt), distX(mt)}; });
    }

    const std::vector<Pad> &getPads() const
    { return mPads; }

    double x1(int i) const
    { return mPads[i].x1; }

    double x2(int i) const
    { return mPads[i].x2; }

    double y1(int i) const
    { return mPads[i].y1; }

    double y2(int i) const
    { return mPads[i].y2; }

    std::vector<Pad> mPads;
    std::vector<Pad> mPads2;
};

static void benchPadAccessThroughVector(benchmark::State &state)
{

  int n = state.range(0);
  Segmentation seg(n);

  auto pads = seg.getPads();

  for (auto _ : state) {

    double sum{0};

    for (int i = 0; i < pads.size(); ++i) {
      sum += pads[i].x1 + pads[i].x2 + pads[i].y1 + pads[i].y2;
    }
    benchmark::DoNotOptimize(sum + 1);
  }
}

static void benchPadAccessThroughInterface(benchmark::State &state)
{

  int n = state.range(0);
  Segmentation seg(n);

  auto pads = seg.getPads();

  for (auto _ : state) {

    double sum{0};

    for (int i = 0; i < pads.size(); ++i) {
      sum += seg.x1(i) + seg.x2(i) + seg.y1(i) + seg.y2(i);
    }
    benchmark::DoNotOptimize(sum + 1);

  }
}

BENCHMARK(benchPadAccessThroughVector)->Arg(1000);
BENCHMARK(benchPadAccessThroughInterface)->Arg(1000);

BENCHMARK_MAIN();
