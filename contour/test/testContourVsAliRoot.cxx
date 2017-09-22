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
#include "contour.h"
#include "svg.h"
#include <TArrayD.h>
#include <boost/format.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
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

Polygon<double> convert(const AliMUONPolygon& polygon)
{
  Polygon<double> p;

  for (int i = 0; i < polygon.NumberOfVertices(); ++i) {
    p.push_back({polygon.X(i), polygon.Y(i)});
  }

  return p;
}

PolygonCollection<double> convert(const TObjArray& polygons)
{
  TIter next(&polygons);
  AliMUONPolygon* polygon;

  PolygonCollection<double> pc;

  while ((polygon = static_cast<AliMUONPolygon*>(next()))) {
    if (polygon->NumberOfVertices() > 2) {
      pc.push_back(convert(*polygon));
    };
  }
  return pc;
}

PolygonCollection<double> createAliRootContour(int detElemId, int manuId)
{
  AliMUONManuContourMaker contourMaker(nullptr);
  std::unique_ptr<AliMUONContour> contour(contourMaker.CreateManuContour(detElemId, manuId));
  return convert(*(contour->Polygons()));
}

PolygonCollection<double> createManuPads(AliMpSegmentation* mseg, int detElemId, int manuId)
{
  const AliMpVSegmentation* seg = mseg->GetMpSegmentationByElectronics(detElemId, manuId);
  AliMpMotifPosition* pos = seg->MotifPosition(manuId);
  AliMpVMotif* motif = pos->GetMotif();
  AliMpMotifType* motifType = motif->GetMotifType();

  PolygonCollection<double> pads;

  auto xoffset = pos->GetPositionX() - seg->GetPositionX();
  auto yoffset = pos->GetPositionY() - seg->GetPositionY();

  if (motifType->IsFull()) {
    auto dx = motif->DimensionX();
    auto dy = motif->DimensionY();
    pads.push_back({{xoffset - dx, yoffset - dy},
                    {xoffset + dx, yoffset - dy},
                    {xoffset + dx, yoffset + dy},
                    {xoffset - dx, yoffset + dy},
                    {xoffset - dx, yoffset - dy}});

    return pads;
  }

  for (Int_t i = 0; i <= 64; ++i) {
    AliMpConnection* connection = motifType->FindConnectionByGassiNum(i);

    if (connection) {
      Int_t ix = connection->GetLocalIx();
      Int_t iy = connection->GetLocalIy();

      Double_t x, y, dx, dy;

      motif->GetPadDimensionsByIndices(ix, iy, dx, dy);
      motif->PadPositionLocal(ix, iy, x, y);

      x += xoffset;
      y += yoffset;

      Polygon<double> pad{
        {x - dx, y - dy},
        {x + dx, y - dy},
        {x + dx, y + dy},
        {x - dx, y + dy},
        {x - dx, y - dy}
      };

      pads.push_back(pad);
    }
  }
  return pads;
}

BOOST_FIXTURE_TEST_SUITE(aliroot_mch_contour, MAPPING)

