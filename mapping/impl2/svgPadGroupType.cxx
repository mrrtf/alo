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

/// Small program which creates an HTML file with a SVG representation of
/// all the pad group types

#include "contour.h"
#include "svgContour.h"
#include "padGroupType.h"
#include "padGroupTypeContour.h"
#include "boost/format.hpp"

using namespace o2::mch::contour;
using namespace o2::mch::mapping::impl2;
using namespace o2::mch::svg;

int main()
{
  std::vector<std::vector<Polygon<double>>> pgtPads;
  std::vector<BBox<double>> boxes;

  double maxWidth{0.0};
  double maxHeight{0.0};

  const int NPG{216};

  for (int i = 0; i < NPG; ++i) {
    auto pgt = getPadGroupType(i);
    auto pads = computePads(pgt);
    pgtPads.push_back(pads);
    boxes.push_back(getBBox(pgtPads.back()));
    auto &b = boxes.back();
    maxWidth = std::max(maxWidth, b.width());
    maxHeight = std::max(maxHeight, b.height());
  }

  BBox<double> maxBox(0, maxWidth, 0, maxHeight);

  auto style = "fill:#eeeeee;stroke:black;stroke-width:1px";
  std::ofstream out("pgt-all.html");
  out << R"(
<html>
<style>
.padGroupType rect {
  stroke:#333333;
  fill:none;
  stroke-width:0.1px;
}
.padGroupType text{
  font-size: 2;
  stroke:none;
  fill: #333333;
}
.padGroupType polygon {
  fill: #DDDDDD;
  stroke:black;
  stroke-width:0.1px;
}
</style>
<body>
)";
  int ncols{5};
  int nlines{1 + NPG / ncols};
  int gutter{2};
  o2::mch::contour::BBox<double> box(0, ncols * (maxBox.width() + gutter), 0,
                                  nlines * (maxBox.height() + gutter));

  {
    o2::mch::svg::Writer writer(out, 1000, box);
    float x{0};
    float y{0};
    float w = maxBox.width();
    float h = maxBox.height();
    for (auto i = 0; i < boxes.size(); ++i) {
      auto pgt = getPadGroupType(i);
      writer.svgGroupStart("padGroupType");
      out << boost::format(
        "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n") %
             x % y % w % h;
      out << boost::format(R"(<text x="%d" y="%d">)") % (x + w * 0.5) % (y + h * 0.8)
          << "PG" + std::to_string(i) + "(" + std::to_string(pgt.getNofPads()) + ")" << "</text>\n";
      for (auto p : pgtPads[i]) {
        p.translate(x, y);
        writer.polygon(p);
      }
      writer.svgGroupEnd();
      x +=maxBox.width() + gutter;
      if ((i + 1) % ncols == 0) {
        x = 0;
        y += maxBox.height() + gutter;
      }
    }
  }
  out << "</body></html>\n";
}

