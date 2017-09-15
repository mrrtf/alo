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


#ifndef O2_MCH_GEOMETRY_VERTEX_H
#define O2_MCH_GEOMETRY_VERTEX_H

#include <utility>
#include <vector>
#include <iostream>
#include <cmath>

namespace o2 {
namespace mch {
namespace geometry {

inline bool areEqual(double a, double b)
{
  return std::fabs(b - a) < 1E-5; // 1E-5 cm = 0.1 micron
}

inline bool areEqual(int a, int b)
{ return a == b; }

template<typename T>
struct Vertex
{
    Vertex(T abscissa, T ordinate) : x(abscissa), y(ordinate)
    {}

    T x;
    T y;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vertex<T>& vertex)
{
  os << '(' << vertex.x << ' ' << vertex.y << ')';
  return os;
}

template<typename T>
bool operator<(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  if (lhs.y < rhs.y) {
    return true;
  }
  if (rhs.y < lhs.y) {
    return false;
  }
  return lhs.x < rhs.x;
}

template<typename T>
bool operator>(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  return rhs < lhs;
}

template<typename T>
bool operator<=(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  return !(rhs < lhs);
}

template<typename T>
bool operator>=(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  return !(lhs < rhs);
}

template<typename T>
bool isVertical(const Vertex<T>& a, const Vertex<T>& b)
{
  return areEqual(a.x, b.x);
}

template<typename T>
bool isHorizontal(const Vertex<T>& a, const Vertex<T>& b)
{
  return areEqual(a.y, b.y);
}

template<typename T>
bool operator==(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator==(const Vertex<double>& lhs, const Vertex<double>& rhs)
{
  return areEqual(lhs.x, rhs.x) && areEqual(lhs.y, rhs.y);
}

template<typename T>
bool operator!=(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  return !(lhs == rhs);
}

}
}
}

#endif
