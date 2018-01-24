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


#ifndef O2_MCH_SVG_CONTOUR_H
#define O2_MCH_SVG_CONTOUR_H

#include "contour.h"
#include "bbox.h"
#include <fstream>
#include "contourCreator.h"
#include <utility>
#include <boost/format.hpp>
#include <cmath>

namespace o2 {
namespace mch {
namespace svg {

class Writer
{

  public:
    Writer(std::ostream &os, int size, o2::mch::contour::BBox<double> &viewingBox) :
      mOutputStream{os},
      mWidth{size},
      mHeight{static_cast<int>(std::round(size * viewingBox.height() / viewingBox.width()))},
      mScale(10),
      mViewingBox{
        viewingBox
      }
    {
      svgStart();
    }


    ~Writer()
    {
      svgEnd();
    }

    template<typename T>
    Writer &operator<<(T a)
    {
      mOutputStream << a;
      return *this;
    }

    void svgStart()
    {
      mOutputStream << boost::format(R"(<svg width="%d" height="%d" viewBox="0 0 %f %f">
)")
                       % mWidth % mHeight
                       % mViewingBox.width() % mViewingBox.height();
    }

    void svgEnd()
    {
      mOutputStream << "</svg>";
    }

    void svgGroupStart(const std::string &classname)
    {
      mOutputStream << "<g class=\"" << classname << "\">\n";
    }

    void svgGroupEnd()
    {
      mOutputStream << "</g>\n";
    }

    void scale(int s)
    { mScale = s; }

    int scale() const
    { return mScale; }

    template<typename T>
    void polygon(const o2::mch::contour::Polygon<T> &polygon)
    {
      mOutputStream << "<polygon points=\"";
      auto vertices = getVertices(polygon);
      for (auto j = 0; j < vertices.size(); ++j) {
        auto v = vertices[j];
        mOutputStream << v.x - mViewingBox.xmin() << "," << v.y - mViewingBox.ymin() << ' ';
      }
      mOutputStream << "\"/>\n";
    }

    template<typename T>
    void contour(const o2::mch::contour::Contour<T> &contour)
    {
      for (auto& p: contour.getPolygons()) {
        polygon(p);
      }
    }

    void points(const std::vector<std::pair<double, double>> &points)
    {
      for (auto &p: points) {
        mOutputStream << boost::format(R"(<circle cx="%f" cy="%f"/>\n)") % p.first % p.second;
      }
    }

  private:
    std::ostream &mOutputStream;
    int mWidth;
    int mHeight;
    int mScale;
    o2::mch::contour::BBox<double> mViewingBox;
};

#if 0
void writeContours(const std::vector<o2::mch::contour::Contour<double>> &contours, const char *filename, double x,
                   double y);

void writeContour(std::ostream &out, int scale, const o2::mch::contour::Contour<double> &contour,
                  o2::mch::contour::BBox<double> &box, const char *style = "fill:none;stroke:black;stroke-width:0.5px");


template<typename T>
void writeHeader(std::ostream &out, o2::mch::contour::BBox<T> &box, int scale)
{
  out << "<svg height=\"" << scale * box.height() << "\" width=\"" << scale * box.width() << "\">\n";
}

void writeContour(std::string filename, const o2::mch::contour::Contour<double> &contour, int scale);

template<typename T>
void writePolygon(std::ostream &out, const o2::mch::contour::Polygon<T> &polygon,
                  const char *style = "fill:#eeeeee;stroke:black;stroke-width:1px")
{
  out << "<polygon points=\"";
  auto vertices = getVertices(polygon);
  for (auto j = 0; j < vertices.size(); ++j) {
    auto v = vertices[j];
    out << v.x << "," << v.y << ' ';
  }
  out << "\" style=\"" << style << "\"/>\n";

}

template<typename T>
void writePolygon(std::ostream &out, int scale, const o2::mch::contour::Polygon<T> &polygon,
                  o2::mch::contour::BBox<T> &box, const char *style = "fill:#eeeeee;stroke:black;stroke-width:1px")
{
  auto p = polygon;
  p.translate(-box.xmin(), -box.ymin());
  p.scale(scale, scale);
  writePolygon(out, p, style);
}

template<typename T>
void writePolygons(std::string filename, const std::vector<o2::mch::contour::Polygon<T>> &polygons, int scale)
{
  std::ofstream out(filename);
  out << "<html><body>\n";
  auto box = getBBox(polygons);
  writeHeader(out, box, scale);
  for (auto &p: polygons) {
    writePolygon(out, scale, p, box);
  }
  out << "</svg>\n";
  out << "</body></html>\n";

}

void writeContour(std::ostream &out, int scale, const o2::mch::contour::Contour<double> &contour,
                  o2::mch::contour::BBox<double> &box, const char *style)
{
  for (auto i = 0; i < contour.size(); ++i) {
    writePolygon(out, scale, contour[i], box, style);
  }
}

void writeContour(std::string filename, const o2::mch::contour::Contour<double> &contour, int scale)
{
  std::ofstream out(filename);
  out << "<html><body>\n";
  auto box = getBBox(contour);
  writeHeader(out, box, scale);
  writeContour(out, scale, contour, box);
  out << "</svg>\n";
  out << "</body></html>\n";
}

void writeContours(std::ostream &out, o2::mch::contour::BBox<double> &box, int scale,
                   const std::vector<o2::mch::contour::Contour<double>> &contours)
{
  writeHeader(out, box, scale);
  auto env = o2::mch::contour::getEnvelop(contours);
  writeContour(out, scale, env, box, "fill:#eeeeee;stroke:black;stroke-width:3px");
  for (auto i = 0; i < contours.size(); ++i) {
    auto &c = contours[i];
    writeContour(out, scale, c, box);
  }
}

void writePoints(std::ostream &out, int scale, const std::vector<std::pair<double, double>> &points,
                 int radius, const std::string &color)
{
  for (auto &p: points) {
    out << boost::format(R"(<circle cx="%f" cy="%f" r="%d" style="stroke:%s;stroke-width:0.25px;fill:none"/>\n)") %
           (scale * p.first) % (scale * p.second) % radius % color.c_str();
  }
}

void writeContours(const std::vector<o2::mch::contour::Contour<double>> &contours, const char *filename, double x,
                   double y)
{
  std::ofstream out(filename);
  int scale = 10;
  out << "<html><body>\n";

  auto env = o2::mch::contour::getEnvelop(contours);
  auto box = getBBox(env);

  x -= box.xmin();
  y -= box.ymin();

  writeHeader(out, box, scale);
  writeContour(out, scale, env, box, "fill:#eeeeee;stroke:black;stroke-width:3px");
  for (auto i = 0; i < contours.size(); ++i) {
    auto &c = contours[i];
    writeContour(out, scale, c, box);
    if (c.contains(x, y)) {
      writeContour(out, scale, c, box, "fill:#aaaaaa;stroke:none");
    }
  }

  out << "<circle cx=\"" << scale * x << "\" cy=\"" << scale * y << "\" r=\"5\"\n"
    "style=\"fill:none;stroke:black;stroke-width:0.5px;\"/>";

  out << "<circle cx=\"" << scale * x << "\" cy=\"" << scale * y << "\" r=\"1\"\n"
    "style=\"fill:none;stroke:red;stroke-width:0.5px;\"/>";

  out << "</svg>\n";
  out << "</body></html>\n";
}

#endif

}
}
}

#endif
