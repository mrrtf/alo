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
#include "contour.h"

using namespace o2::mch::geometry;

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

BOOST_AUTO_TEST_SUITE(o2_mch_geometry)

BOOST_FIXTURE_TEST_SUITE(contour, POLYGONS)

BOOST_AUTO_TEST_CASE(CircularTestIntegralToDoublePolygon)
{
  std::vector<double> xPositions, yPositions;
  Polygon<int> ipolygons = integralPolygon(testPolygon, xPositions, yPositions);
  Polygon<double> test = fpPolygon(ipolygons, xPositions, yPositions);
  BOOST_CHECK(test == testPolygon);
}

BOOST_AUTO_TEST_CASE(ContourCreationGeneratesEmptyContourForEmptyInput)
{
  PolygonCollection<double> list;
  PolygonCollection<double> contour = createContour(list);
  BOOST_CHECK(contour.empty());
}

BOOST_AUTO_TEST_CASE(ContourCreationThrowsIfInputPolygonsAreNotCounterClockwiseOriented)
{
  PolygonCollection<double> list;
  list.push_back(clockwisePolygonDouble);
  BOOST_CHECK_THROW(createContour(list), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(ContourCreationReturnsInputIfInputIsASinglePolygon)
{
  PolygonCollection<double> list;
  Polygon<double> polygon{{0, 0},
                          {1, 0},
                          {1, 1},
                          {0, 1},
                          {0, 0}};
  list.push_back(polygon);
  auto contour = createContour(list);
  BOOST_REQUIRE(contour.size() == 1);
  BOOST_CHECK_EQUAL(contour[0], polygon);
}

BOOST_AUTO_TEST_CASE(GetVerticalEdgesOfOneSimplePolygon)
{
  std::vector<double> xpos, ypos;
  auto p = integralPolygon(testPolygon, xpos, ypos);
  auto edges = getEdges<true>(p);

  BOOST_REQUIRE(edges.size() == 4);
  BOOST_CHECK_EQUAL(edges[0], VerticalEdge(1.0, 0, 1));
  BOOST_CHECK_EQUAL(edges[1], VerticalEdge(2.0, 1, 3));
  BOOST_CHECK_EQUAL(edges[2], VerticalEdge(1.0, 3, 2));
  BOOST_CHECK_EQUAL(edges[3], VerticalEdge(0.0, 2, 0));
}

BOOST_AUTO_TEST_CASE(GetVerticalEdgesOfAMultiPolygon)
{
  PolygonCollection<int> group;
  Polygon<int> triangle{{{0, 0}, {1, 0}, {0, 1}, {0, 0}}};
  std::vector<double> xpos, ypos;
  auto p = integralPolygon(testPolygon, xpos, ypos);
  group.push_back(p);
  group.push_back(triangle);

  auto edges = getEdges<true>(group);

  BOOST_REQUIRE(edges.size() == 5);
  BOOST_CHECK_EQUAL(edges[0], VerticalEdge(1.0, 0, 1));
  BOOST_CHECK_EQUAL(edges[1], VerticalEdge(2.0, 1, 3));
  BOOST_CHECK_EQUAL(edges[2], VerticalEdge(1.0, 3, 2));
  BOOST_CHECK_EQUAL(edges[3], VerticalEdge(0.0, 2, 0));
  BOOST_CHECK_EQUAL(edges[4], VerticalEdge(0.0, 1, 0));
}

BOOST_AUTO_TEST_CASE(VerticalEdgeSortingMustSortSameAbcissaPointsLeftEdgeFirst)
{
  std::vector<VerticalEdge> edges;
  constexpr int sameX{42};
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
  std::vector<double> xPositions, yPositions;

  auto p = integralPolygon(testPads, xPositions, yPositions);
  BOOST_REQUIRE(yPositions.size() == 5);

  std::vector<VerticalEdge> polygonVerticalEdges{getEdges<true>(p)};

  sortVerticalEdges(polygonVerticalEdges);

  std::unique_ptr<Node> segmentTree{createSegmentTree(yPositions)};

  std::vector<VerticalEdge> contourVerticalEdges{sweep(segmentTree.get(), polygonVerticalEdges)};

  BOOST_REQUIRE(contourVerticalEdges.size() == 3);
  BOOST_CHECK_EQUAL(contourVerticalEdges[0], VerticalEdge(0, 2, 0));
  BOOST_CHECK_EQUAL(contourVerticalEdges[1], VerticalEdge(1, 4, 2));
  BOOST_CHECK_EQUAL(contourVerticalEdges[2], VerticalEdge(2, 0, 4));
}

BOOST_AUTO_TEST_CASE(VerticalsToHorizontals)
{
  std::vector<HorizontalEdge> he{verticalsToHorizontals(testVerticals)};

  std::vector<HorizontalEdge> expected{
    {1, 0, 1},
    {0, 1, 3},
    {1, 3, 5},
    {0, 5, 6},
    {7, 6, 0},
    {3, 2, 4},
    {5, 4, 2}
  };

  BOOST_CHECK(he == expected);
}

BOOST_AUTO_TEST_CASE(GetVertexFromVertical)
{
  VerticalEdge e{12, 20, 100};

  BOOST_CHECK_EQUAL(beginVertex(e), Vertex<int>(12, 20));
  BOOST_CHECK_EQUAL(endVertex(e), Vertex<int>(12, 100));
}

BOOST_AUTO_TEST_CASE(GetVertexFromHorizontal)
{
  HorizontalEdge e{12, 20, 100};

  BOOST_CHECK_EQUAL(beginVertex(e), Vertex<int>(20, 12));
  BOOST_CHECK_EQUAL(endVertex(e), Vertex<int>(100, 12));
}

BOOST_AUTO_TEST_CASE(FinalizeContourThrowsIfNumberOfVerticalsDifferFromNumberOfHorizontals)
{
  std::vector<VerticalEdge> v{{0, 1, 0},
                              {1, 0, 1}};
  std::vector<HorizontalEdge> h{{0, 0, 1}};
  BOOST_CHECK_THROW(finalizeContour(v, h), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(FinalizeContourThrowsIfEndOfVerticalsDoNotMatchBeginOfHorizontals)
{
  std::vector<VerticalEdge> v{{0, 7, 1}};
  std::vector<HorizontalEdge> wrong{{1, 2, 3}};

  BOOST_CHECK_THROW(finalizeContour(v, wrong), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(FinalizeContour)
{
  std::vector<HorizontalEdge> he{verticalsToHorizontals(testVerticals)};

  auto contour = finalizeContour(testVerticals, he);

  PolygonCollection<int> expected{
    {{0, 7}, {0, 1}, {1, 1}, {1, 0}, {3, 0}, {3, 1}, {5, 1}, {5, 0}, {6, 0}, {6, 7}, {0, 7}},
    {{2, 5}, {2, 3}, {4, 3}, {4, 5}, {2, 5}}
  };

  BOOST_TEST(contour == expected);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

