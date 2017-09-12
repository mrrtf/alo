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
#include "segmentTree.h"
#include <stdexcept>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <cmath>
#include <iostream>

namespace bg = boost::geometry;

namespace o2 {
namespace mch {
namespace geometry {

void sortVerticalEdges(std::vector<VerticalEdge>& edges)
{
// sort vertical edges in ascending x order
// if same x, insure that left edges are before right edges
// within same x, order by increasing bottommost y
// Mind your steps ! This sorting is critical to the contour merging algorithm !

  std::sort(edges.begin(), edges.end(), [](const VerticalEdge& e1, const VerticalEdge& e2) {

    auto x1 = e1.abscissa();
    auto x2 = e2.abscissa();

    auto y1 = bottom(e1);
    auto y2 = bottom(e2);

    if (areEqual(x1, x2)) {
      if (e1.isLeftEdge() && e2.isRightEdge()) { return true; }
      if (e1.isRightEdge() && e2.isLeftEdge()) { return false; }
      return y1 < y2;
    } else if (x1 < x2) {
      return true;
    } else {
      return false;
    }
  });
}

int findIndex(const std::vector<double>& vect, double y)
{
  auto result = std::find(vect.begin(), vect.end(), y);
  if (result == vect.end()) {
    throw std::out_of_range("unknown ordinate");
  }
  auto i = std::distance(vect.begin(), result);
  if (CanTypeFitValue<int>(i)) {
    return static_cast<int>(i);
  }
  throw std::range_error("Hum. Got an index not fitting in an int. That's highly suspicious for that algorithm !");
}

std::vector<VerticalEdge> sweep(Node* segmentTree, const std::vector<VerticalEdge>& polygonVerticalEdges)
{
  std::vector<VerticalEdge> contourVerticalEdges;

  std::vector<Interval> edgeStack;

  for (auto i = 0; i < polygonVerticalEdges.size(); ++i) {

    const auto& edge = polygonVerticalEdges[i];

    if (edge.isLeftEdge()) {
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

    if ((edge.isLeftEdge() != e1.isLeftEdge()) ||
        (!areEqual(edge.abscissa(), e1.abscissa())) ||
        (i == polygonVerticalEdges.size() - 1)) {
      for (auto&& interval : edgeStack) {
        contourVerticalEdges.push_back(edge.isRightEdge() ? VerticalEdge{edge.abscissa(), interval.begin(), interval.end()} :
                                       VerticalEdge{edge.abscissa(), interval.end(), interval.begin()});
      }
      edgeStack.clear();
    }
  }

  return contourVerticalEdges;
}

//void
//AliMUONContourMaker::Sweep(const TObjArray& polygonVerticalEdges,
//                           TObjArray& contourVerticalEdges) const
//{
//  TArrayD yPositions;
//  GetYPositions(polygonVerticalEdges,yPositions);
//
//  AliMUONSegmentTree segmentTree(yPositions);
//
//  for ( Int_t i = 0; i <= polygonVerticalEdges.GetLast(); ++i )
//  {
//    const AliMUONSegment* edge = static_cast<const AliMUONSegment*>(polygonVerticalEdges.UncheckedAt(i));
//
//    assert(edge!=0x0);
//
//    if ( edge->IsLeftEdge() )
//    {
//      segmentTree.Contribution(edge->Bottom(),edge->Top());
//      segmentTree.InsertInterval(edge->Bottom(),edge->Top());
//    }
//    else
//    {
//      segmentTree.DeleteInterval(edge->Bottom(),edge->Top());
//      segmentTree.Contribution(edge->Bottom(),edge->Top());
//    }
//
//    AliMUONSegment e1(*edge);
//
//    if ( i < polygonVerticalEdges.GetLast() )
//    {
//      const AliMUONSegment* next = static_cast<const AliMUONSegment*>(polygonVerticalEdges.UncheckedAt(i+1));
//      e1 = *next;
//    }
//
//    if ( ( edge->IsLeftEdge() != e1.IsLeftEdge() ) ||
//         ( !AliMUONSegment::AreEqual(edge->StartX(),e1.StartX() ) ) ||
//         ( i == polygonVerticalEdges.GetLast() ) )
//    {
//      const TObjArray& stack = segmentTree.Stack();
//
//      double x = edge->StartX();
//
//      for ( Int_t j = 0; j <= stack.GetLast(); ++j )
//      {
//        AliMUONSegment* sj = static_cast<AliMUONSegment*>(stack.UncheckedAt(j));
//        AliMUONSegment* s = new AliMUONSegment(x,sj->StartY(),x,sj->EndY());
//
//        if  (s->IsAPoint())
//        {
//          delete s;
//          continue;
//        }
//
//        if ( edge->IsLeftEdge() != s->IsLeftEdge() )
//        {
//          s->Set(x,sj->EndY(),x,sj->StartY());
//        }
//        contourVerticalEdges.Add(s);
//      }
//      segmentTree.ResetStack();
//    }
//  }
//}

MultiPolygon createContour(MultiPolygon& polygons)
{
  MultiPolygon c;
  if (bg::is_empty(polygons)) {
    return c;
  }
  for (auto i = 0; i < polygons.size(); ++i) {
    if (!isCounterClockwiseOriented(polygons[i])) {
      throw std::invalid_argument("polygons should be oriented counterclockwise");
    }
  }

  // trivial case : only one input polygon
  if (polygons.size() == 1) {
    return polygons;
  }

  std::vector<double> yPositions = getUniqueVerticalPositions(polygons);

  std::vector<VerticalEdge> polygonVerticalEdges{getVerticalEdges(polygons, yPositions)};

  sortVerticalEdges(polygonVerticalEdges);

  std::unique_ptr<Node> segmentTree{createSegmentTree(yPositions)};

  std::vector<VerticalEdge> contourVerticalEdges{sweep(segmentTree.get(), polygonVerticalEdges)};

//  // Find the vertical edges of the merged contour. This is the meat of the algorithm...
//  TObjArray contourVerticalEdges;
//  contourVerticalEdges.SetOwner(kTRUE);
//  Sweep(polygonVerticalEdges,contourVerticalEdges);
//
//  TObjArray horizontals;
//  horizontals.SetOwner(kTRUE);
//  VerticalToHorizontal(contourVerticalEdges,horizontals);
//
//  contour = FinalizeContour(contourVerticalEdges,horizontals);
//
//  if ( contour && name ) contour->SetName(name);
  return c;
}

double signedArea(const SimplePolygon& polygon)
{
  /// Compute the signed area of this polygon
  /// Algorithm from F. Feito, J.C. Torres and A. Urena,
  /// Comput. & Graphics, Vol. 19, pp. 595-600, 1995
  double area{0.0};
  for (auto i = 0; i < polygon.outer().size() - 1; ++i) {
    const Point& current = polygon.outer()[i];
    const Point& next = polygon.outer()[i + 1];
    area += current.x() * next.y() - next.x() * current.y();
  }
  return area * 0.5;
}

bool isCounterClockwiseOriented(const SimplePolygon& polygon)
{
  return signedArea(polygon) > 0.0;
}

std::vector<VerticalEdge> getVerticalEdges(const SimplePolygon& polygon, const std::vector<double>& yPositions)
{
  /// Return the vertical edges of the input polygon
  std::vector<VerticalEdge> verticals;
  for (auto i = 0; i < polygon.outer().size() - 1; ++i) {
    const Point& current = polygon.outer()[i];
    const Point& next = polygon.outer()[i + 1];
    if (areEqual(current.x(), next.x()))// ve is vertical
    {
      int b = findIndex(yPositions, current.y());
      int e = findIndex(yPositions, next.y());
      verticals.push_back({current.x(), b, e});
    }
  }
  return verticals;
}

bool areEqual(double a, double b)
{
  return std::fabs(b - a) < 1E-5; // 1E-5 cm = 0.1 micron
}

std::vector<VerticalEdge> getVerticalEdges(const MultiPolygon& polygons, const std::vector<double>& yPositions)
{
  /// From an array of polygons, extract the list of vertical edges.
  std::vector<VerticalEdge> verticals;
  for (auto&& p : polygons) {
    std::vector<VerticalEdge> v = getVerticalEdges(p, yPositions);
    verticals.insert(verticals.end(), v.begin(), v.end());
  }
  return verticals;
}

std::vector<double> getUniqueVerticalPositions(const SimplePolygon& polygon)
{
  std::vector<double> yPositions;
  for (auto i = 0; i < polygon.outer().size() - 1; ++i) {
    const Point& current = polygon.outer()[i];
    const Point& next = polygon.outer()[i + 1];
    if (areEqual(current.x(), next.x()))// ve is vertical
    {
      yPositions.push_back(current.y());
      yPositions.push_back(next.y());
    }
  }

  std::sort(yPositions.begin(), yPositions.end());
  auto last = std::unique(yPositions.begin(), yPositions.end(),
                          [](const double& a, const double& b) { return areEqual(a, b); });
  yPositions.erase(last, yPositions.end());
  return yPositions;
}

std::vector<double> getUniqueVerticalPositions(const MultiPolygon& polygons)
{
  std::vector<double> yPositions;

  for (auto&& p : polygons) {
    auto ypos = getUniqueVerticalPositions(p);
    yPositions.insert(yPositions.end(), ypos.begin(), ypos.end());
  }
  std::sort(yPositions.begin(), yPositions.end());
  auto last = std::unique(yPositions.begin(), yPositions.end(),
                          [](const double& a, const double& b) { return areEqual(a, b); });
  yPositions.erase(last, yPositions.end());
  return yPositions;
}

bool areEqual(const VerticalEdge& a, const VerticalEdge& b)
{
  return areEqual(a.abscissa(), b.abscissa()) && a.interval() == b.interval();
}

std::ostream& operator<<(std::ostream& os, const VerticalEdge& interval)
{
  os << "abscissa: " << interval.abscissa() << " interval: [";
  auto b = interval.interval().begin();
  auto e = interval.interval().end();
  if (interval.isLeftEdge()) {
    os << e << "," << b;
  } else {
    os << b << "," << e;
  }
  os << "]";
  return os;
}

int top(const VerticalEdge& vi)
{ return vi.isLeftEdge() ? vi.interval().begin() : vi.interval().end(); }

int bottom(const VerticalEdge& vi)
{ return vi.isLeftEdge() ? vi.interval().end() : vi.interval().begin(); }

bool operator==(const VerticalEdge& lhs, const VerticalEdge& rhs)
{
  return areEqual(lhs.abscissa(), rhs.abscissa()) &&
         lhs.interval() == rhs.interval() &&
         lhs.isLeftEdge() == rhs.isLeftEdge();
}

bool operator!=(const VerticalEdge& lhs, const VerticalEdge& rhs)
{
  return !(rhs == lhs);
}
}
}
}

