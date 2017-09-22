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
#include "segmentTree.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace o2 {
namespace mch {
namespace contour {

void sortVerticalEdges(std::vector<VerticalEdge<double>>& edges)
{
// sort vertical edges in ascending x order
// if same x, insure that left edges are before right edges
// within same x, order by increasing bottommost y
// Mind your steps ! This sorting is critical to the contour merging algorithm !

  std::sort(edges.begin(), edges.end(), [](const VerticalEdge<double>& e1, const VerticalEdge<double>& e2) {

    auto x1 = e1.begin().x;
    auto x2 = e2.begin().x;

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

std::vector<VerticalEdge<double>> sweep(Node<double>* segmentTree, const std::vector<VerticalEdge<double>>& polygonVerticalEdges)
{
  std::vector<VerticalEdge<double>> contourVerticalEdges;

  std::vector<Interval<double>> edgeStack;

  for (auto i = 0; i < polygonVerticalEdges.size(); ++i) {

    const auto& edge = polygonVerticalEdges[i];

    if (isLeftEdge(edge)) {
      segmentTree->contribution(interval(edge), edgeStack);
      segmentTree->insertInterval(interval(edge));
    } else {
      segmentTree->deleteInterval(interval(edge));
      segmentTree->contribution(interval(edge), edgeStack);
    }

    auto e1{edge};

    if (i < polygonVerticalEdges.size() - 1) {
      e1 = polygonVerticalEdges[i + 1];
    }

    if ((isLeftEdge(edge) != isLeftEdge(e1)) ||
        (!areEqual(edge.begin().x, e1.begin().x)) ||
        (i == polygonVerticalEdges.size() - 1)) {
      for (const auto& interval : edgeStack) {
        contourVerticalEdges.push_back(
          isRightEdge(edge) ? VerticalEdge<double>{edge.begin().x, interval.begin(), interval.end()} :
          VerticalEdge<double>{edge.begin().x, interval.end(), interval.begin()});
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

std::vector<HorizontalEdge<double>>
verticalsToHorizontals(const std::vector<VerticalEdge<double>>& verticals)
{
  std::vector<HorizontalEdge<double>> horizontals(verticals.size());

  typedef std::pair<Vertex<double>, double> VertexWithRef;
  std::vector<VertexWithRef> vertices;

  for (auto i = 0; i < verticals.size(); ++i) {
    const auto& edge = verticals[i];
    vertices.push_back({edge.begin(),i});
    vertices.push_back({edge.end(),i});
  }

  std::sort(vertices.begin(), vertices.end(), [](const VertexWithRef& v1, const VertexWithRef& v2) {
    return v1.first < v2.first;
  });

  for (auto i = 0; i < vertices.size() / 2; ++i) {
    const auto& p1 = vertices[2 * i];
    const auto& p2 = vertices[2 * i + 1];
    const VerticalEdge<double>& refEdge = verticals[p1.second];
    auto e = p1.first.x;
    auto b = p2.first.x;
    if ((p1.first.y == bottom(refEdge) && isLeftEdge(refEdge)) ||
        (p1.first.y == top(refEdge) && isRightEdge(refEdge))) {
      std::swap(b, e);
    }
    HorizontalEdge<double> h{p1.first.y, b, e};
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

PolygonCollection<double>
finalizeContour(const std::vector<VerticalEdge<double>>& verticals, const std::vector<HorizontalEdge<double>>& horizontals)
{
  if (verticals.size() != horizontals.size()) {
    throw std::invalid_argument("should get the same number of verticals and horizontals");
  }

  for (auto i = 0; i < verticals.size(); ++i) {
    if (horizontals[i].begin() != verticals[i].end()) {
      throw std::invalid_argument("got an horizontal edge not connected to its (supposedly) preceding vertical edge");
    }
  }

  std::vector<ManhattanEdge<double>> all;

  for (auto i = 0; i < verticals.size(); ++i) {
    all.push_back(verticals[i]);
    all.push_back(horizontals[i]);
  }

  PolygonCollection<double> contour;

  std::vector<bool> alreadyAdded(all.size(), false);
  std::vector<int> inorder;

  int nofUsed{0};
  int iCurrent{0};

  ManhattanEdge<double> startSegment{all[iCurrent]};

  while (nofUsed < all.size()) {

    const ManhattanEdge<double>& currentSegment{all[iCurrent]};
    inorder.push_back(iCurrent);
    alreadyAdded[iCurrent] = true;
    ++nofUsed;
    if (currentSegment.end() == startSegment.begin()) {
      Polygon<double> polygon;
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

  std::vector<VerticalEdge<double>> polygonVerticalEdges{getVerticalEdges(polygons)};

  sortVerticalEdges(polygonVerticalEdges);

  // Initialize the segment tree that is used by the sweep() function
  std::unique_ptr<Node<double>> segmentTree{createSegmentTree(getYPositions(polygons))};

  // Find the vertical edges of the merged contour. This is the meat of the algorithm...
  std::vector<VerticalEdge<double>> contourVerticalEdges{sweep(segmentTree.get(), polygonVerticalEdges)};

  // Deduce the horizontal edges from the vertical ones
  std::vector<HorizontalEdge<double>> contourHorizontalEdges{verticalsToHorizontals(contourVerticalEdges)};

  return finalizeContour(contourVerticalEdges, contourHorizontalEdges);

}

}
}
}

