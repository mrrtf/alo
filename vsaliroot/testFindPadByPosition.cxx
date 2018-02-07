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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "AliMpDDLStore.h"
#include "AliMpDetElement.h"
#include "AliMpManuIterator.h"
#include "AliMpMotif.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPad.h"
#include "AliMpVPadIterator.h"
#include "AliMpVSegmentation.h"
#include "contour.h"
#include "contourCreator.h"
#include "generateTestPoints.h"
#include "mappingFixture.h"
#include "segmentationContours.h"
#include "segmentationSVGWriter.h"
#include "svgWriter.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <segmentation.h>
#include <set>
#include <utility>
#include <vector>

using namespace o2::mch::mapping;
using namespace o2::mch::contour;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentationvsaliroot, MAPPING)

std::string padAsString(const AliMpPad &p)
{
  if (p.IsValid()) {
    return boost::str(boost::format("Pad %10d FEC %4d CH %2d X %7.3f Y %7.3f SX %7.3f SY %7.3f") % -1
                      % p.GetManuId() % p.GetManuChannel() % p.GetPositionX() %
                      p.GetPositionY() % (p.GetDimensionX() * 2.0) % (p.GetDimensionY() * 2.0));
  } else {
    return "invalid pad";
  }
}

// filter out test points to remove those that are close to the pad edges
// (closer to margin, where closer is defined by AliRoot version of the segmentation)
std::vector<std::pair<double, double>> filterTestPoints(const std::vector<std::pair<double, double>> &origTestPoints,
                                                        const AliMpVSegmentation &seg,
                                                        double margin)
{
  std::vector<std::pair<double, double>> testPoints;

  for (auto &tp: origTestPoints) {
    const AliMpPad &pad = seg.PadByPosition(tp.first, tp.second, kFALSE);
    if (pad.IsValid()) {
      double xmin{pad.GetPositionX() - pad.GetDimensionX()};
      double xmax{pad.GetPositionX() + pad.GetDimensionX()};
      double ymin{pad.GetPositionY() - pad.GetDimensionY()};
      double ymax{pad.GetPositionY() + pad.GetDimensionY()};
      if (
        tp.first > (xmin + margin) &&
        tp.first<(xmax - margin) &&
                 tp.second>(ymin + margin) &&
        tp.second < (ymax - margin)) {
        testPoints.push_back(tp);
      }
    }
  }
  return testPoints;
}

std::vector<std::pair<double, double>> boxTestPoints(const std::vector<std::pair<double, double>> &testPoints,
                                                     double xmin, double ymin, double xmax, double ymax)
{
  std::vector<std::pair<double, double>> filtered;

  for (const auto &tp: testPoints) {
    if (tp.first > xmin && tp.first < xmax && tp.second > ymin && tp.second < ymax) {
      filtered.push_back(tp);
    }
  }

  return filtered;
}

BBox<double> getBBox(const std::vector<std::pair<double, double>> &points)
{
  double xmin{std::numeric_limits<double>::max()};
  double xmax{std::numeric_limits<double>::lowest()};
  double ymin{std::numeric_limits<double>::max()};
  double ymax{std::numeric_limits<double>::lowest()};

  for (const auto &p: points) {
    xmin = std::min(xmin, p.first);
    xmax = std::max(xmax, p.first);
    ymin = std::min(ymin, p.second);
    ymax = std::max(ymax, p.second);
  }
  return {
    xmin - 5.0, ymin - 5.0, xmax + 5.0, ymax + 5.0
  };
}

SVGWriter writeSegmentation(const Segmentation &o2Seg, const BBox<double> &box)
{
  SVGWriter w(box);

  w.addStyle(svgSegmentationDefaultStyle());

  w.addStyle(R"(
.diffpoints {
  fill:none;
  stroke-width:0.25px;
  stroke: red;
  opacity: 0.25;
}
.usedtestpoints {
  fill:none;
  stroke-width:0.01px;
  stroke: red;
}
)");

  svgSegmentation(o2Seg, w, true, true, true, false);

  return w;
}

bool checkHasPadByPosition(int detElemId, const AliMpVSegmentation &alSeg,
                           const Segmentation &o2Seg,
                           const std::vector<std::pair<double, double>> &testPoints)
{
  SVGWriter w = writeSegmentation(o2Seg, getBBox(testPoints));

  std::vector<std::pair<double, double>> diffPoints;

  for (auto i = 0; i < testPoints.size(); ++i) {
    double x = testPoints[i].first;
    double y = testPoints[i].second;
    const AliMpPad alPad = alSeg.PadByPosition(x, y, kFALSE);
    int paduid = o2Seg.findPadByPosition(x, y);
    if (!comparePads(alPad, o2Seg, paduid)) {
      diffPoints.push_back(testPoints[i]);
#if 0
      std::cout
        << boost::format("\nDIFF for x=%7.3f y=%7.3f\n") % x % y;
      std::cout << "    " << padAsString(alPad) << "\n";
      std::cout << "    " << padAsString(o2Seg, paduid);
#endif
    }
  }

  if (diffPoints.empty()) {
    return true;

  }

  w.svgGroupStart("diffpoints");
  w.points(diffPoints);
  w.svgGroupEnd();

  bool isBendingPlane = o2Seg.isBendingPlane();
  std::ostringstream filename;
  filename << "bug-" << detElemId << "-" << (isBendingPlane ? "B" : "NB") << ".html";
  std::ofstream out(filename.str());
  w.writeHTML(out);

  return false;
}

