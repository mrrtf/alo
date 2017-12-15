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

#include "generateTestPoints.h"
#include "benchmark/benchmark.h"
#include "segmentationContours.h"
#include "segmentationInterface.h"
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/box.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

namespace {
constexpr int NTESTPOINTS{1000};
}

using namespace o2::mch::mapping;
using namespace o2::mch::contour;

using Contours = std::vector<Contour<double>>;
using TestPoints = std::vector<std::pair<double, double>>;

Contours getSampaContours(int detElemId, bool isBendingPlane)
{
  auto seg = getSegmentation(detElemId, isBendingPlane);
  return getSampaContours(*seg);
}

static void segmentationList(benchmark::internal::Benchmark *b)
{
  for (auto detElemId : getOneDetElemIdPerSegmentation()) {
    for (auto bending : {true, false}) {
      for (auto extent : {0, 10}) {
        b->Args({detElemId, bending, extent});
      }
    }
  }
}

static void benchImpl0(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);
  int extent = state.range(2);

  auto contours = getSampaContours(detElemId, isBendingPlane);
  auto testPoints = generateTestPoints(NTESTPOINTS, detElemId, extent);

  for (auto _ : state) {
    for (auto &p: testPoints) {
      for (auto &c: contours) {
        if (c.contains(p.first, p.second)) { break; }
      }
    }
  }
}

static void benchImpl1(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);
  int extent = state.range(2);
  typedef std::pair<Contour<double>, unsigned> value;

  auto contours = getSampaContours(detElemId, isBendingPlane);
  auto testPoints = generateTestPoints(NTESTPOINTS, detElemId, extent);
  //bgi::rtree<value, bgi::rstar<16, 4> > rtree;

  /*
  for (auto i = 0; i < contours.size(); ++i) {
    rtree.insert(std::make_pair<contours[i], i>);
  }
  */

  for (auto _ : state) {
    for (auto &p: testPoints) {
      for (auto &c: contours) {
        if (c.contains(p.first, p.second)) { break; }
      }
    }
  }
}

BENCHMARK(benchImpl0)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);
BENCHMARK(benchImpl1)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN()
