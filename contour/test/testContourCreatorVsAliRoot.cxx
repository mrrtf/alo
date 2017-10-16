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
#include "contourCreator.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <utility>

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
    p.addVertex({polygon.X(i), polygon.Y(i)});
  }

  return p;
}

Contour<double> convert(const TObjArray& polygons)
{
  TIter next(&polygons);
  AliMUONPolygon* polygon;

  Contour<double> pc;

  while ((polygon = static_cast<AliMUONPolygon*>(next()))) {
    if (polygon->NumberOfVertices() > 2) {
      pc.addPolygon(convert(*polygon));
    };
  }
  return pc;
}

Contour<double> createAliRootContour(int detElemId, int manuId)
{
  AliMUONManuContourMaker contourMaker(nullptr);
  std::unique_ptr<AliMUONContour> contour(contourMaker.CreateManuContour(detElemId, manuId));
  return convert(*(contour->Polygons()));
}

std::vector<Polygon<double>> createManuPads(AliMpSegmentation* mseg, int detElemId, int manuId)
{
  const AliMpVSegmentation* seg = mseg->GetMpSegmentationByElectronics(detElemId, manuId);
  AliMpMotifPosition* pos = seg->MotifPosition(manuId);
  AliMpVMotif* motif = pos->GetMotif();
  AliMpMotifType* motifType = motif->GetMotifType();

  std::vector<Polygon<double>> pads;

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

Contour<double> createO2Contour(AliMpSegmentation* mseg, int detElemId, int manuId)
{
  auto pads = createManuPads(mseg, detElemId, manuId);
  return createContour(pads);
}

std::pair<Contour<double>, Contour<double>>
createContours(AliMpSegmentation* mseg, int detElemId, int manuId)
{
  Contour<double> fromAliRoot = createAliRootContour(detElemId, manuId);
  Contour<double> fromO2 = createO2Contour(mseg, detElemId, manuId);
  return {fromAliRoot, fromO2};
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
  std::uniform_real_distribution<double> distX{bbox.xmin(), bbox.xmax()};
  std::uniform_real_distribution<double> distY{bbox.ymin(), bbox.ymax()};

  std::generate(testPoints.begin(), testPoints.end(),
                [&distX, &distY, &mt] { return std::make_pair<double, double>(distX(mt), distY(mt)); });

  int nok{0};

  for (const auto& tp: testPoints) {

    bool alirootInside = alirootContour->IsInside(tp.first, tp.second);
    bool o2Inside = o2Contour.isInside(tp.first, tp.second);
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

double area1(const Polygon<double>& p)
{
  auto vertices = p.getVertices();
  double a{0.0};
  for (auto i = 0; i < vertices.size() - 1; ++i) {
    const auto& current = vertices[i];
    const auto& next = vertices[i + 1];
    a += current.x * next.y - current.y * next.x;
  }
  return a / 2.0;
}

double area1(const Contour<double>& c)
{
  double a{0.0};
  for (auto i = 0; i < c.size(); ++i) {
    a += area1(c[i]);
  }
  return a;
}

double area2(const Polygon<double>& p)
{
  auto vertices = p.getVertices();
  double a{0.0};
  for (auto i = 0; i < vertices.size() - 1; ++i) {
    const auto& current = vertices[i];
    const auto& next = vertices[i + 1];
    a += (current.x + next.x) * (next.y - current.y);
  }
  return a / 2.0;
}

double area2(const Contour<double>& c)
{
  double a{0.0};
  for (auto i = 0; i < c.size(); ++i) {
    a += area2(c[i]);
  }
  return a;
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

std::vector<std::vector<Polygon<double>>> getPads(AliMpSegmentation* mseg)
{
  AliMpManuIterator it;

  int detElemId, manuId;

  std::vector<std::vector<Polygon<double>>> pads;

  while (it.Next(detElemId, manuId)) {
    pads.push_back(createManuPads(mseg, detElemId, manuId));
  }
  return pads;

}

std::vector<Contour<double>> getContours(const std::vector<std::vector<Polygon<double>>>& pads)
{
  std::vector<Contour<double>> contours(pads.size());

  for (auto i = 0; i < pads.size(); ++i) {
    contours[i] = createContour(pads[i]);
  }
  return contours;
}

BOOST_AUTO_TEST_SUITE(TimeContourCreation, *boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE(TimeCreationOfAllAliRootContours)
{
  auto start = std::chrono::high_resolution_clock::now();

  AliMpManuIterator it;

  int detElemId, manuId;

  std::vector<Contour<double>> contours(16828);
  int i{0};
  while (it.Next(detElemId, manuId)) {
    contours[i++] = createAliRootContour(detElemId, manuId);
  }

  auto stop = std::chrono::high_resolution_clock::now();

  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

  std::cout << "AliRoot: " << t << " ms to build " << contours.size() << " manu contours\n";

  BOOST_CHECK(t < 2000);
}

BOOST_AUTO_TEST_CASE(TimeCreationOfAllO2Contours)
{
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<std::vector<Polygon<double>>> pads = getPads(mseg);

  std::cout << "O2: " << std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::high_resolution_clock::now() - start).count()
            << " ms to build pads\n";

  start = std::chrono::high_resolution_clock::now();

  std::vector<Contour<double>> contours = getContours(pads);

  auto stop = std::chrono::high_resolution_clock::now();

  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

  std::cout << "O2: " << t << " ms to build " << contours.size() << " manu contours\n";

  BOOST_CHECK(t < 700);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TimePolygonArea, *boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE(TimePolygonArea1)
{
  auto contours = getContours(getPads(mseg));
  auto start = std::chrono::high_resolution_clock::now();
  for (auto i = 0; i < 100; ++i) {
    for (auto& c: contours) {
      area1(c);
    }
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
  std::cout << "area1 " << t << " ms \n";
  BOOST_CHECK(t < 1000);
}

BOOST_AUTO_TEST_CASE(TimePolygonArea2)
{
  auto contours = getContours(getPads(mseg));
  auto start = std::chrono::high_resolution_clock::now();
  for (auto i = 0; i < 100; ++i) {
    for (auto& c: contours) {
      area2(c);
    }
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
  std::cout << "area2 " << t << " ms \n";
  BOOST_CHECK(t < 1000);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE_END()


