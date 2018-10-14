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

#include "MCHContour/ContourCreator.h"
#include "benchmark/benchmark.h"
#include "AliMpManuIterator.h"
#include "CommonContour.h"
#include "CommonBench.h"

using namespace o2::mch::contour;

BENCHMARK_DEFINE_F(BenchAliRoot, benchContourAliRoot)(benchmark::State &state)
{
  for (auto _ : state) {
    AliMpManuIterator it;

    int detElemId, manuId;

    std::vector<Contour<double>>
      contours(16828);
    int i{0};
    while (it.Next(detElemId, manuId)) {
      contours[i++] = createAliRootContour(detElemId, manuId);
    }
  }
}

BENCHMARK_DEFINE_F(BenchAliRoot, benchContourO2pads)(benchmark::State &state)
{
  for (auto _:state) {
    std::vector<std::vector<Polygon<double>>> pads = getPads(mseg);
  }
}

BENCHMARK_DEFINE_F(BenchAliRoot, benchContourO2contours)(benchmark::State &state)
{
  for (auto _: state) {
    std::vector<std::vector<Polygon<double>>> pads = getPads(mseg);
    std::vector<Contour<double>> contours = getContours(pads);
  }
}

BENCHMARK_DEFINE_F(BenchAliRoot, benchPolygonArea1)(benchmark::State &state)
{
  auto contours = getContours(getPads(mseg));
  for (auto _: state) {
    for (auto &c: contours) {
      area1(c);
    }
  }
}

BENCHMARK_DEFINE_F(BenchAliRoot, benchPolygonArea2)(benchmark::State &state)
{
  auto contours = getContours(getPads(mseg));
  for (auto _: state) {
    for (auto &c: contours) {
      area2(c);
    }
  }
}

BENCHMARK_DEFINE_F(BenchAliRoot, benchGetYPosition)(benchmark::State &state)
{
  auto contours = getContours(getPads(mseg));

  for (auto _: state) {
    for (auto &c: contours) {
      std::vector<Polygon<double>> polygons;
      for ( auto i = 0; i < c.size(); ++i ) {
        polygons.push_back(c[i]);
      }
      auto ypos = impl::getYPositions(polygons);
    }
  }
}
BENCHMARK_REGISTER_F(BenchAliRoot, benchGetYPosition)->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(BenchAliRoot, benchContourAliRoot)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchAliRoot, benchContourO2contours)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchAliRoot, benchContourO2pads)->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(BenchAliRoot, benchPolygonArea1)->Unit(benchmark::kMicrosecond);;
BENCHMARK_REGISTER_F(BenchAliRoot, benchPolygonArea2)->Unit(benchmark::kMicrosecond);;

BENCHMARK_MAIN();
