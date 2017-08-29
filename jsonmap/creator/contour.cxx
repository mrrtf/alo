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
#include <boost/geometry/algorithms/is_empty.hpp>
#include <cmath>

namespace o2 {
namespace mch {
namespace geometry {

MultiPolygon createContour(MultiPolygon& polygons)
{
  MultiPolygon c;
  if (boost::geometry::is_empty(polygons)) {
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
{ return signedArea(polygon) > 0.0; }

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

std::vector<Segment> getVerticalEdges(const MultiPolygon& polygons) {
  /// From an array of polygons, extract the list of vertical edges.
  std::vector<Segment> verticals;
  for (auto&& p : polygons ) {
    std::vector<Segment> v = getVerticalEdges(p);
    verticals.insert(verticals.end(),v.begin(),v.end());
  }
  return verticals;
}

//void
//AliMUONContourMaker::GetVerticalEdges(const TObjArray& polygons, TObjArray& polygonVerticalEdges) const
//{
//  /// From an array of polygons, extract the list of vertical edges.
//  /// Output array polygonVerticalEdges should be empty before calling.
//
//  AliCodeTimerAuto("",0);
//
//  for ( Int_t i = 0; i <= polygons.GetLast(); ++i )
//  {
//    const AliMUONPolygon* g = static_cast<const AliMUONPolygon*>(polygons.UncheckedAt(i));
//    for ( Int_t j = 0; j < g->NumberOfVertices()-1; ++j )
//    {
//      if ( AliMUONSegment::AreEqual(g->X(j),g->X(j+1)) ) // segment is vertical
//      {
//        polygonVerticalEdges.Add(new AliMUONSegment(g->X(j),g->Y(j),g->X(j+1),g->Y(j+1)));
//      }
//    }
//  }
//}


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
