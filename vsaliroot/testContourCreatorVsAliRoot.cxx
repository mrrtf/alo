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

#include "AliMUONContour.h"
#include "AliMUONContourMaker.h"
#include "AliMUONManuContourMaker.h"
#include "AliMUONPolygon.h"
#include "AliMUONSegment.h"
#include "AliMpConnection.h"
#include "AliMpDDLStore.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpManuIterator.h"
#include "AliMpManuIterator.h"
#include "AliMpMotif.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include "commonContour.h"
#include "contourCreator.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

constexpr int NLOOP = 1;

using namespace o2::mch::contour;

struct MAPPING
{
    MAPPING()
    {
      if (!ddlStore) {
        AliMpDataProcessor mp;
        AliMpDataMap* dataMap = mp.CreateDataMap("data");
        AliMpDataStreams dataStreams(dataMap);
        ddlStore = AliMpDDLStore::ReadData(dataStreams);
        mseg = AliMpSegmentation::Instance();
      }
    }

    static AliMpDDLStore* ddlStore;
    static AliMpSegmentation* mseg;
};

AliMpDDLStore* MAPPING::ddlStore{nullptr};
AliMpSegmentation* MAPPING::mseg{nullptr};

BOOST_FIXTURE_TEST_SUITE(aliroot_mch_contour, MAPPING)

