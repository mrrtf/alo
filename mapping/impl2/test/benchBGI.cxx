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
#include "segmentation.h"
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

namespace {
constexpr int NTESTPOINTS{1000};
}

using namespace o2::mch::mapping;
using namespace o2::mch::contour;

using Contours = std::vector<Contour<double>>;
using TestPoints = std::vector<std::pair<double, double>>;

typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::polygon<point, false, true> polygon; // ccw, closed polygon
typedef std::pair<box, unsigned> value;

box pol2bgibox(const Polygon<double> &p)
{
  auto bbox = getBBox(p);
  return box{point(bbox.xmin(),bbox.ymin()),point(bbox.xmax(),bbox.ymax())};
}

auto getRTree(const Contours& contours, int detElemId, bool isBendingPlane)
{
  bgi::rtree<value, bgi::rstar<8,2>> rtree;
  //bgi::rtree<value, bgi::linear<16,1>> rtree;
  //bgi::rtree<value, bgi::quadratic<16,4>> rtree;

  for (auto i = 0; i < contours.size(); ++i) {
    const Contour<double> &c = contours[i];
    for (auto j = 0; j < c.size(); ++j) {
      const Polygon<double> &p = c[j];
      rtree.insert(std::make_pair(pol2bgibox(p), i));
    }
  }
  return rtree;
}

std::vector<point> point2bgi(const std::vector<std::pair<double, double>>& points)
{
  std::vector<point> v;
  for (auto& p: points) {
    v.push_back(point(p.first,p.second));
  }
  return v;
}

static void segmentationList(benchmark::internal::Benchmark *b)
{
  o2::mch::mapping::forOneDetectionElementOfEachSegmentationType([&b](int detElemId) {
    for (auto bending : {true, false}) {
      for (auto extent : {0, 10}) {
        b->Args({detElemId, bending, extent});
      }
    }
  });
}

static void benchImpl0(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);
  int extent = state.range(2);

  auto contours = getSampaContours(Segmentation{detElemId,isBendingPlane});
  auto testPoints = generateTestPoints(NTESTPOINTS, detElemId, extent);

  int nin{0};
  int n{0};

  for (auto _ : state) {
    for (auto &p: testPoints) {
      ++n;
      for (auto &c: contours) {
        if (c.contains(p.first, p.second)) { ++nin; break; }
      }
    }
  }

  state.counters["in"]=100.0*nin/n;
}

static void benchImpl1(benchmark::State &state)
{
  int detElemId = state.range(0);
  bool isBendingPlane = state.range(1);
  int extent = state.range(2);

  Contours contours = getSampaContours(Segmentation{detElemId,isBendingPlane});

  auto testPoints = point2bgi(generateTestPoints(NTESTPOINTS, detElemId, extent));

  auto rtree = getRTree(contours,detElemId, isBendingPlane);

  int nin{0};
  int n{0};
  int nr{0};

  double delta{1E-3};
  for (auto _ : state) {
    for (auto &p: testPoints) {
      ++n;
      std::vector<value> result_n;
      //rtree.query(bgi::covers(p), std::back_inserter(result_n));
      rtree.query(bgi::contains(p), std::back_inserter(result_n));

//      box query_box(
//        point(p.get<0>() - delta, p.get<1>() - delta),
//        point(p.get<0>() + delta, p.get<1>() + delta));
//
//      rtree.query(bgi::contains(query_box), std::back_inserter(result_n));
      nr += result_n.size();
      for (auto &r : result_n) {
        if (contours[r.second].contains(p.get<0>(), p.get<1>())) { ++nin; break; }
      }
    }
  }
  state.counters["in"]=100.0*nin/n;
  state.counters["nresults"]=1.0*nr/nin;
}

BENCHMARK(benchImpl0)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);
BENCHMARK(benchImpl1)->Apply(segmentationList)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN()
