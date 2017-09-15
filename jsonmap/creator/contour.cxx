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


#include "contour.h"
#include "segmentTree.h"
#include <stdexcept>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <cmath>
#include <iostream>

namespace bg = boost::geometry;

namespace o2 {
namespace mch {
namespace geometry {

void sortVerticalEdges(std::vector<VerticalEdge>& edges)
{
// sort vertical edges in ascending x order
// if same x, insure that left edges are before right edges
// within same x, order by increasing bottommost y
// Mind your steps ! This sorting is critical to the contour merging algorithm !

  std::sort(edges.begin(), edges.end(), [](const VerticalEdge& e1, const VerticalEdge& e2) {

    auto x1 = e1.coordinate();
    auto x2 = e2.coordinate();

    auto y1 = bottom(e1);
    auto y2 = bottom(e2);

    if (areEqual(x1, x2)) {
      if (isLeftEdge(e1) && isRightEdge(e2)) { return true; }
      if (isRightEdge(e1) && isLeftEdge(e2)) { return false; }
      return y1 < y2;
    } else if (x1 < x2) {
      return true;
    } else {
      return false;
    }
  });
}

int findIndex(const std::vector<double>& vect, double y)
{
  auto result = std::find(vect.begin(), vect.end(), y);
  if (result == vect.end()) {
    throw std::out_of_range("unknown ordinate");
  }
  auto i = std::distance(vect.begin(), result);
  if (CanTypeFitValue<int>(i)) {
    return static_cast<int>(i);
  }
  throw std::range_error("Hum. Got an index not fitting in an int. That's highly suspicious for that algorithm !");
}

std::vector<VerticalEdge> sweep(Node* segmentTree, const std::vector<VerticalEdge>& polygonVerticalEdges)
{
  std::vector<VerticalEdge> contourVerticalEdges;

  std::vector<Interval> edgeStack;

  for (auto i = 0; i < polygonVerticalEdges.size(); ++i) {

    const auto& edge = polygonVerticalEdges[i];

    if (isLeftEdge(edge)) {
      segmentTree->contribution(edge.interval(), edgeStack);
      segmentTree->insertInterval(edge.interval());
    } else {
      segmentTree->deleteInterval(edge.interval());
      segmentTree->contribution(edge.interval(), edgeStack);
    }

    auto e1{edge};

    if (i < polygonVerticalEdges.size() - 1) {
      e1 = polygonVerticalEdges[i + 1];
    }

    if ((isLeftEdge(edge) != isLeftEdge(e1)) ||
        (!areEqual(edge.coordinate(), e1.coordinate())) ||
        (i == polygonVerticalEdges.size() - 1)) {
      for (auto&& interval : edgeStack) {
        contourVerticalEdges.push_back(
          isRightEdge(edge) ? VerticalEdge{edge.coordinate(), interval.begin(), interval.end()} :
          VerticalEdge{edge.coordinate(), interval.end(), interval.begin()});
      }
      edgeStack.clear();
    }
  }

  return contourVerticalEdges;
}

/**
 * Generates horizontal edges from the vertical ones
 * The horizontals are ordered relative to the verticals, i.e. the first horizontal
 * should be the edge __following__ the first vertical, etc...
 *
 * @param verticals
 * @return the horizontals, in the relevant order
 */
std::vector<HorizontalEdge> verticalsToHorizontals(const std::vector<VerticalEdge>& verticals)
{
  std::vector<HorizontalEdge> horizontals(verticals.size());
  typedef std::pair<Vertex<int>, int> VertexWithRef;
  std::vector<VertexWithRef> vertices;

  for (auto i = 0; i < verticals.size(); ++i) {
    const VerticalEdge& edge = verticals[i];
    vertices.push_back({{edge.coordinate(), edge.interval().end()}, i});
    vertices.push_back({{edge.coordinate(), edge.interval().begin()}, i});
  }

  std::sort(vertices.begin(), vertices.end(), [](const VertexWithRef& v1, const VertexWithRef& v2) {
    return v1.first < v2.first;
  });

  for (auto i = 0; i < vertices.size() / 2; ++i) {
    auto& p1 = vertices[2 * i];
    auto& p2 = vertices[2 * i + 1];
    const VerticalEdge& refEdge = verticals[p1.second];
    int e = p1.first.x;
    int b = p2.first.x;
    if ((p1.first.y == bottom(refEdge) && isLeftEdge(refEdge)) ||
        (p1.first.y == top(refEdge) && isRightEdge(refEdge))) {
      std::swap(b, e);
    }
    HorizontalEdge h{p1.first.y, b, e};
    // which vertical edge is preceding this horizontal ?
    int preceding = p1.second;
    if (b > e) {
      preceding = p2.second;
    }
    horizontals[preceding] = h;
    //std::cout << std::setw(2) << horizontals.size() << " H=" << horizontals.back() << " after " << std::setw(2) << preceding << " V=" << verticals[preceding] << '\n';
  }
  return horizontals;
}

Polygon<double> fpPolygon(const Polygon<int>& ipolygon, const std::vector<double>& xPositions,
                          const std::vector<double>& yPositions)
{
  PolygonCollection<int> c{ipolygon};
  return fpPolygon(c, xPositions, yPositions)[0];
}

PolygonCollection<double> fpPolygon(const PolygonCollection<int>& ipolygons, const std::vector<double>& xPositions,
                                    const std::vector<double>& yPositions)
{
  PolygonCollection<double> polygons;

  for (const auto& ip: ipolygons) {
    Polygon<double> p;
    for (const auto& v: ip) {
      p.push_back({xPositions[v.x], yPositions[v.y]});
    }
    polygons.push_back(p);
  }
  return polygons;
}

PolygonCollection<int>
finalizeContour(const std::vector<VerticalEdge>& verticals, const std::vector<HorizontalEdge>& horizontals)
{
  if (verticals.size() != horizontals.size()) {
    throw std::invalid_argument("should get the same number of verticals and horizontals");
  }

  for (auto i = 0; i < verticals.size(); ++i) {
    if (beginVertex(horizontals[i]) != endVertex(verticals[i])) {
      throw std::invalid_argument("got an horizontal edge not connected to its (supposedly) preceding vertical edge");
    }
  }

  PolygonCollection<int> contour;

  for (auto i = 0; i < verticals.size(); ++i) {
    std::cout << verticals[i] << " | " << horizontals[i] << '\n';
  }

  int ifirst{0};
  int count{0};

  while (ifirst < verticals.size()) {

    ++count;

    if ( count > 1000 ) {
      throw std::runtime_error("too many iterations, most probably an infinite loop ?");
    }

    Polygon<int> polygon;

    const Vertex<int>& first{beginVertex(verticals[ifirst])};

    for (auto i = ifirst; i < verticals.size(); ++i) {

      const VerticalEdge& v = verticals[i];
      const HorizontalEdge& h = horizontals[i];

      polygon.push_back(beginVertex(v));
      polygon.push_back(endVertex(v));

      if (endVertex(h)==first) {
        ifirst = i+1;
        break;
      }
    }

    if (polygon.empty()) {
      throw std::runtime_error("got an empty polygon");
    }

    std::cout << polygon << '\n';

    contour.push_back(close(polygon));
  }

  return contour;
}

PolygonCollection<double> createContour(PolygonCollection<double>& polygons)
{
  if (polygons.empty()) {
    return {};
  }
  for (auto i = 0; i < polygons.size(); ++i) {
    if (!isCounterClockwiseOriented(polygons[i])) {
      throw std::invalid_argument("polygons should be oriented counterclockwise");
    }
  }

  // trivial case : only one input polygon
  if (polygons.size() == 1) {
    return polygons;
  }

  std::vector<double> xPositions;
  std::vector<double> yPositions;

  PolygonCollection<int> ipolygons{integralPolygon(polygons, xPositions, yPositions)};

  std::vector<VerticalEdge> polygonVerticalEdges{getEdges<true>(ipolygons)};

  sortVerticalEdges(polygonVerticalEdges);

  // Initialize the segment tree that is used by the sweep() function
  std::unique_ptr<Node> segmentTree{createSegmentTree(yPositions)};

  // Find the vertical edges of the merged contour. This is the meat of the algorithm...
  std::vector<VerticalEdge> contourVerticalEdges{sweep(segmentTree.get(), polygonVerticalEdges)};

  // Deduce the horizontal edges from the vertical ones
  std::vector<HorizontalEdge> contourHorizontalEdges{verticalsToHorizontals(contourVerticalEdges)};

  PolygonCollection<int> icontour{finalizeContour(contourVerticalEdges, contourHorizontalEdges)};

  return fpPolygon(icontour, xPositions, yPositions);
}

bool areEqual(double a, double b)
{
  return std::fabs(b - a) < 1E-5; // 1E-5 cm = 0.1 micron
}

void unique(std::vector<double>& v)
{
  // remove non-unique values from v
  std::sort(v.begin(), v.end());
  auto last = std::unique(v.begin(), v.end(),
                          [](const double& a, const double& b) { return areEqual(a, b); });
  v.erase(last, v.end());
}

bool operator==(const Polygon<int>& lhs, const Polygon<int>& rhs)
{
  if (lhs.size() != rhs.size()) {
    return false;
  }

  if (isCounterClockwiseOriented(lhs) != isCounterClockwiseOriented(rhs)) {
    return false;
  }

  auto l = lhs;
  auto r = rhs;

  std::sort(l.begin(), l.end());
  std::sort(r.begin(), r.end());
  return l == r;
}

bool operator!=(const Polygon<int>& lhs, const Polygon<int>& rhs)
{
  return !(rhs == lhs);

}

bool operator==(const Vertex<double>& lhs, const Vertex<double>& rhs)
{
  return areEqual(lhs.x, rhs.x) && areEqual(lhs.y, rhs.y);
}

Polygon<int>
integralPolygon(const Polygon<double>& polygon, std::vector<double>& xPositions, std::vector<double>& yPositions)
{
  PolygonCollection<double> c{polygon};
  return integralPolygon(c, xPositions, yPositions)[0];
}

PolygonCollection<int>
integralPolygon(const PolygonCollection<double>& polygons, std::vector<double>& xPositions,
                std::vector<double>& yPositions)
{
  PolygonCollection<int> intPolygons;

  for (const auto& p: polygons) {
    for (const auto& v: p) {
      xPositions.push_back(v.x);
      yPositions.push_back(v.y);
    }
  }

  unique(xPositions);
  unique(yPositions);

  for (const auto& p: polygons) {
    Polygon<int> intPol;
    for (auto& v: p) {
      int x = findIndex(xPositions, v.x);
      int y = findIndex(yPositions, v.y);
      intPol.push_back({x, y});
    }
    intPolygons.push_back(intPol);
  }
  return intPolygons;
}

}
}
}

