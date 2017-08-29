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

constexpr int NLOOP = 1;

using namespace o2::mch::geometry;

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
    SimplePolygon polygon;
    SimplePolygon testPolygon{
      {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {2.0, 1.0}, {2.0, 3.0}, {1.0, 3.0}, {1.0, 2.0}, {0.0, 2.0}, {0.0, 0.0}}};

};

SimplePolygon convertToGGL(const AliMUONPolygon& polygon)
{
  SimplePolygon p;

  for (int i = 0; i < polygon.NumberOfVertices(); ++i) {
    boost::geometry::append(p.outer(), Point{polygon.X(i), polygon.Y(i)});
  }

  return p;
}

MultiPolygon createManuContour(int detElemId, int manuId)
{
  AliMUONManuContourMaker contourMaker(nullptr);
  std::unique_ptr<AliMUONContour> contour(contourMaker.CreateManuContour(detElemId, manuId));
  TIter next(contour->Polygons());
  AliMUONPolygon* polygon;

  MultiPolygon contourGGL;
  contourGGL.resize(contour->Polygons()->GetSize());
  int i{0};
  while ((polygon = static_cast<AliMUONPolygon*>(next()))) {
    contourGGL[i] = convertToGGL(*polygon);
    ++i;
  }
  return contourGGL;
}

BOOST_AUTO_TEST_SUITE(mch_aliroot_mapping)

BOOST_FIXTURE_TEST_SUITE(contoursFromAliRoot, CONTOURS)

