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

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <chrono>
#include <iostream>
#include "AliMUONContour.h"
#include "AliMUONManuContourMaker.h"
#include "AliMpMotifPosition.h"
#include "AliMpSegmentation.h"
#include "AliMpSlat.h"
#include "AliMpSlatSegmentation.h"
#include "mapping.h"
#include "motifPosition.h"
#include "seg.h"
#include "segnumbers.h"
#include "AliMpMotifType.h"
#include "AliMpConnection.h"
#include "AliMpManuIterator.h"
#include <vector>
#include <AliMpManuIterator.h>
#include <boost/format.hpp>
#include <AliMpMotif.h>
#include "AliMUONPolygon.h"
#include "contour.h"
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/io/wkt/write.hpp>
#include <boost/geometry/io/wkt/read.hpp>
#include <boost/geometry/io/svg/write.hpp>
#include <boost/geometry/algorithms/union.hpp>
#include <fstream>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/geometries/box.hpp>
#include "svg.h"
#include <boost/geometry/geometries/linestring.hpp>
#include <AliMUONContourMaker.h>
#include <TArrayD.h>
#include <AliMUONSegment.h>

constexpr int NLOOP = 1;

using namespace o2::mch::geometry;
namespace bg = boost::geometry;

struct CONTOURS
{

    CONTOURS()
    {
      std::vector<std::string> segnames = get_all_segmentation_names(m.ddlStore(), m.mseg());
      std::vector<std::pair<std::vector<AliMpMotifPosition*>, std::vector<AliMpMotifPosition*>>> mp = get_motifpositions(
        m.ddlStore(), m.mseg(), segnames);
      for (auto& p:mp) {
        for (auto& pos: p.first) {
          motifPositions.push_back(pos);
        }
        for (auto& pos: p.second) {
          motifPositions.push_back(pos);
        }
      }
    }

    Mapping m;
    std::vector<AliMpMotifPosition*> motifPositions;
};

struct POLYGONS
{
    POLYGONS()
    {
      testPads.push_back({{{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0}}});
      testPads.push_back({{{1.0, 3.0}, {2.0, 3.0}, {2.0, 4.0}, {1.0, 4.0}, {1.0, 3.0}}});
      testPads.push_back({{{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}}});
      testPads.push_back({{{0.0, 1.0}, {1.0, 1.0}, {1.0, 2.0}, {0.0, 2.0}, {0.0, 1.0}}});
      testPads.push_back({{{1.0, 1.0}, {2.0, 1.0}, {2.0, 2.0}, {1.0, 2.0}, {1.0, 1.0}}});
      testPads.push_back({{{1.0, 2.0}, {2.0, 2.0}, {2.0, 3.0}, {1.0, 3.0}, {1.0, 2.0}}});
    }

    MultiPolygon testPads;
    SimplePolygon polygon;
    SimplePolygon testPolygon{
      {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {2.0, 1.0}, {2.0, 3.0}, {1.0, 3.0}, {1.0, 2.0}, {0.0, 2.0}, {0.0, 0.0}}};
};

SimplePolygon convertToGGL(const AliMUONPolygon& polygon)
{
  SimplePolygon p;

  for (int i = 0; i < polygon.NumberOfVertices(); ++i) {
    bg::append(p.outer(), Point{polygon.X(i), polygon.Y(i)});
  }

  return p;
}

MultiPolygon convertToGGL(const TObjArray& polygons)
{
  TIter next(&polygons);
  AliMUONPolygon* polygon;

  MultiPolygon contourGGL;
  contourGGL.resize(polygons.GetSize());
  int i{0};
  while ((polygon = static_cast<AliMUONPolygon*>(next()))) {
    if (polygon->NumberOfVertices() > 2) {
      contourGGL[i] = convertToGGL(*polygon);
    };
    ++i;
  }
  contourGGL.resize(i);
  return contourGGL;
}


MultiPolygon createManuContour(int detElemId, int manuId)
{
  AliMUONManuContourMaker contourMaker(nullptr);
  std::unique_ptr<AliMUONContour> contour(contourMaker.CreateManuContour(detElemId, manuId));

  return convertToGGL(*(contour->Polygons()));
}

BOOST_AUTO_TEST_SUITE(mch_aliroot_mapping)

