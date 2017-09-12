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

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include "segmentTree.h"

#include <vector>
#include <ostream>

namespace o2 {
namespace mch {
namespace geometry {

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::polygon<Point, false> SimplePolygon;
typedef boost::geometry::model::multi_polygon<SimplePolygon> MultiPolygon;

class VerticalEdge
{
  public:
    VerticalEdge(double x, int b, int e) : mAbscissa(x), mInterval{b > e ? e : b, b > e ? b : e}, mIsLeft(b > e)
    {
        if (b<0 || e<0) throw std::out_of_range("b,e are supposed to be indices, i.e. >=0");
    }

    double abscissa() const
    { return mAbscissa; }

    Interval interval() const
    { return mInterval; }

    bool isLeftEdge() const
    { return mIsLeft; }

    bool isRightEdge() const
    { return !mIsLeft; }

  private:
    double mAbscissa;
    Interval mInterval;
    bool mIsLeft;
};

std::ostream& operator<<(std::ostream& os, const VerticalEdge& interval);

int top(const VerticalEdge& vi);

int bottom(const VerticalEdge& vi);

MultiPolygon createContour(MultiPolygon& polygons);

double signedArea(const SimplePolygon& polygon);

bool isCounterClockwiseOriented(const SimplePolygon& polygon);

bool areEqual(double a, double b);

void sortVerticalEdges(std::vector<VerticalEdge>& edges);

std::vector<VerticalEdge> sweep(Node* segmentTree, const std::vector<VerticalEdge>& polygonVerticalEdges);

std::vector<double> getUniqueVerticalPositions(const MultiPolygon& polygons);

std::vector<double> getUniqueVerticalPositions(const SimplePolygon& polygon);

std::vector<VerticalEdge> getVerticalEdges(const MultiPolygon& polygons, const std::vector<double>& yPositions);

std::vector<VerticalEdge> getVerticalEdges(const SimplePolygon& polygon, const std::vector<double>& yPositions);

bool operator==(const VerticalEdge& lhs, const VerticalEdge& rhs);

bool operator!=(const VerticalEdge& lhs, const VerticalEdge& rhs);
}
}
}

#endif
