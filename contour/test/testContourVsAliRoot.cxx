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
typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::polygon<Point, false> SimplePolygon;
typedef boost::geometry::model::multi_polygon<SimplePolygon> MultiPolygon;

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

    PolygonCollection<double> testPads;
    Polygon<double> polygon;
    Polygon<double> testPolygon{
      {{0.1, 0.1}, {1.1, 0.1}, {1.1, 1.1}, {2.1, 1.1}, {2.1, 3.1}, {1.1, 3.1}, {1.1, 2.1}, {0.1, 2.1}, {0.1, 0.1}}};
    Polygon<int> counterClockwisePolygon{{0, 0},
                                         {1, 0},
                                         {1, 1},
                                         {0, 1},
                                         {0, 0}};
    Polygon<int> clockwisePolygon{{0, 0},
                                  {0, 1},
                                  {1, 1},
                                  {1, 0},
                                  {0, 0}};
    Polygon<double> clockwisePolygonDouble{{0, 0},
                                           {0, 1},
                                           {1, 1},
                                           {1, 0},
                                           {0, 0}};
    std::vector<VerticalEdge> testVerticals{{0, 7, 1},
                                            {1, 1, 0},
                                            {3, 0, 1},
                                            {5, 1, 0},
                                            {6, 0, 7},
                                            {2, 5, 3},
                                            {4, 3, 5}};

};

template<typename T>
SimplePolygon convertToGGL(const Polygon<T>& polygon)
{

  SimplePolygon p;

  for (auto&& v: polygon) {
    bg::append(p.outer(), Point{v.x, v.y});
  }

  return p;
}

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

PolygonCollection<double> createManuPads(AliMpSegmentation* mseg, int detElemId, int manuId)
{
  const AliMpVSegmentation* seg = mseg->GetMpSegmentationByElectronics(detElemId, manuId);
  const AliMpSlat* slat = slat_from_seg(*seg);
  assert(slat);
  AliMpMotifPosition* pos = slat->FindMotifPosition(manuId);
  AliMpVMotif* motif = pos->GetMotif();
  AliMpMotifType* motifType = motif->GetMotifType();

  PolygonCollection<double> pads;

  for (Int_t i = 0; i <= 64; ++i) {
    AliMpConnection* connection = motifType->FindConnectionByGassiNum(i);

    if (connection) {
      Int_t ix = connection->GetLocalIx();
      Int_t iy = connection->GetLocalIy();

      Double_t x, y, dx, dy;

      motif->GetPadDimensionsByIndices(ix, iy, dx, dy);
      motif->PadPositionLocal(ix, iy, x, y);

//      x += pos->GetPositionX() - seg->GetPositionX();
//      y += pos->GetPositionY() - seg->GetPositionY();

      Polygon<double> pad{{
                            {x - dx, y - dy},
                            {x + dx, y - dy},
                            {x + dx, y + dy},
                            {x - dx, y + dy},
                            {x - dx, y - dy}
                          }};

      pads.push_back(pad);
    }
  }
  return pads;
}

BOOST_AUTO_TEST_SUITE(aliroot_mch_contour)

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

  AliMUONContourMaker maker;

  AliMUONContour* contour = maker.CreateContour(pads);

  MultiPolygon c = convertToGGL(*(contour->Polygons()));


  MultiPolygon expected;
  bg::read_wkt("MULTIPOLYGON(((0 2,0 0,2 0,2 4,1 4,1 2,0 2)))", expected);
  BOOST_CHECK(bg::equals(c, expected));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


