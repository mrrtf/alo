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
#include <stdexcept>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <cmath>

namespace bg = boost::geometry;

namespace o2 {
namespace mch {
namespace geometry {

bool isVertical(const Segment& segment)
{
  // a vertical segment has both points with the same x
  return areEqual(bg::get<0, 0>(segment), bg::get<1, 0>(segment));
}

bool isLeftEdge(const Segment& segment)
{
  // a vertical segment with first point above the second one is a left edge
  return isVertical(segment) && bg::get<0, 1>(segment) > bg::get<1, 1>(segment);
}

bool isRightEdge(const Segment& segment)
{
  // a vertical segment with first point below the second one is a right edge
  return isVertical(segment) && bg::get<0, 1>(segment) < bg::get<1, 1>(segment);
}

double smallestY(const Segment& segment)
{
  return std::min(bg::get<0, 1>(segment), bg::get<1, 1>(segment));
}

void sortVerticalEdges(std::vector<Segment>& verticalEdges)
{
// sort vertical edges in ascending x order
// if same x, insure that left edges are before right edges
// within same x, order by increasing bottommost y
// Mind your steps ! This sorting is critical to the contour merging algorithm !

  for (auto&& e: verticalEdges) {
    if (!isVertical(e)) { throw std::invalid_argument("can only sort vertical edges"); }
  }

  std::sort(verticalEdges.begin(), verticalEdges.end(), [](const Segment& s1, const Segment& s2) {

    auto x1 = bg::get<0, 0>(s1);
    auto x2 = bg::get<1, 0>(s2);

    auto y1 = smallestY(s1);
    auto y2 = smallestY(s2);

    if (areEqual(x1, x2)) {
      if (isLeftEdge(s1) && isRightEdge(s2)) { return true; }
      if (isRightEdge(s1) && isLeftEdge(s2)) { return false; }
      return y1 < y2;
    } else if (x1 < x2) {
      return true;
    } else {
      return false;
    }
  });
}

void sweep(std::vector<Segment> polygonVerticalEdges, std::vector<Segment> contourVerticalEdges);

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

  std::vector<Segment> polygonVerticalEdges{getVerticalEdges(polygons)};

  sortVerticalEdges(polygonVerticalEdges);

  std::vector<Segment> contourVerticalEdges{sweep(polygonVerticalEdges)};

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

std::vector<Segment> sweep(const std::vector<Segment>& polygonVerticalEdges)
{
   return {};
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

std::vector<Segment> getVerticalEdges(const SimplePolygon& polygon)
{
  /// Return the vertical edges of the input polygon
  std::vector<Segment> verticals;
  for (auto i = 0; i < polygon.outer().size() - 1; ++i) {
    const Point& current = polygon.outer()[i];
    const Point& next = polygon.outer()[i + 1];
    if (areEqual(current.x(), next.x()))// segment is vertical
    {
      verticals.push_back({current, next});
    }
  }
  return verticals;
}

bool areEqual(double a, double b)
{
  return std::fabs(b - a) < 1E-5; // 1E-5 cm = 0.1 micron
}

std::vector<Segment> getVerticalEdges(const MultiPolygon& polygons)
{
  /// From an array of polygons, extract the list of vertical edges.
  std::vector<Segment> verticals;
  for (auto&& p : polygons) {
    std::vector<Segment> v = getVerticalEdges(p);
    verticals.insert(verticals.end(), v.begin(), v.end());
  }
  return verticals;
}

}
}
}

////_____________________________________________________________________________
//AliMUONContour*
//AliMUONContourMaker::CreateContour(const TObjArray& polygons, const char* name) const
//{
//  /// Create the contour of the polygon array
//  /// and get back the intermediate verticals and horizontal segments
//  /// both arrays are arrays of AliMUONSegment objects.
//
//  AliCodeTimerAuto("",0);
//
//  if ( polygons.IsEmpty() ) return 0x0; // protection against user error...
//
//  // Sanity check : insure that all polygons are oriented counter-clockwise
//  TIter next(&polygons);
//  AliMUONPolygon* pol;
//  while ( ( pol = static_cast<AliMUONPolygon*>(next()) ) )
//  {
//    if ( !pol->IsCounterClockwiseOriented() )
//    {
//      AliError(Form("Got a clockwise oriented polygon in CreateContour(%s). That's not OK !",name));
//      StdoutToAliError(polygons.Print());
//      return 0x0;
//    }
//  }
//
//  AliMUONContour* contour(0x0);
//
//  if ( polygons.GetLast() == 0 )
//  {
//    AliCodeTimerAuto("Trivial case",1);
//    contour = new AliMUONContour(name);
//    pol = static_cast<AliMUONPolygon*>(polygons.First());
//    contour->Add(*pol);
//    contour->AssertOrientation();
//    return contour;
//  }
//
//  TObjArray polygonVerticalEdges; // arrray of AliMUONSegment objects
//  polygonVerticalEdges.SetOwner(kTRUE);
//  // get vertical edges of input polygons
//  GetVerticalEdges(polygons,polygonVerticalEdges);
//
//  // sort them in ascending x order
//  // if same x, insure that left edges are before right edges
//  // within same x, order by increasing bottommost y (see AliMUONSegment::Compare method)
//  polygonVerticalEdges.Sort();
//
//  if ( polygonVerticalEdges.GetLast()+1 < 2 )
//  {
//    polygons.Print();
//    AliFatal(Form("Got too few edges here for createContour %s",name));
//  }
//
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
//
//  return contour;
//}
