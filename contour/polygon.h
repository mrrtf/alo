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


#ifndef O2_MCH_GEOMETRY_POLYGON_H
#define O2_MCH_GEOMETRY_POLYGON_H

#include <utility>
#include <vector>
#include <iostream>
#include "vertex.h"

namespace o2 {
namespace mch {
namespace geometry {

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
Polygon<T> close(Polygon<T> polygon)
{
  if (polygon.back() != polygon.front()) { // not already closed
    polygon.push_back(polygon.front());
  }
  if (!isManhattan(polygon)) { throw std::logic_error("closing resulted in non Manhattan polygon"); }
  return polygon;
}

Polygon<int>
integralPolygon(const Polygon<double>& polygon, std::vector<double>& xPositions, std::vector<double>& yPositions);

PolygonCollection<int>
integralPolygon(const PolygonCollection<double>& polygons, std::vector<double>& xPositions,
                std::vector<double>& yPositions);

bool operator==(const Polygon<int>& lhs, const Polygon<int>& rhs);

bool operator!=(const Polygon<int>& lhs, const Polygon<int>& rhs);

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
std::ostream& operator<<(std::ostream& os, const Polygon<T>& polygon)
{
  os << "POLYGON(";

  for (auto& v : polygon) {
    os << "(" << v.x << " " << v.y << "),";
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