std::vector<std::pair<double, double>>
generateManyTestPoints(const AliMpVSegmentation &alSeg, double step, int ntimes)
{
  const double offset{1}; // cm
  const double margin{0.01}; // margin in cm. Do not go below 100 micrometers, there's no point.
  int extent{0}; // -1 for fixed matrix, 0 for flat, > 0 for gaussian

  double xmin{alSeg.GetPositionX() - alSeg.GetDimensionX() - offset};
  double ymin{alSeg.GetPositionY() - alSeg.GetDimensionY() - offset};
  double xmax{alSeg.GetPositionX() + alSeg.GetDimensionX() + offset};
  double ymax{alSeg.GetPositionY() + alSeg.GetDimensionY() + offset};

  std::vector<std::pair<double, double>> testPoints;

  for (double x = xmin; x < xmax; x += step) {
    for (double y = ymin; y < ymax; y += step) {
      auto origTestPoints = boxTestPoints(generateTestPoints(ntimes, x, y, x + step, y + step, extent), xmin, ymin,
                                          xmax, ymax);
      auto filteredTestPoints = filterTestPoints(origTestPoints, alSeg, margin);
      std::copy(filteredTestPoints.begin(), filteredTestPoints.end(), std::back_inserter(testPoints));
    }
  }

  return testPoints;
}

std::vector<std::pair<double, double>>
generateOneTestPointPerPad(const AliMpVSegmentation &alSeg)
{
  std::vector<std::pair<double, double>> testPoints;
  std::unique_ptr<AliMpVPadIterator> it{alSeg.CreateIterator()};
  it->First();
  while (!it->IsDone()) {
    const AliMpPad &p = it->CurrentItem();
    testPoints.push_back({p.GetPositionX(), p.GetPositionY()});
    it->Next();
  }
  return testPoints;
}

BOOST_AUTO_TEST_CASE(One)
{
  int detElemId{100};
  bool isBendingPlane{true};
  Segmentation seg{detElemId, isBendingPlane};
  auto box{enlarge(getBBox(seg), 10.0, 2.0)};

  std::vector<std::pair<double, double>> testPoints;
  std::vector<int> pads;

#if 0
  for (double x = 1.3; x < 1.6; x += 0.1) {
    for (double y = box.ymin(); y < box.ymax(); y += 0.25) {
      testPoints.push_back({x, y});
      pads.push_back(seg.findPadByPosition(x, y));
    }
  }
#endif
  double y = 18.27;
  for (double x = box.xmin(); x < box.xmax(); x += 0.25) {
    testPoints.push_back({x, y});
    pads.push_back(seg.findPadByPosition(x, y));
  }

  std::vector<std::pair<double, double>> there;
  std::vector<std::pair<double, double>> notthere;
  for (auto i = 0; i < testPoints.size(); ++i) {
    if (seg.isValid(pads[i])) {
      there.push_back(testPoints[i]);
      //there.push_back({seg.padPositionX(pads[i]),seg.padPositionY(pads[i])});
      std::cout << boost::format("x %7.2f") % (seg.padPositionX(pads[i]) - seg.padSizeX(pads[i]) / 2.0) << "\n";
    } else {
      notthere.push_back(testPoints[i]);
    }
  }

  SVGWriter w(box);

  svgSegmentation(seg, w, true, true, true, false);
  w.addStyle(svgSegmentationDefaultStyle());
  w.addStyle(R"(
.there {
  fill:none;
  stroke-width:0.01px;
  stroke: green;
}
.notthere {
  fill:none;
  stroke-width:0.01px;
  stroke: pink;
}
)");

  w.svgGroupStart("there");
  w.points(there);
  w.svgGroupEnd();

  w.svgGroupStart("notthere");
  w.points(notthere);
  w.svgGroupEnd();

  std::ofstream out("one.html");
  w.writeHTML(out);

  BOOST_TEST(false);
}

bool SamePadsRandom(AliMpSegmentation *mseg, int detElemId, bool isBendingPlane)
{
  double step{1}; // cm
  int ntimes{10};
  auto alSeg = alirootSegmentation(mseg, detElemId, isBendingPlane);
  Segmentation o2Seg{detElemId, isBendingPlane};
  return checkHasPadByPosition(detElemId, *alSeg, o2Seg,
                               generateManyTestPoints(*alSeg, step, ntimes));
}

bool SamePadsSingle(AliMpSegmentation *mseg, int detElemId, bool isBendingPlane)
{
  auto alSeg = alirootSegmentation(mseg, detElemId, isBendingPlane);
  Segmentation o2Seg{detElemId, isBendingPlane};
  return checkHasPadByPosition(detElemId, *alSeg, o2Seg,
                               generateOneTestPointPerPad(*alSeg));
}

BOOST_DATA_TEST_CASE(SamePadsRandomBending,
                     boost::unit_test::data::make(MAPPING::detElemIds),
                     detElemId)
{
  BOOST_TEST(SamePadsRandom(mseg, detElemId, true));
}

BOOST_DATA_TEST_CASE(SamePadsRandomNonBending,
                     boost::unit_test::data::make(MAPPING::detElemIds),
                     detElemId)
{
  BOOST_TEST(SamePadsRandom(mseg, detElemId, false));
}

BOOST_DATA_TEST_CASE(SamePadsSingleBending,
                     boost::unit_test::data::make(MAPPING::detElemIds),
                     detElemId)
{
  BOOST_TEST(SamePadsSingle(mseg, detElemId, true));
}

BOOST_DATA_TEST_CASE(SamePadsSingleNonBending,
                     boost::unit_test::data::make(MAPPING::detElemIds),
                     detElemId)
{
  BOOST_TEST(SamePadsSingle(mseg, detElemId, false));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