BOOST_FIXTURE_TEST_SUITE(contoursFromAliRoot, CONTOURS)

BOOST_AUTO_TEST_CASE(NumberOfMotifPositionsIs2265)
{
  BOOST_CHECK_EQUAL(motifPositions.size(), 2265);
}

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

  MultiPolygon p = convertToGGL(pads);

  std::cout << bg::wkt(p) << '\n';

  AliMUONContourMaker maker;

  AliMUONContour* contour = maker.CreateContour(pads);

  MultiPolygon c = convertToGGL(*(contour->Polygons()));

  std::cout << bg::wkt(c) << '\n';
  basicSVG("toto.svg", {c, p});
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(contoursFromO2, POLYGONS)

BOOST_AUTO_TEST_CASE(GetYPositions)
{
  std::vector<double> ypos{getUniqueVerticalPositions(testPads)};

  const std::vector<double> expected{0, 1, 2, 3, 4};
  BOOST_TEST(ypos == expected);
}

BOOST_AUTO_TEST_CASE(GGLUnionGivesTooManyPoints)
{
  SimplePolygon a, b;

  bg::read_wkt("POLYGON((0.0 0.0, 1.0 0.0, 1.0 1.0, 0.0 1.0, 0.0 0.0))", a);
  bg::read_wkt("POLYGON((0.0 1.0, 1.0 1.0, 1.0 2.0, 0.0 2.0, 0.0 1.0))", b);
  bg::correct(a);
  bg::correct(b);
  MultiPolygon c;
  bg::union_(a, b, c);
  bg::correct(c);
  MultiPolygon expected;
  bg::read_wkt("MULTIPOLYGON(((0 1,0 0,1 0,1 1,1 2,0 2,0 1)))", expected);
  BOOST_CHECK(bg::equals(c, expected));
}

//BOOST_AUTO_TEST_CASE(CompareWithAliRootContour)
//{
//  Contour c = createManuContour(1001, 407);
//  Contour c2 = createContour(m.mseg(), 1001, 407);
//  std::cout << wkt(c) << '\n';
//  std::cout << wkt(c2) << '\n';
//  BOOST_CHECK(equals(c, c2));
//}

BOOST_AUTO_TEST_CASE(CreateCounterClockwiseOrientedPolygon)
{
  bg::read_wkt("POLYGON((0 0, 1 0, 1 1, 0 1, 0 0))", polygon);
  BOOST_CHECK(isCounterClockwiseOriented(polygon));

}

BOOST_AUTO_TEST_CASE(CreateClockwiseOrientedPolygon)
{
  bg::read_wkt("POLYGON((0 0, 0 1, 1 1, 1 0, 0 0))", polygon);
  BOOST_CHECK(!isCounterClockwiseOriented(polygon));
}

BOOST_AUTO_TEST_CASE(SignedArea)
{
  BOOST_CHECK_CLOSE(signedArea(testPolygon), 4.0, 0.1);
}

BOOST_AUTO_TEST_CASE(ContourCreationGeneratesEmptyContourForEmptyInput)
{
  MultiPolygon list;
  MultiPolygon contour = createContour(list);
  BOOST_CHECK(bg::is_empty(contour));
}

