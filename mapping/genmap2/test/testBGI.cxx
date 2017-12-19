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


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "generateTestPoints.h"
#include "segmentationContours.h"
#include "segmentationInterface.h"
#include <array>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/function_output_iterator.hpp>
#include <iostream>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

namespace {
constexpr int NTESTPOINTS{1000};
}

using Contours = std::vector<o2::mch::contour::Contour<double>>;
using TestPoints = std::vector<std::pair<double, double>>;

Contours getSampaContours(int detElemId, bool isBendingPlane)
{
  auto seg = o2::mch::mapping::getSegmentation(detElemId, isBendingPlane);
  return o2::mch::mapping::getSampaContours(*seg);
}

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(boost_geometry_index)

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::polygon<point, false, true> polygon; // ccw, closed polygon
typedef std::pair<box, unsigned> value;

void print(const polygon &p, const point &tp)
{
  std::cout << bg::wkt(p) << " " << bg::within(tp, p) << "\n";
}

BOOST_AUTO_TEST_CASE(TwoPolygonsNotFillingUpTheBoundingBox)
{
  std::vector<polygon> p(3);

  bg::read_wkt("POLYGON((0 1,1 1,1 2,2 2,2 3,0 3,0 1))", p[0]);
  bg::read_wkt("POLYGON((2 0,4 0,4 2,3 2,3 1,2 1,2 0))", p[1]);
  bg::read_wkt("POLYGON((0 0, 2 0,2 1,3 1,3 2,4 2,4 3,2 3,2 2,1 2,1 1, 0 1, 0 0))", p[2]);

  const std::string style = "stroke:black;stroke-width:1%;fill:";
  const std::vector<std::string> colors = {"blue", "red", "green"};

  for (auto i = 0; i < 3; ++i) {
    std::cout << bg::svg(p[i], style + colors[i]) << "\n";
  }

  for (auto i = 0; i < 3; ++i) {
    print(p[i], {2, 1.5});
  }

  // create the rtree using default constructor
  bgi::rtree<value, bgi::rstar<16> > rtree;
// fill the spatial index
  for (unsigned i = 0; i < p.size(); ++i) {
    // calculate polygon bounding box
    box b = bg::return_envelope<box>(p[i]);
    // insert new value
    rtree.insert(std::make_pair(b, i));
  }
// find values intersecting some area defined by a box
  float delta{0.01};
  point testPoint{3.5, 0.5};
  box query_box(
    point(testPoint.get<0>() - delta, testPoint.get<1>() - delta),
    point(testPoint.get<0>() + delta, testPoint.get<1>() + delta));

  // intersect
  rtree.query(bgi::intersects(query_box), boost::make_function_output_iterator(
    [&p](auto const &val) { std::cout << "found:" << bg::wkt(p[val.second]) << "\n"; }
  ));

// find 5 nearest values to a point
  std::vector<value> result_n;
  rtree.query(bgi::nearest(testPoint, 1), std::back_inserter(result_n));

  std::cout << "nearest query result:" << std::endl;
  BOOST_FOREACH(value const &v, result_n) { std::cout << bg::wkt<polygon>(p[v.second]) << std::endl; }

}

BOOST_AUTO_TEST_CASE(FractionOfRectangles)
{
  int npolygons{0};
  int nrectangles{0};
  for (auto detElemId : o2::mch::mapping::getOneDetElemIdPerSegmentation()) {
    int np{0};
    int nr{0};
    for (auto isBendingPlane : {true, false}) {
      auto contours = getSampaContours(detElemId, isBendingPlane);
      for (auto &c: contours) {
        for (auto i = 0; i < c.size(); ++i) {
          ++npolygons;
          ++np;
          const auto &p = c[i];
          if (p.size() == 5) {
            ++nrectangles;
            ++nr;
          }
        }
      }
    }

    std::cout << boost::format("DE %4d nr/np = %7.2f%%\n") % detElemId % (nr*100.0/np);
  }
  std::cout << "npolygons=" << npolygons << " nrectangles=" << nrectangles << "\n";
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
