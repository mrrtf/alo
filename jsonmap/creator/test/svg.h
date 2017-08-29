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


#ifndef ALO_SVG_H
#define ALO_SVG_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

template<typename T>
void getBBOX(std::initializer_list<T> geom, double& xmin, double& ymin, double& xmax, double& ymax)
{
  xmin = FLT_MAX;
  xmax = -FLT_MAX;
  ymin = FLT_MAX;
  ymax = -FLT_MAX;
  for (const auto& g: geom) {
    boost::geometry::model::box<typename boost::geometry::point_type<T>::type> box;
    boost::geometry::envelope(g, box);
    xmin = std::min(xmin, box.min_corner().x());
    xmax = std::max(xmax, box.max_corner().x());
    ymin = std::min(ymin, box.min_corner().y());
    ymax = std::max(ymax, box.max_corner().y());
  }
}

template<typename T>
std::ofstream headerSVG(const char* svgFile, std::initializer_list<T> geom)
{
  std::ofstream out(svgFile);

  double xmin, ymin, xmax, ymax;

  getBBOX(geom, xmin, ymin, xmax, ymax);

  out << R"(<?xml version="1.0" encoding="utf-8"?>
<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="800" viewBox=")";
  out << xmin << " " << ymin << " " << xmax << " " << ymax << "\">" << std::endl;

  return out;
}

template<typename T>
void outputToSVG(std::ostream& out, double strokeWidth, const char* strokeColor, std::initializer_list<T> geom)
{
  const std::string style = boost::io::str(
    boost::format("fill: none; stroke-width: %7.3f; stroke: %s") % strokeWidth % strokeColor);

  for (const auto& g: geom) {
    out << boost::geometry::svg(g, style) << std::endl;
  }
}

template<typename T>
void basicSVG(const char* filename, std::initializer_list<T> geom)
{
  double xmin, ymin, xmax, ymax;
  getBBOX(geom, xmin, ymin, xmax, ymax);
  double strokeWidth = 1E-2;
  std::ofstream out = headerSVG(filename, geom);
  outputToSVG(out, strokeWidth, "blue", geom);
  out << "</svg>";
}

#endif //ALO_SVG_H
