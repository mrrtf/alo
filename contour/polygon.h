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


#ifndef O2_MCH_CONTOUR_POLYGON_H
#define O2_MCH_CONTOUR_POLYGON_H

#include "vertex.h"
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

namespace o2 {
namespace mch {
namespace contour {

template<typename T>
using Polygon = std::vector<Vertex<T>>;

template<typename T>
using PolygonCollection = std::vector<Polygon<T>>;

inline std::ostream& operator<<(std::ostream& os, const Polygon<int>& polygon)
{
  os << "POLYGON(";

  for (auto i = 0; i < polygon.size(); ++i) {
    os << polygon[i];
    if (i < polygon.size() - 1) {
      os << ',';
    }
  }
  os << ')';
  return os;
}

template<typename T>
bool isManhattan(const Polygon<T>& polygon)
{
  for (auto i = 0; i < polygon.size() - 1; ++i) {
    if (!isVertical(polygon[i], polygon[i + 1]) && !isHorizontal(polygon[i], polygon[i + 1])) {
      return false;
    }
  }
  return true;
}

template<typename T>
bool isClosed(const Polygon<T>& polygon)
{
  return polygon.back() == polygon.front();
}

template<typename T>
bool isClosed(const PolygonCollection<T>& polygons)
{
  for (const auto& p: polygons) {
    if (!isClosed(p)) {
      return false;
    }
  }
  return true;
}

template<typename T>
Polygon<T> close(Polygon<T> polygon)
{
  if (!isClosed(polygon)) {
    polygon.push_back(polygon.front());
  }
  if (!isManhattan(polygon)) {
    throw std::logic_error("closing resulted in non Manhattan polygon");
  }
  return polygon;
}

Polygon<int>
integralPolygon(const Polygon<double>& polygon, std::vector<double>& xPositions, std::vector<double>& yPositions);

PolygonCollection<int>
integralPolygon(const PolygonCollection<double>& polygons, std::vector<double>& xPositions,
                std::vector<double>& yPositions);

/**
 * Two polygons are considered equal if they include the same set of vertices,
 * irrespective of orientation
 */
template<typename T>
bool operator==(const Polygon<T>& lhs, const Polygon<T>& rhs)
{
  if (lhs.size() != rhs.size()) {
    return false;
  }

  auto l = lhs;
  auto r = rhs;

  std::sort(l.begin(), l.end());
  std::sort(r.begin(), r.end());

  for (auto i = 0; i < lhs.size(); ++i) {
    if (l[i] != r[i]) {
      return false;
    }
  }
  return true;
}

template<typename T>
bool operator!=(const Polygon<T>& lhs, const Polygon<T>& rhs)
{
  return !(rhs == lhs);
}

template<typename T>
std::vector<Vertex<T> > getVertices(const PolygonCollection<T>& polygons)
{
  std::vector<Vertex<T> > vertices;

  for (auto i = 0; i < polygons.size(); ++i) {
    vertices.insert(vertices.end(), polygons[i].begin(), polygons[i].end());
  }

  return vertices;
}

template<typename T>
std::vector<Vertex<T> > getSortedVertices(const PolygonCollection<T>& polygons)
{
  std::vector<Vertex<T> > vertices{getVertices(polygons)};
  std::sort(vertices.begin(), vertices.end());
  return vertices;
}

/**
 * Two polygon collections are considered equal if they contain
 * the same set of vertices
 */
template<typename T>
bool operator==(const PolygonCollection<T>& lhs, const PolygonCollection<T>& rhs)
{
  std::vector<Vertex<T> > vl{getSortedVertices(lhs)};
  std::vector<Vertex<T> > vr{getSortedVertices(rhs)};

  if (vl.size() != vr.size()) {
    return false;
  }
  for (auto i = 0; i < vl.size(); ++i) {
    if (vl[i] != vr[i]) {
      return false;
    }
  }
  return true;
}

template<typename T>
bool operator!=(const PolygonCollection<T>& lhs, const PolygonCollection<T>& rhs)
{
  return !(rhs == lhs);
}

template<typename T>
double signedArea(const Polygon<T>& polygon)
{
  /// Compute the signed area of this polygon
  /// Algorithm from F. Feito, J.C. Torres and A. Urena,
  /// Comput. & Graphics, Vol. 19, pp. 595-600, 1995
  double area{0.0};
  for (auto i = 0; i < polygon.size() - 1; ++i) {
    auto& current = polygon[i];
    auto& next = polygon[i + 1];
    area += current.x * next.y - next.x * current.y;
  }
  return area * 0.5;
}

template<typename T>
bool isCounterClockwiseOriented(const Polygon<T>& polygon)
{
  return signedArea(polygon) > 0.0;
}

template<typename T>
bool isCounterClockwiseOriented(const PolygonCollection<T>& polygons)
{
  for (const auto& p: polygons) {
    if (!isCounterClockwiseOriented(p)) {
      return false;
    }
  }
  return true;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Polygon<T>& polygon)
{
  os << "POLYGON(";

  for (auto i = 0; i < polygon.size(); ++i) {
    os << std::setw(5) << polygon[i].x << " " << std::setw(5) << polygon[i].y;
    if (i < polygon.size() - 1) {
      os << ',';
    }
  }

  os << ')';
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const PolygonCollection<T>& polygons)
{
  os << "MULTIPOLYGON(";

  for (auto j = 0; j < polygons.size(); ++j) {
    const Polygon<T>& p{polygons[j]};
    os << '(';
    for (auto i = 0; i < p.size(); ++i) {
      os << p[i].x << " " << p[i].y;
      if (i < p.size() - 1) {
        os << ',';
      }
    }
    os << ')';
    if (j < polygons.size() - 1) {
      os << ',';
    }
  }
  os << ')';
  return os;
}

PolygonCollection<double> fpPolygon(const PolygonCollection<int>& ipolygons, const std::vector<double>& xPositions,
                                    const std::vector<double>& yPositions);

Polygon<double> fpPolygon(const Polygon<int>& ipolygons, const std::vector<double>& xPositions,
                          const std::vector<double>& yPositions);


}
}
}

#endif
