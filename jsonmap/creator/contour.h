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


#ifndef O2_MCH_GEOMETRY_CONTOUR_H
#define O2_MCH_GEOMETRY_CONTOUR_H

#include "segmentTree.h"

#include <utility>
#include <vector>
#include <iostream>

namespace o2 {
namespace mch {
namespace geometry {

bool areEqual(double a, double b);

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
using Polygon = std::vector<Vertex<T>>;

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

template<typename T>
using PolygonCollection = std::vector<Polygon<T>>;

template<bool vertical>
class ManhattanEdge
{
  public:

    ManhattanEdge(int coordinate = {}, int b = {}, int e = 1) : mCoordinate(coordinate),
                                                                mInterval{b > e ? e : b, b > e ? b : e},
                                                                mIsDirect(b > e)
    {
      if (b < 0 || e < 0) { throw std::out_of_range("b,e are supposed to be indices, i.e. >=0"); }
    }

    int coordinate() const
    { return mCoordinate; }

    Interval interval() const
    { return mInterval; }

    bool isDirect() const
    { return mIsDirect; }

    bool isVertical() const
    { return vertical; }

    bool isHorizontal() const
    { return !vertical; }

  private:
    int mCoordinate;
    Interval mInterval;
    bool mIsDirect;

};

using VerticalEdge= ManhattanEdge<true>;
using HorizontalEdge = ManhattanEdge<false>;

inline bool isLeftEdge(const VerticalEdge& edge)
{ return edge.isDirect(); }

inline bool isRightEdge(const VerticalEdge& edge)
{ return !isLeftEdge(edge); }

inline bool isTopToBottom(const VerticalEdge& edge)
{
  return isLeftEdge(edge);
}

inline bool isBottomToTop(const VerticalEdge& edge)
{
  return !isTopToBottom(edge);
}

inline bool isLeftToRight(const HorizontalEdge& edge)
{ return !edge.isDirect(); }

inline bool isRightToLeft(const HorizontalEdge& edge)
{ return !isLeftToRight(edge); }

template<bool vertical>
int begin(const ManhattanEdge<vertical>& edge)
{
  return edge.isDirect() ? edge.interval().end() : edge.interval().begin();
}

template<bool vertical>
int end(const ManhattanEdge<vertical>& edge)
{
  return edge.isDirect() ? edge.interval().begin() : edge.interval().end();
}
//inline int begin(const VerticalEdge& v) {
//  return isLeftEdge(v) ? v.interval().end() : v.interval().begin();
//}
//
//inline int end(const VerticalEdge& v) {
//  return isLeftEdge(v) ? v.interval().begin() : v.interval().end();
//}

inline int top(const VerticalEdge& vi)
{ return std::max(vi.interval().begin(), vi.interval().end()); }

inline int bottom(const VerticalEdge& vi)
{ return std::min(vi.interval().begin(), vi.interval().end()); }

template<typename T>
std::ostream& operator<<(std::ostream& os, const Polygon<T>& polygon);

PolygonCollection<double> createContour(PolygonCollection<double>& polygons);

template<typename T>
double signedArea(const Polygon<T>& polygon);

template<typename T>
bool isCounterClockwiseOriented(const Polygon<T>& polygon);

void sortVerticalEdges(std::vector<VerticalEdge>& edges);

std::vector<VerticalEdge> sweep(Node* segmentTree, const std::vector<VerticalEdge>& polygonVerticalEdges);

Polygon<int>
integralPolygon(const Polygon<double>& polygon, std::vector<double>& xPositions, std::vector<double>& yPositions);

PolygonCollection<int>
integralPolygon(const PolygonCollection<double>& polygons, std::vector<double>& xPositions,
                std::vector<double>& yPositions);

bool operator==(const Polygon<int>& lhs, const Polygon<int>& rhs);

bool operator!=(const Polygon<int>& lhs, const Polygon<int>& rhs);

std::vector<HorizontalEdge> verticalsToHorizontals(const std::vector<VerticalEdge>& verticals);

template<typename T>
bool operator==(const Vertex<T>& lhs, const Vertex<T>& rhs);

template<typename T>
bool operator!=(const Vertex<T>& lhs, const Vertex<T>& rhs);

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

template<bool vertical>
std::ostream& operator<<(std::ostream& os, const ManhattanEdge<vertical>& edge)
{
  os << (vertical ? "abscissa: " : "ordinate: ");
  os << edge.coordinate() << " interval: [";
  auto b = edge.interval().begin();
  auto e = edge.interval().end();
  if (edge.isDirect()) {
    os << e << "," << b;
  } else {
    os << b << "," << e;
  }
  os << "]";
  return os;
  return os;
}


template<bool vertical>
bool operator==(const ManhattanEdge<vertical>& lhs, const ManhattanEdge<vertical>& rhs)
{
  return areEqual(lhs.coordinate(), rhs.coordinate()) &&
         lhs.interval() == rhs.interval() &&
         lhs.isDirect() == rhs.isDirect();
}

template<bool vertical>
bool operator!=(const ManhattanEdge<vertical>&& lhs, const ManhattanEdge<vertical>& rhs)
{
  return !(rhs == lhs);
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

template<typename T>
bool operator==(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<typename T>
bool operator!=(const Vertex<T>& lhs, const Vertex<T>& rhs)
{
  return !(lhs == rhs);
}

PolygonCollection<double> fpPolygon(const PolygonCollection<int>& ipolygons, const std::vector<double>& xPositions,
                                    const std::vector<double>& yPositions);

Polygon<double> fpPolygon(const Polygon<int>& ipolygons, const std::vector<double>& xPositions,
                          const std::vector<double>& yPositions);

PolygonCollection<int>
finalizeContour(const std::vector<VerticalEdge>& verticals, const std::vector<HorizontalEdge>& horizontals);

template<bool vertical>
Vertex<int> beginVertex(const ManhattanEdge<vertical>& edge)
{
  return vertical ? Vertex<int> {edge.coordinate(), begin(edge)} : Vertex<int> {begin(edge), edge.coordinate()};

}

template<bool vertical>
Vertex<int> endVertex(const ManhattanEdge<vertical>& edge)
{
  return vertical ? Vertex<int> {edge.coordinate(), end(edge)} : Vertex<int> {end(edge), edge.coordinate()};
}

}
}
}

#endif
