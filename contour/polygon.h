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
#include <initializer_list>
#include <sstream>
#include <algorithm>

namespace o2 {
namespace mch {
namespace contour {

template<typename T>
class Polygon
{
  public:

    using size_type = typename std::vector<o2::mch::contour::Vertex<T>>::size_type;

    Polygon() = default;

    Polygon(std::initializer_list<o2::mch::contour::Vertex<T>> args) : mVertices{args}
    {}

    o2::mch::contour::Vertex<T> firstVertex() const
    { return mVertices.front(); }

    Polygon<T>& addVertex(const Vertex <T>& vertex)
    {
      mVertices.push_back(vertex);
      return *this;
    }

    size_type size() const
    { return mVertices.size(); }

    bool empty() const
    { return size() == 0; }

    o2::mch::contour::Vertex<T> operator[](int i) const
    { return mVertices[i]; }

    bool isCounterClockwiseOriented() const
    {
      return signedArea() > 0.0;
    }

    std::vector<o2::mch::contour::Vertex<T>> getVertices() const
    { return mVertices; }

    std::vector<o2::mch::contour::Vertex<T>> getSortedVertices() const
    {
      std::vector<o2::mch::contour::Vertex<T>> vertices{
        mVertices.begin(), isClosed() ? mVertices.end() - 1 : mVertices.end()
      };
      std::sort(vertices.begin(), vertices.end());
      return vertices;
    }

    bool isManhattan() const
    {
      for (auto i = 0; i < mVertices.size() - 1; ++i) {
        if (!isVertical(mVertices[i], mVertices[i + 1]) && !isHorizontal(mVertices[i], mVertices[i + 1])) {
          return false;
        }
      }
      return true;
    }

    bool isClosed() const
    {
      return mVertices.back() == mVertices.front();
    }

    double signedArea() const
    {
      /// Compute the signed area of this polygon
      /// Algorithm from F. Feito, J.C. Torres and A. Urena,
      /// Comput. & Graphics, Vol. 19, pp. 595-600, 1995
      double area{0.0};
      for (auto i = 0; i < mVertices.size() - 1; ++i) {
        auto& current = mVertices[i];
        auto& next = mVertices[i + 1];
        area += current.x * next.y - next.x * current.y;
      }
      return area * 0.5;
    }

    friend std::ostream& operator<<(std::ostream& os, const Polygon<T>& polygon)
    {
      os << "POLYGON(";
      os << polygon.mVertices;
      os << ")";
      return os;
    }

  private:
    std::vector<o2::mch::contour::Vertex<T>> mVertices;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<Vertex<T>>& vertices)
{
  for (auto i = 0; i < vertices.size(); ++i) {
    os << std::setw(5) << vertices[i].x << " " << std::setw(5) << vertices[i].y;
    if (i < vertices.size() - 1) {
      os << ',';
    }
  }
  os << ')';
  return os;
}

template<typename T>
Polygon<T> close(Polygon<T> polygon)
{
  Polygon<T> pol{polygon};
  if (!polygon.isClosed()) {
    pol.addVertex(polygon.firstVertex());
  }
  if (!pol.isManhattan()) {
    throw std::logic_error("closing resulted in non Manhattan polygon");
  }
  return pol;
}

template<typename T>
bool operator!=(const Polygon<T>& lhs, const Polygon<T>& rhs)
{
  return !(rhs == lhs);
}


/**
 * Two polygons are considered equal if they include the same set of vertices,
 * irrespective of orientation.
 */
template<typename T>
bool operator==(const Polygon<T>& lhs, const Polygon<T>& rhs)
{
  if ( lhs.size() != rhs.size()) {
    return false;
  }

  auto l = lhs.getSortedVertices();
  auto r = rhs.getSortedVertices();

  if (l.size() != r.size()) {
    return false;
  }

  for (auto i = 0; i < l.size(); ++i) {
    if (l[i] != r[i]) {
      return false;
    }
  }
  return true;
}


}
}
}

#endif
