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


#ifndef O2_MCH_CONTOUR_CONTOUR_H
#define O2_MCH_CONTOUR_CONTOUR_H

#include <utility>
#include <vector>
#include <iostream>

#include "edge.h"
#include "polygon.h"

namespace o2 {
namespace mch {
namespace contour {

class Node;

PolygonCollection<double> createContour(const PolygonCollection<double>& polygons);

void sortVerticalEdges(std::vector<VerticalEdge>& edges);

std::vector<VerticalEdge> sweep(Node* segmentTree, const std::vector<VerticalEdge>& polygonVerticalEdges);

std::vector<HorizontalEdge> verticalsToHorizontals(const std::vector<VerticalEdge>& verticals);

template<bool vertical>
std::vector<ManhattanEdge<vertical>> getEdges(const Polygon<int>& polygon)
{
  /// Return the vertical edges of the input polygon
  std::vector<ManhattanEdge<vertical>>
    edges;
  for (auto i = 0; i < polygon.size() - 1; ++i) {
    auto& current = polygon[i];
    auto& next = polygon[i + 1];
    if (vertical && current.x == next.x) {
      edges.push_back({current.x, current.y, next.y});
    }
    if (!vertical && current.y == next.y) {
      edges.push_back({current.y, current.x, next.x});
    }
  }
  return edges;
}

template<bool vertical>
std::vector<ManhattanEdge<vertical>> getEdges(const PolygonCollection<int>& polygons)
{
  std::vector<ManhattanEdge<vertical>>
    edges;
  for (const auto& p: polygons) {
    auto e = getEdges<vertical>(p);
    edges.insert(edges.end(), e.begin(), e.end());
  }
  return edges;
}

PolygonCollection<int>
finalizeContour(const std::vector<VerticalEdge>& verticals, const std::vector<HorizontalEdge>& horizontals);

}
}
}


#endif
