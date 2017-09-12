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

struct VerticalEdge
{
    double x;
    double yBegin;
    double yEnd;
};

class VerticalInterval
{
  public:
    VerticalInterval(double x, int b, int e) : mAbscissa(x), mInterval{b > e ? e : b, b > e ? b : e}, mIsLeft(b > e)
    {}

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

std::ostream& operator<<(std::ostream& os, const VerticalInterval& interval);

double top(const VerticalEdge& ve);

double bottom(const VerticalEdge& ve);

int top(const VerticalInterval& vi);

int bottom(const VerticalInterval& vi);

typedef boost::geometry::model::polygon<Point, false> SimplePolygon;
typedef boost::geometry::model::multi_polygon<SimplePolygon> MultiPolygon;

MultiPolygon createContour(MultiPolygon& polygons);

double signedArea(const SimplePolygon& polygon);

bool isCounterClockwiseOriented(const SimplePolygon& polygon);

std::vector<VerticalEdge> getVerticalEdges(const SimplePolygon& polygon);

std::vector<VerticalEdge> getVerticalEdges(const MultiPolygon& polygons);

void sortVerticalEdges(std::vector<VerticalEdge>& verticalEdges);

std::vector<double> getUniqueVerticalPositions(const std::vector<VerticalEdge>& verticalEdges);

std::vector<VerticalEdge> sweep(const std::vector<VerticalEdge>& polygonVerticalEdges);

bool areEqual(double a, double b);

bool areEqual(const VerticalEdge& a, const VerticalEdge& b);

bool isLeftEdge(const VerticalEdge& edge);

bool isRightEdge(const VerticalEdge& edge);

double smallestY(const VerticalEdge& edge);

std::vector<VerticalInterval> edge2interval(const std::vector<VerticalEdge>& edges,
                                            const std::vector<double>& allowedValues);


}
}
}

#endif
