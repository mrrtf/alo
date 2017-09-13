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

template<typename T>
struct Vertex
{

    T x;
    T y;
};

template<typename T>
using Polygon = std::vector<Vertex<T>>;

template<typename T>
using PolygonCollection = std::vector<Polygon<T>>;

template<bool vertical>
class ManhattanEdge
{
  public:
    ManhattanEdge(int coordinate, int b, int e) : mCoordinate(coordinate),
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

typedef ManhattanEdge<true> VerticalEdge;
typedef ManhattanEdge<false> HorizontalEdge;

inline bool isLeftEdge(const VerticalEdge& edge)
{ return edge.isDirect(); }

inline bool isRightEdge(const VerticalEdge& edge)
{ return !isLeftEdge(edge); }

inline bool isLeftToRight(const HorizontalEdge& edge)
{ return edge.isDirect(); }

inline bool isRightToLeft(const HorizontalEdge& edge)
{ return !isLeftToRight(edge); }

template<typename T>
std::ostream& operator<<(std::ostream& os, const Polygon<T>& polygon);

int top(const VerticalEdge& vi);

int bottom(const VerticalEdge& vi);

PolygonCollection<double> createContour(PolygonCollection<double>& polygons);

template<typename T>
double signedArea(const Polygon<T>& polygon);

template<typename T>
bool isCounterClockwiseOriented(const Polygon<T>& polygon);

bool areEqual(double a, double b);

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
  std::vector<ManhattanEdge<vertical>> edges;
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
  std::vector<ManhattanEdge<vertical>> edges;
  for (const auto& p: polygons) {
    auto e = getEdges<vertical>(p);
    edges.insert(edges.end(),e.begin(),e.end());
  }
  return edges;
}


template<bool vertical>
std::ostream& operator<<(std::ostream& os, const ManhattanEdge<vertical>& interval)
{
  os << "abscissa: " << interval.coordinate() << " interval: [";
  auto b = interval.interval().begin();
  auto e = interval.interval().end();
  if (interval.isDirect()) {
    os << e << "," << b;
  } else {
    os << b << "," << e;
  }
  os << "]";
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


}
}
}

#endif
