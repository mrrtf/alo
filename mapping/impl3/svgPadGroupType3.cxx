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
using namespace o2::mch::mapping::impl3;
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
  out << "<html><body>\n";
  int ncols{5};
  int nlines{1 + NPG / ncols};
  float scale{8};
  int gutter{2};
  o2::mch::contour::BBox<double> box(0, ncols * (maxBox.width() + gutter), 0,
                                  nlines * (maxBox.height() + gutter));
  o2::mch::svg::writeHeader(out, box, scale);
  float x{0};
  float y{0};
  float w = scale * maxBox.width();
  float h = scale * maxBox.height();
  for (auto i = 0; i < boxes.size(); ++i) {
    auto pgt = getPadGroupType(i);
    out << "<g>\n";
    out << boost::format(
      "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:none;stroke:#DDDDDD;fill:#AAAAAA;stroke-width:1px\"/>\n") %
           x % y % w % h;
    out << boost::format(R"(<text x="%d" y="%d">)") % ( x + w*0.5 ) % ( y + h*0.8)
        << "PG" + std::to_string(i) + "(" +std::to_string(pgt.getNofPads()) + ")" << "</text>\n";
    for (auto p : pgtPads[i]) {
      p.scale(scale, scale);
      p.translate(x, y);
      writePolygon(out, p, style);
    }
    out << "</g>\n";
    x += scale * (maxBox.width() + gutter);
    if ((i + 1) % ncols == 0) {
      x = 0;
      y += scale * (maxBox.height() + gutter);
    }
  }
  out << "</svg>\n";
  out << "</body></html>\n";
}