BOOST_AUTO_TEST_CASE(AliRootGetYPositions)
{
  AliMUONContourMaker maker;

  TObjArray polygonVerticalEdges(kTRUE);
  TArrayD yPositions;

  polygonVerticalEdges.AddLast(new AliMUONSegment(0, 2, 0, 3));
  polygonVerticalEdges.AddLast(new AliMUONSegment(1, 2, 1, 3));
  polygonVerticalEdges.AddLast(new AliMUONSegment(2, 2, 2, 4));
  polygonVerticalEdges.AddLast(new AliMUONSegment(3, 2, 3, 4));
  polygonVerticalEdges.AddLast(new AliMUONSegment(3, 5, 3, 6));

  maker.GetYPositions(polygonVerticalEdges, yPositions);

  std::vector<double> ypos{yPositions.GetArray(), yPositions.GetArray() + yPositions.GetSize()};
  const std::vector<double> expected{2, 3, 4, 5, 6};
  BOOST_TEST(ypos == expected);
}


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

  PolygonCollection<double> expected{
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

PolygonCollection<double> createO2Contour(AliMpSegmentation* mseg, int detElemId, int manuId)
{
  auto pads = createManuPads(mseg, detElemId, manuId);
  return createContour(pads);
}

std::pair<PolygonCollection<double>, PolygonCollection<double>>
createContours(AliMpSegmentation* mseg, int detElemId, int manuId)
{
  PolygonCollection<double> fromAliRoot = createAliRootContour(detElemId, manuId);
  PolygonCollection<double> fromO2 = createO2Contour(mseg, detElemId, manuId);
  return {fromAliRoot, fromO2};
}

bool areTheSame(const std::pair<PolygonCollection<double>, PolygonCollection<double>>& contours)
{
  if (contours.first == contours.second) {
    return true;
  } else {
    std::cout << "FromAliRoot: " << contours.first.size() << "\n" << contours.first << '\n';
    std::cout << "FromO2: " << contours.second.size() << "\n" << contours.second << '\n';
    std::cout << '\n';
    std::cout << getSortedVertices(contours.first).size() << " " << getSortedVertices(contours.first) << '\n';
    std::cout << getSortedVertices(contours.second).size() << " " << getSortedVertices(contours.second) << '\n';
    basicSVG("fromaliroot.svg", contours.first);
    basicSVG("fromo2.svg", contours.second);
    return false;
  }
}

BOOST_AUTO_TEST_CASE(ContourForMotifTypeE15IsDisjoint)
{
  BOOST_TEST(areTheSame(createContours(mseg, 1001, 408)));
}

BOOST_AUTO_TEST_CASE(SomeExamplesOfNormalContours)
{
  BOOST_TEST(areTheSame(createContours(mseg, 601, 7)));
  BOOST_TEST(areTheSame(createContours(mseg, 505, 8)));
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
  BOOST_TEST(isCounterClockwiseOriented(contour));
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

BOOST_AUTO_TEST_CASE(CreateO2ContourWithOneCommonVertex)
{
  std::vector<Polygon<double>> input{
    {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}},
    {{0, 1}, {1, 1}, {1, 2}, {0, 2}, {0, 1}},
    {{1, 2}, {2, 2}, {2, 3}, {1, 3}, {1, 2}},
    {{1, 3}, {2, 3}, {2, 4}, {1, 4}, {1, 3}}
  };

  auto contour = createContour(input);

  PolygonCollection<double> expected{
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

  PolygonCollection<double> expected{
    {{0, 2}, {0, 0}, {1, 0}, {1, 2}, {0, 2}},
    {{1, 4}, {1, 2}, {2, 2}, {2, 4}, {1, 4}}
  };

  BOOST_CHECK(c == expected);
}

BOOST_AUTO_TEST_SUITE(TimeContourCreation, *boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE(TimeCreationOfAllAliRootContours)
{
  auto start = std::chrono::high_resolution_clock::now();

  AliMpManuIterator it;

  int detElemId, manuId;

  std::vector<PolygonCollection<double>> contours(16828);
  int i{0};
  while (it.Next(detElemId, manuId)) {
    contours[i++] = createAliRootContour(detElemId, manuId);
  }

  auto stop = std::chrono::high_resolution_clock::now();

  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

  std::cout << "AliRoot: " << t << " ms to build " << contours.size() << " manu contours\n";

  BOOST_CHECK(t < 5200);
}

BOOST_AUTO_TEST_CASE(TimeCreationOfAllO2Contours)
{
  auto start = std::chrono::high_resolution_clock::now();

  AliMpManuIterator it;

  int detElemId, manuId;

  std::vector<PolygonCollection<double>> pads;

  while (it.Next(detElemId, manuId)) {
    pads.push_back(createManuPads(mseg, detElemId, manuId));
  }

  std::cout << "O2: " << std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::high_resolution_clock::now() - start).count()
            << " ms to build pads\n";

  std::vector<PolygonCollection<double>> contours(16828);

  start = std::chrono::high_resolution_clock::now();

  it.Reset();
  int i{0};
  while (it.Next(detElemId, manuId)) {
    contours[i] = createContour(pads[i]);
    ++i;
  }

  auto stop = std::chrono::high_resolution_clock::now();

  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

  std::cout << "O2: " << t << " ms to build " << contours.size() << " manu contours\n";

  BOOST_CHECK(t < 5200);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


