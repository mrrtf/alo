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

std::vector<HorizontalEdge> verticalsToHorizontals(const std::vector<VerticalEdge>& verticals)
{
  // TODO : implement me
  return {};
}

PolygonCollection<double> createContour(PolygonCollection<double>& polygons)
{
  PolygonCollection<double> c;
  if (polygons.empty()) {
    return c;
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

  PolygonCollection<int> ipolygons = integralPolygon(polygons, xPositions, yPositions);

  std::vector<VerticalEdge> polygonVerticalEdges{getEdges<true>(ipolygons)};

  sortVerticalEdges(polygonVerticalEdges);

  std::unique_ptr<Node> segmentTree{createSegmentTree(yPositions)};

  // Find the vertical edges of the merged contour. This is the meat of the algorithm...
  std::vector<VerticalEdge> contourVerticalEdges{sweep(segmentTree.get(), polygonVerticalEdges)};

  std::vector<HorizontalEdge> contourHorizontalEdges{verticalsToHorizontals(contourVerticalEdges)};

//  contour = FinalizeContour(contourVerticalEdges,horizontals);
//
  // return fpPolygon(ipolygons,xPositions,yPositions);

  return c;
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

int top(const VerticalEdge& vi)
{ return isLeftEdge(vi) ? vi.interval().begin() : vi.interval().end(); }

int bottom(const VerticalEdge& vi)
{ return isLeftEdge(vi) ? vi.interval().end() : vi.interval().begin(); }

bool operator<(const Vertex<int>& v1, const Vertex<int>& v2)
{
  if (v1.y < v2.y) { return true; }
  if (v1.y > v2.y) { return false; }
  if (v1.x < v2.y) { return true; }
  return false;
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

