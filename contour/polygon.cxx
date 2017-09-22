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


#include "polygon.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include "cantypefitvalue.h"

namespace o2 {
namespace mch {
namespace contour {

Polygon<double> fpPolygon(const Polygon<int>& ipolygon, const std::vector<double>& xPositions,
                          const std::vector<double>& yPositions)
{
  PolygonCollection<int> c{ipolygon};
  return fpPolygon(c, xPositions, yPositions)[0];
}

PolygonCollection<double> fpPolygon(const PolygonCollection<int>& ipolygons, const std::vector<double>& xPositions,
                                    const std::vector<double>& yPositions)
{
  PolygonCollection<double> polygons;

  for (const auto& ip: ipolygons) {
    Polygon<double> p;
    for (const auto& v: ip) {
      p.push_back({xPositions[v.x], yPositions[v.y]});
    }
    polygons.push_back(p);
  }
  return polygons;
}

void unique(std::vector<double>& v)
{
  // remove non-unique values from v
  std::sort(v.begin(), v.end());
  auto last = std::unique(v.begin(), v.end(),
                          [](const double& a, const double& b) { return areEqual(a, b); });
  v.erase(last, v.end());
}

int findIndex(const std::vector<double>& vect, double y)
{
  auto result = std::find_if(vect.begin(), vect.end(), [y](double a) {
    return areEqual(a, y);
  });
  if (result == vect.end()) {
    throw std::out_of_range("unknown ordinate");
  }
  auto i = std::distance(vect.begin(), result);
  if (CanTypeFitValue<int>(i)) {
    return static_cast<int>(i);
  }
  throw std::range_error("Hum. Got an index not fitting in an int. That's highly suspicious for that algorithm !");
}

Polygon<int>
integralPolygon(const Polygon<double>& polygon, std::vector<double>& xPositions, std::vector<double>& yPositions)
{
  PolygonCollection<double> c{polygon};
  return integralPolygon(c, xPositions, yPositions)[0];
}

PolygonCollection<int>
integralPolygon(const PolygonCollection<double>& polygons, std::vector<double>& xPositions,
                std::vector<double>& yPositions)
{
  PolygonCollection<int> intPolygons;

  for (const auto& p: polygons) {
    for (const auto& v: p) {
      xPositions.push_back(v.x);
      yPositions.push_back(v.y);
    }
  }

  unique(xPositions);
  unique(yPositions);

  for (const auto& p: polygons) {
    Polygon<int> intPol;
    for (auto& v: p) {
      int x = findIndex(xPositions, v.x);
      int y = findIndex(yPositions, v.y);
      intPol.push_back({x, y});
    }
    intPolygons.push_back(intPol);
  }
  return intPolygons;
}

}
}
}
