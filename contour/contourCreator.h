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


#ifndef O2_MCH_CONTOUR_CONTOURCREATOR_H
#define O2_MCH_CONTOUR_CONTOURCREATOR_H

#include <utility>
#include <vector>
#include <iostream>

#include "edge.h"
#include "polygon.h"
#include "contour.h"
#include "segmentTree.h"

namespace o2 {
namespace mch {
namespace contour {

Contour<double> createContour(const std::vector<Polygon<double>>& polygons);

void sortVerticalEdges(std::vector<VerticalEdge<double>>& edges);

std::vector<VerticalEdge<double>>
sweep(Node<double>* segmentTree, const std::vector<VerticalEdge<double>>& polygonVerticalEdges);

std::vector<HorizontalEdge<double>> verticalsToHorizontals(const std::vector<VerticalEdge<double>>& verticals);

Contour<double>
finalizeContour(const std::vector<VerticalEdge<double>>& verticals,
                const std::vector<HorizontalEdge<double>>& horizontals);

template<typename T>
Interval<T> interval(const VerticalEdge<T>& edge)
{
  auto y1 = edge.begin().y;
  auto y2 = edge.end().y;
  return y2 > y1 ? Interval<T>(y1, y2) : Interval<T>(y2, y1);
}

template<typename T>
std::vector<VerticalEdge<T>> getVerticalEdges(const Polygon<T>& polygon)
{
  /// Return the vertical edges of the input polygon
  std::vector<VerticalEdge<T>> edges;
  for (auto i = 0; i < polygon.size() - 1; ++i) {
    auto current = polygon[i];
    auto next = polygon[i + 1];
    if (current.x == next.x) {
      edges.push_back({current.x, current.y, next.y});
    }
  }
  return edges;
}

template<typename T>
std::vector<VerticalEdge<T>> getVerticalEdges(const std::vector<Polygon<T>>& polygons)
{
  std::vector<VerticalEdge<T>> edges;
  for (const auto& p: polygons) {
    auto e = getVerticalEdges(p);
    edges.insert(edges.end(), e.begin(), e.end());
  }
  return edges;
}

template<typename T>
std::vector<T> getYPositions(const std::vector<Polygon<T>>& polygons)
{
  auto vertices = getVertices(polygons);
  std::vector<T> ypos;
  for (const auto& v: vertices) {
    ypos.push_back(v.y);
  }
  std::sort(ypos.begin(),ypos.end());
  auto last = std::unique(ypos.begin(), ypos.end(),
                          [](const T& a, const T& b) { return areEqual(a, b); });
  ypos.erase(last, ypos.end());
  return ypos;
}

template<typename T>
std::vector<T> getXPositions(const std::vector<Polygon<T>>& polygons)
{
  auto vertices = getVertices(polygons);
  std::vector<T> xpos;
  for (const auto& v: vertices) {
    xpos.push_back(v.x);
  }
  std::sort(xpos.begin(),xpos.end());
  auto last = std::unique(xpos.begin(), xpos.end(),
                          [](const T& a, const T& b) { return areEqual(a, b); });
  xpos.erase(last, xpos.end());
  return xpos;
}
}
}
}

#endif