BOOST_AUTO_TEST_CASE(ContourCreationThrowsIfInputPolygonsAreNotCounterClockwiseOriented)
{
  MultiPolygon list;
  bg::read_wkt("POLYGON((0 0, 0 1, 1 1, 1 0, 0 0))", polygon);
  BOOST_CHECK(!isCounterClockwiseOriented(polygon));
  list.push_back(polygon);
  BOOST_CHECK_THROW(createContour(list), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(ContourCreationReturnsInputIfInputIsASinglePolygon)
{
  MultiPolygon list;
  bg::read_wkt("POLYGON((0 0, 1 0, 1 1, 0 1, 0 0))", polygon);
  list.push_back(polygon);
  MultiPolygon contour = createContour(list);
  BOOST_REQUIRE(contour.size() == 1);
  BOOST_CHECK(bg::equals(contour[0], polygon));
}

BOOST_AUTO_TEST_CASE(GetVerticalEdgesOfOneSimplePolygon)
{
  auto ypos = getUniqueVerticalPositions(testPolygon);
  auto edges = getVerticalEdges(testPolygon, ypos);

  BOOST_REQUIRE(edges.size() == 4);
  BOOST_CHECK_EQUAL(edges[0], VerticalEdge(1.0, 0, 1));
  BOOST_CHECK_EQUAL(edges[1], VerticalEdge(2.0, 1, 3));
  BOOST_CHECK_EQUAL(edges[2], VerticalEdge(1.0, 3, 2));
  BOOST_CHECK_EQUAL(edges[3], VerticalEdge(0.0, 2, 0));
}

BOOST_AUTO_TEST_CASE(GetVerticalEdgesOfAMultiPolygon)
{
  MultiPolygon group;
  SimplePolygon triangle{{{0.0, 0}, {1.0, 0.0}, {0.0, 1.0}, {0.0, 0.0}}};
  group.push_back(testPolygon);
  group.push_back(triangle);

  auto ypos = getUniqueVerticalPositions(group);
  auto edges = getVerticalEdges(group, ypos);

  BOOST_REQUIRE(edges.size() == 5);
  BOOST_CHECK_EQUAL(edges[0], VerticalEdge(1.0, 0, 1));
  BOOST_CHECK_EQUAL(edges[1], VerticalEdge(2.0, 1, 3));
  BOOST_CHECK_EQUAL(edges[2], VerticalEdge(1.0, 3, 2));
  BOOST_CHECK_EQUAL(edges[3], VerticalEdge(0.0, 2, 0));
  BOOST_CHECK_EQUAL(edges[4], VerticalEdge(0.0, 1, 0));
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeWithBeginAboveEndIsALefty)
{
  VerticalEdge vi{0.0, 12, 10};
  BOOST_CHECK_EQUAL(vi.isLeftEdge(), true);
  BOOST_CHECK_EQUAL(vi.isRightEdge(), false);
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeWithBeginAboveEndIsARighty)
{
  VerticalEdge vi{0.0, 10, 12};
  BOOST_CHECK_EQUAL(vi.isRightEdge(), true);
  BOOST_CHECK_EQUAL(vi.isLeftEdge(), false);
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeIntervalIsAnIntervalOfPositiveIndices)
{
  BOOST_CHECK_THROW(VerticalEdge(0, -1, 0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeHasATopAndBottom)
{
  VerticalEdge edge{2, 10, 12};
  BOOST_CHECK_EQUAL(bottom(edge), 10);
  BOOST_CHECK_EQUAL(top(edge), 12);
}

BOOST_AUTO_TEST_CASE(VerticalEdgeSortingMustSortSameAbcissaPointsLeftEdgeFirst)
{
  std::vector<VerticalEdge> edges;
  constexpr double sameX{42.42};
  VerticalEdge lastEdge{sameX + 1, 2, 0};
  VerticalEdge leftEdgeBottom{sameX, 2, 0};
  VerticalEdge leftEdgeTop{sameX, 10, 5};
  VerticalEdge rightEdge{sameX, 0, 2};

  edges.push_back(lastEdge);
  edges.push_back(rightEdge);
  edges.push_back(leftEdgeTop);
  edges.push_back(leftEdgeBottom);

  sortVerticalEdges(edges);

  BOOST_CHECK_EQUAL(edges[0], leftEdgeBottom);
  BOOST_CHECK_EQUAL(edges[1], leftEdgeTop);
  BOOST_CHECK_EQUAL(edges[2], rightEdge);
  BOOST_CHECK_EQUAL(edges[3], lastEdge);
}

BOOST_AUTO_TEST_CASE(SweepCreateContour)
{

  basicSVG("titi.svg", {testPads});

  std::vector<double> yPositions = getUniqueVerticalPositions(testPads);
  BOOST_REQUIRE(yPositions.size() == 5);

  std::vector<VerticalEdge> polygonVerticalEdges{getVerticalEdges(testPads, yPositions)};

  sortVerticalEdges(polygonVerticalEdges);

  std::unique_ptr<Node> segmentTree{createSegmentTree(yPositions)};

  std::vector<VerticalEdge> contourVerticalEdges{sweep(segmentTree.get(), polygonVerticalEdges)};

  BOOST_CHECK_EQUAL(contourVerticalEdges.size(), 3);

  for (auto& edge: contourVerticalEdges) {
    std::cout << edge << '\n';
  }
}

BOOST_AUTO_TEST_SUITE_END()
//
//BOOST_AUTO_TEST_CASE(TimeAliRootContourCreation)
//{
//  std::chrono::high_resolution_clock timer;
//
//  auto start = timer.now();
//
//  for (int i = 0; i < NLOOP; ++i) {
//    AliMpManuIterator it;
//    int deId, manuId;
//    while (it.Next(deId, manuId)) {
//      if (deId < 500) { continue; }
//      AliMUONManuContourMaker contourMaker(nullptr);
//      std::unique_ptr<AliMUONContour> contour(contourMaker.CreateManuContour(deId, manuId));
//    }
//  }
//  std::cout << "TimeAliRootContourCreation"
//            << std::chrono::duration_cast<std::chrono::milliseconds>(timer.now() - start).count() << " ms"
//            << '\n';
//
//  BOOST_CHECK(true);
//}

BOOST_AUTO_TEST_SUITE_END()


//Contour createContour(AliMpSegmentation* mseg, int detElemId, int manuId)
//{
//  const AliMpVSegmentation* seg = mseg->GetMpSegmentationByElectronics(detElemId, manuId);
//  const AliMpSlat* slat = slat_from_seg(*seg);
//  assert(slat);
//  AliMpMotifPosition* pos = slat->FindMotifPosition(manuId);
//  AliMpVMotif* motif = pos->GetMotif();
//  AliMpMotifType* motifType = motif->GetMotifType();
//
//  Contour contour;
//  Contour polygon;
//
//  for (Int_t i = 0; i <= 64; ++i) {
//    AliMpConnection* connection = motifType->FindConnectionByGassiNum(i);
//
//    if (connection) {
//      Int_t ix = connection->GetLocalIx();
//      Int_t iy = connection->GetLocalIy();
//
//      Double_t x, y, dx, dy;
//
//      motif->GetPadDimensionsByIndices(ix, iy, dx, dy);
//      motif->PadPositionLocal(ix, iy, x, y);
//
////      x += pos->GetPositionX() - seg->GetPositionX();
////      y += pos->GetPositionY() - seg->GetPositionY();
//
//      SimplePolygon pad{{
//                          {x - dx, y - dy},
//                          {x - dx, y + dy},
//                          {x + dx, y + dy},
//                          {x + dx, y - dy},
//                          {x - dx, y - dy}
//                        }};
//
//      union_(pad, polygon, contour);
//      polygon = contour;
//    }
//  }
//  return contour;
//}


//BOOST_AUTO_TEST_CASE(IsGGLUnionMethodActuallyWorking)
//{
//  SimplePolygon green, blue;
//
//  bg::read_wkt(
//    "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
//      "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))", green);
//  bg::read_wkt(
//    "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);
//
//  MultiPolygon output;
//  bg::correct(green);
//  bg::correct(blue);
//  bg::union_(green, blue, output);
//
//  bg::correct(output[0]);
//
//  std::cout << bg::wkt(output) << '\n';
//  MultiPolygon expected;
//  bg::read_wkt(
//    "MULTIPOLYGON(((2.15 1.45,2 1.3,2.9 0.7,3.58852 0.734426,4 -0.5,4.42542 0.776271,4.9 0.8,5.4 1.2,5.39302 1.29767,"
//      "6 1.5,5.36341 1.7122,5.3 2.6,4.175 2.975,4 3.5,3.59091 2.27273,3.4 2,3.42 1.97333,2.825 1.775,2.8 1.8,2.4 1.7,"
//      "2.3 1.6,2 1.5,2.15 1.45),(4.8 1.9,4.5 2,4.44444 2.16667,4.8 1.9)))",
//    expected);
//
//  expected = output;
//
//  bool areEqual = bg::equals(output, expected);
//  std::cout << bg::wkt(expected) << " " << areEqual << '\n';
//  BOOST_CHECK(areEqual);
//
//  auto list = {green, blue};
//  double xmin, ymin, xmax, ymax;
//  getBBOX(list, xmin, ymin, xmax, ymax);
//  double strokeWidth = (xmax - xmin) / 500.0;
//
//  std::ofstream out = headerSVG("test.svg", list);
//  outputToSVG(out, strokeWidth, "blue", list);
//  outputToSVG(out, strokeWidth * 2.0, "red", {output});
//
//  out << "</svg>";
//}

