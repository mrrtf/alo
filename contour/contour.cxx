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
#include "edge.h"
#include "segment.h"
#include "segmentTree.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace o2 {
namespace mch {
namespace contour {

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

std::vector<VerticalEdge> sweep(Node* segmentTree, const std::vector<VerticalEdge>& polygonVerticalEdges)
{
  std::vector<VerticalEdge> contourVerticalEdges;

  std::vector<Interval<int>> edgeStack;

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

std::vector<HorizontalEdge>
verticalsToHorizontals(const std::vector<VerticalEdge>& verticals)
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
    int next = p2.second;
    if (b > e) {
      std::swap(preceding, next);
    }
    horizontals[preceding] = h;
  }
  return horizontals;
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

  std::vector<Segment> all;

  for (auto i = 0; i < verticals.size(); ++i) {
    all.push_back(Segment{verticals[i]});
    all.push_back(Segment{horizontals[i]});
  }

  PolygonCollection<int> contour;

  std::vector<bool> alreadyAdded(all.size(), false);
  std::vector<int> inorder;

  int nofUsed{0};
  int iCurrent{0};

  Segment startSegment{all[iCurrent]};

  while (nofUsed < all.size()) {

    const Segment& currentSegment{all[iCurrent]};
    inorder.push_back(iCurrent);
    alreadyAdded[iCurrent] = true;
    ++nofUsed;
    if (currentSegment.end() == startSegment.begin()) {
      Polygon<int> polygon;
      for (auto i: inorder) {
        polygon.push_back(all[i].begin());
      }
      if (polygon.empty()) {
        throw std::runtime_error("got an empty polygon");
      }
      contour.push_back(close(polygon));
      iCurrent = std::distance(alreadyAdded.begin(), std::find_if(alreadyAdded.begin(), alreadyAdded.end(),
                                                                  [](bool a) { return a == false; }));
      startSegment = all[iCurrent];
      inorder.clear();
    }

    for (auto i = 0; i < alreadyAdded.size(); ++i) {
      if (i != iCurrent && alreadyAdded[i] == false) {
        if (currentSegment.end() == all[i].begin()) {
          iCurrent = i;
          break;
        }
      }
    }
  }

  return contour;
}

PolygonCollection<double> createContour(const PolygonCollection<double>& polygons)
{
  if (polygons.empty()) {
    return {};
  }

  if (!isCounterClockwiseOriented(polygons)) {
    throw std::invalid_argument("polygons should be oriented counterclockwise");
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

}
}
}