BOOST_AUTO_TEST_CASE(AliRootCreateContour)
{
  TObjArray pads(kTRUE);

  pads.AddLast(new AliMUONPolygon(0.5, 0.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(0.5, 1.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(1.5, 0.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(1.5, 1.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(1.5, 2.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(1.5, 3.5, 0.5, 0.5));

  AliMUONContourMaker maker;

  AliMUONContour* contour = maker.CreateContour(pads);

  auto c = convert(*(contour->Polygons()));

  Contour<double> expected{
    {
      {0, 2},
      {0, 0},
      {2, 0},
      {2, 4},
      {1, 4},
      {1, 2},
      {0, 2}
    }
  };
  BOOST_CHECK(c == expected);
}

BOOST_AUTO_TEST_CASE(O2CreateContour)
{
  std::vector<Polygon<double>> pads;

  pads.push_back({{0.0, 0.0},
                  {1.0, 0.0},
                  {1.0, 1.0},
                  {0.0, 1.0},
                  {0.0, 0.0}});
  pads.push_back({{0.0, 1.0},
                  {1.0, 1.0},
                  {1.0, 2.0},
                  {0.0, 2.0},
                  {0.0, 1.0}});
  pads.push_back({{1.0, 0.0},
                  {2.0, 0.0},
                  {2.0, 1.0},
                  {1.0, 1.0},
                  {1.0, 0.0}});
  pads.push_back({{1.0, 1.0},
                  {2.0, 1.0},
                  {2.0, 2.0},
                  {1.0, 2.0},
                  {1.0, 1.0}});
  pads.push_back({{1.0, 2.0},
                  {2.0, 2.0},
                  {2.0, 3.0},
                  {1.0, 3.0},
                  {1.0, 2.0}});
  pads.push_back({{1.0, 3.0},
                  {2.0, 3.0},
                  {2.0, 4.0},
                  {1.0, 4.0},
                  {1.0, 3.0}});

  auto c = createContour(pads);

  Contour<double> expected{
    {
      {0, 2},
      {0, 0},
      {2, 0},
      {2, 4},
      {1, 4},
      {1, 2},
      {0, 2}
    }
  };
  BOOST_CHECK(c == expected);
}


bool areTheSame(const std::pair<Contour<double>, Contour<double>>& contours)
{
  if (contours.first == contours.second) {
    return true;
  } else {
    std::cout << "FromAliRoot: " << contours.first.size() << "\n" << contours.first << '\n';
    std::cout << "FromO2: " << contours.second.size() << "\n" << contours.second << '\n';
    std::cout << '\n';
    std::cout << contours.first.getSortedVertices().size() << " " << contours.first.getSortedVertices() << '\n';
    std::cout << contours.second.getSortedVertices().size() << " " << contours.second.getSortedVertices() << '\n';
    //basicSVG("fromaliroot.svg", contours.first);
    //basicSVG("fromo2.svg", contours.second);
    return false;
  }
}

//BOOST_AUTO_TEST_CASE(ContourForMotifTypeE15IsDisjoint)
//{
//  BOOST_TEST(areTheSame(createContours(mseg, 1001, 408)));
//}

BOOST_AUTO_TEST_CASE(FirstExampleOfNormalContour)
{
  BOOST_TEST(areTheSame(createContours(mseg, 505, 8)));
}

BOOST_AUTO_TEST_CASE(SecondExampleOfNormalContour)
{
  BOOST_TEST(areTheSame(createContours(mseg, 601, 7)));
}

BOOST_AUTO_TEST_CASE(ThirdExampleOfNormalContour)
{
  BOOST_TEST(areTheSame(createContours(mseg, 501, 407)));
}

BOOST_AUTO_TEST_CASE(DE716MANU1135)
{
  BOOST_TEST(areTheSame(createContours(mseg, 716, 1135)));
}

BOOST_AUTO_TEST_CASE(DE701MANU407VsAliRoot)
{
  BOOST_TEST(areTheSame(createContours(mseg, 701, 407)));
}

BOOST_AUTO_TEST_CASE(IsContourDE701MANU407CounterClockwiseOriented)
{
  auto contour = createO2Contour(mseg, 701, 407);
  BOOST_TEST(contour.isCounterClockwiseOriented());
}

BOOST_AUTO_TEST_CASE(TwoDifferentContours)
{
  auto c1 = createContours(mseg, 601, 7);
  auto c2 = createContours(mseg, 1001, 408);
  BOOST_TEST(c1.second != c2.second);
}

BOOST_AUTO_TEST_CASE(AllManuContoursMustBeTheSameWhateverTheCreateContourMethod)
{
  AliMpManuIterator it;
  int detElemId, manuId;

  try {
    while (it.Next(detElemId, manuId)) {
      BOOST_TEST_CHECK(areTheSame(createContours(mseg, detElemId, manuId)),
                       " problem with de " << detElemId << " manu " << manuId);
    }
  }
  catch (std::exception& err) {
    std::cout << "problem with de " << detElemId << " manu " << manuId << '\n';
    std::cout << err.what() << '\n';
  }
}

bool isInsideCheck(AliMUONContour* alirootContour, const Contour<double>& o2Contour, int n)
{
  // check that for a large range of points, the isInside of O2 is giving back the same
  // answer as the AliRoot one
  std::vector<std::pair<double, double>> testPoints(n);

  // generate n testpoints within bounding box +- offset cm
  double offset{10.0};

  auto bbox = getBBox(o2Contour);

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distX{bbox.xmin()-offset, bbox.xmax()+offset};
  std::uniform_real_distribution<double> distY{bbox.ymin()-offset, bbox.ymax()+offset};

  std::generate(testPoints.begin(), testPoints.end(),
                [&distX, &distY, &mt] { return std::make_pair<double, double>(distX(mt), distY(mt)); });

  int nok{0};

  for (const auto& tp: testPoints) {

    bool alirootInside = alirootContour->IsInside(tp.first, tp.second);
    bool o2Inside = o2Contour.contains(tp.first, tp.second);
    if (alirootInside != o2Inside) {
      std::cout << "Point " << tp.first << "," << tp.second << " is aliroot:" << alirootInside << " o2: " << o2Inside
                << '\n';
      ++nok;
    }
  }
  return nok==0;
}

BOOST_AUTO_TEST_CASE(IsInsideMustBehaveTheSameInAliRootAndInO2Polygons)
{
  AliMpManuIterator it;
  int detElemId, manuId;
  AliMUONManuContourMaker contourMaker(nullptr);

  try {
    while (it.Next(detElemId, manuId)) {
      std::unique_ptr<AliMUONContour> aliroot(contourMaker.CreateManuContour(detElemId, manuId));
      auto o2 = createO2Contour(mseg, detElemId, manuId);
      BOOST_TEST_CHECK(isInsideCheck(aliroot.get(), o2, 100),
                       " problem with de " << detElemId << " manu " << manuId);
    }
  }
  catch (std::exception& err) {
    std::cout << "problem with de " << detElemId << " manu " << manuId << '\n';
    std::cout << err.what() << '\n';
  }
}

BOOST_AUTO_TEST_CASE(CreateO2ContourWithOneCommonVertex)
{
  std::vector<Polygon<double>> input{
    {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}},
    {{0, 1}, {1, 1}, {1, 2}, {0, 2}, {0, 1}},
    {{1, 2}, {2, 2}, {2, 3}, {1, 3}, {1, 2}},
    {{1, 3}, {2, 3}, {2, 4}, {1, 4}, {1, 3}}
  };

  auto contour = createContour(input);

  Contour<double> expected{
    {{0, 2}, {0, 0}, {1, 0}, {1, 2}, {0, 2}},
    {{1, 4}, {1, 2}, {2, 2}, {2, 4}, {1, 4}}
  };

  BOOST_CHECK(contour == expected);
}

BOOST_AUTO_TEST_CASE(CreateAliRootContourWithOneCommonVertex)
{
  TObjArray pads(kTRUE);

  pads.AddLast(new AliMUONPolygon(0.5, 0.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(0.5, 1.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(1.5, 2.5, 0.5, 0.5));
  pads.AddLast(new AliMUONPolygon(1.5, 3.5, 0.5, 0.5));

  AliMUONContourMaker maker;

  AliMUONContour* contour = maker.CreateContour(pads);

  auto c = convert(*(contour->Polygons()));

  Contour<double> expected{
    {{0, 2}, {0, 0}, {1, 0}, {1, 2}, {0, 2}},
    {{1, 4}, {1, 2}, {2, 2}, {2, 4}, {1, 4}}
  };

  BOOST_CHECK(c == expected);
}


BOOST_AUTO_TEST_CASE(PolygonArea1, *boost::unit_test::tolerance(1E-6))
{
  Polygon<double> testPolygon{
    {{0.1, 0.1}, {1.1, 0.1}, {1.1, 1.1}, {2.1, 1.1}, {2.1, 3.1}, {1.1, 3.1}, {1.1, 2.1}, {0.1, 2.1}, {0.1, 0.1}}};
  BOOST_CHECK(testPolygon.signedArea() == area1(testPolygon));
}

BOOST_AUTO_TEST_CASE(PolygonArea2, *boost::unit_test::tolerance(1E-6))
{
  Polygon<double> testPolygon{
    {{0.1, 0.1}, {1.1, 0.1}, {1.1, 1.1}, {2.1, 1.1}, {2.1, 3.1}, {1.1, 3.1}, {1.1, 2.1}, {0.1, 2.1}, {0.1, 0.1}}};
  BOOST_TEST(testPolygon.signedArea() == area2(testPolygon));
}

BOOST_AUTO_TEST_SUITE_END()