BOOST_AUTO_TEST_CASE(NumberOfMotifPositionsIs2265)
{
  BOOST_CHECK_EQUAL(motifPositions.size(), 2265);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(contoursFromO2, POLYGONS)

BOOST_AUTO_TEST_CASE(GGLUnionGivesTooManyPoints)
{
  SimplePolygon a, b;

  boost::geometry::read_wkt("POLYGON((0.0 0.0, 1.0 0.0, 1.0 1.0, 0.0 1.0, 0.0 0.0))", a);
  boost::geometry::read_wkt("POLYGON((0.0 1.0, 1.0 1.0, 1.0 2.0, 0.0 2.0, 0.0 1.0))", b);
  boost::geometry::correct(a);
  boost::geometry::correct(b);
  MultiPolygon c;
  boost::geometry::union_(a, b, c);
  boost::geometry::correct(c);
  MultiPolygon expected;
  boost::geometry::read_wkt("MULTIPOLYGON(((0 1,0 0,1 0,1 1,1 2,0 2,0 1)))", expected);
  BOOST_CHECK(boost::geometry::equals(c, expected));
}

//BOOST_AUTO_TEST_CASE(CompareWithAliRootContour)
//{
//  Contour c = createManuContour(1001, 407);
//  Contour c2 = createContour(m.mseg(), 1001, 407);
//  std::cout << wkt(c) << std::endl;
//  std::cout << wkt(c2) << std::endl;
//  BOOST_CHECK(equals(c, c2));
//}

BOOST_AUTO_TEST_CASE(CreateCounterClockwiseOrientedPolygon)
{
  boost::geometry::read_wkt("POLYGON((0 0, 1 0, 1 1, 0 1, 0 0))", polygon);
  BOOST_CHECK(isCounterClockwiseOriented(polygon));

}

BOOST_AUTO_TEST_CASE(CreateClockwiseOrientedPolygon)
{
  boost::geometry::read_wkt("POLYGON((0 0, 0 1, 1 1, 1 0, 0 0))", polygon);
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
  BOOST_CHECK(boost::geometry::is_empty(contour));
}

BOOST_AUTO_TEST_CASE(ContourCreationThrowsIfInputPolygonsAreNotCounterClockwiseOriented)
{
  MultiPolygon list;
  boost::geometry::read_wkt("POLYGON((0 0, 0 1, 1 1, 1 0, 0 0))", polygon);
  BOOST_CHECK(!isCounterClockwiseOriented(polygon));
  list.push_back(polygon);
  BOOST_CHECK_THROW(createContour(list), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(ContourCreationReturnsInputIfInputIsASinglePolygon)
{
  MultiPolygon list;
  boost::geometry::read_wkt("POLYGON((0 0, 1 0, 1 1, 0 1, 0 0))", polygon);
  list.push_back(polygon);
  MultiPolygon contour = createContour(list);
  BOOST_REQUIRE(contour.size() == 1);
  BOOST_CHECK(boost::geometry::equals(contour[0], polygon));
}

BOOST_AUTO_TEST_CASE(GetVerticalEdgesOfOneSimplePolygon)
{
  auto edges = getVerticalEdges(testPolygon);

  BOOST_REQUIRE(edges.size()==4);
  BOOST_CHECK(boost::geometry::equals(edges[0],Segment{ {1.0,0.0},{ 1.0, 1.0}}));
  BOOST_CHECK(boost::geometry::equals(edges[1],Segment{ {2.0,1.0},{ 2.0, 3.0}}));
  BOOST_CHECK(boost::geometry::equals(edges[2],Segment{ {1.0,3.0},{ 1.0, 2.0}}));
  BOOST_CHECK(boost::geometry::equals(edges[3],Segment{ {0.0,2.0},{ 0.0, 0.0}}));
}

BOOST_AUTO_TEST_CASE(GetVerticalEdgesOfAMultiPolygon)
{
  MultiPolygon group;
  SimplePolygon triangle{ { {0.0,0},{1.0,0.0},{0.0,1.0},{0.0,0.0}}};
  group.push_back(testPolygon);
  group.push_back(triangle);

  auto edges = getVerticalEdges(group);

  BOOST_REQUIRE(edges.size()==5);
  BOOST_CHECK(boost::geometry::equals(edges[0],Segment{ {1.0,0.0},{ 1.0, 1.0}}));
  BOOST_CHECK(boost::geometry::equals(edges[1],Segment{ {2.0,1.0},{ 2.0, 3.0}}));
  BOOST_CHECK(boost::geometry::equals(edges[2],Segment{ {1.0,3.0},{ 1.0, 2.0}}));
  BOOST_CHECK(boost::geometry::equals(edges[3],Segment{ {0.0,2.0},{ 0.0, 0.0}}));
  BOOST_CHECK(boost::geometry::equals(edges[4],Segment{ {0.0,1.0},{ 0.0, 0.0}}));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(TimeAliRootContourCreation)
{
  std::chrono::high_resolution_clock timer;

  auto start = timer.now();

  for (int i = 0; i < NLOOP; ++i) {
    AliMpManuIterator it;
    int deId, manuId;
    while (it.Next(deId, manuId)) {
      if (deId < 500) { continue; }
      AliMUONManuContourMaker contourMaker(nullptr);
      std::unique_ptr<AliMUONContour> contour(contourMaker.CreateManuContour(deId, manuId));
    }
  }
  std::cout << "TimeAliRootContourCreation"
            << std::chrono::duration_cast<std::chrono::milliseconds>(timer.now() - start).count() << " ms"
            << std::endl;

  BOOST_CHECK(true);
}

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
//  boost::geometry::read_wkt(
//    "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
//      "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))", green);
//  boost::geometry::read_wkt(
//    "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);
//
//  MultiPolygon output;
//  boost::geometry::correct(green);
//  boost::geometry::correct(blue);
//  boost::geometry::union_(green, blue, output);
//
//  boost::geometry::correct(output[0]);
//
//  std::cout << boost::geometry::wkt(output) << std::endl;
//  MultiPolygon expected;
//  boost::geometry::read_wkt(
//    "MULTIPOLYGON(((2.15 1.45,2 1.3,2.9 0.7,3.58852 0.734426,4 -0.5,4.42542 0.776271,4.9 0.8,5.4 1.2,5.39302 1.29767,"
//      "6 1.5,5.36341 1.7122,5.3 2.6,4.175 2.975,4 3.5,3.59091 2.27273,3.4 2,3.42 1.97333,2.825 1.775,2.8 1.8,2.4 1.7,"
//      "2.3 1.6,2 1.5,2.15 1.45),(4.8 1.9,4.5 2,4.44444 2.16667,4.8 1.9)))",
//    expected);
//
//  expected = output;
//
//  bool areEqual = boost::geometry::equals(output, expected);
//  std::cout << boost::geometry::wkt(expected) << " " << areEqual << std::endl;
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

