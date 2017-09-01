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

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/segment.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include <vector>

namespace o2 {
namespace mch {
namespace geometry {

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::segment<Point> Segment;
typedef boost::geometry::model::polygon<Point, false> SimplePolygon;
typedef boost::geometry::model::multi_polygon<SimplePolygon> MultiPolygon;

MultiPolygon createContour(MultiPolygon& polygons);

double signedArea(const SimplePolygon& polygon);

bool isCounterClockwiseOriented(const SimplePolygon& polygon);

std::vector<Segment> getVerticalEdges(const SimplePolygon& polygon);

std::vector<Segment> getVerticalEdges(const MultiPolygon& polygons);

void sortVerticalEdges(std::vector<Segment>& verticalEdges);

std::vector<Segment> sweep(const std::vector<Segment>& polygonVerticalEdges);

bool areEqual(double a, double b);

bool isVertical(const Segment& segment);

bool isLeftEdge(const Segment& segment);

bool isRightEdge(const Segment& segment);

double smallestY(const Segment& segment);

}
}
}

#endif
