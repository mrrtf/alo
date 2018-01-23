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


#include "segmentationImpl2.h"
#include "svgContour.h"
#include "padGroupTypeContour.h"
#include <ostream>
#include <string>

using namespace o2::mch::contour;
using namespace o2::mch::mapping::impl2;
using namespace o2::mch::svg;

void asSVG(std::ostream &out, const Segmentation &seg, int scale)
{
  std::vector<Segmentation::Contour> contours;
  for (auto i = 0; i < seg.nofPadGroups(); ++i) {
    contours.push_back(seg.padGroupContour(i));
  }
  auto env = o2::mch::contour::createContour(contours);
  auto box = getBBox(env);
  writeHeader(out, box, scale);
  for (auto i = 0; i < contours.size(); ++i) {
    auto &c = contours[i];
    auto &pg = seg.padGroup(i);
    auto &pgt = seg.padGroupType(pg);

    // pads
    auto padPolygons = computePads(pgt);
    for (auto &p: padPolygons) {
      p.scale(seg.padSizeX(i), seg.padSizeY(i));
      p.translate(pg.mX, pg.mY);
      writePolygon(out, scale, p, box, "fill:none;stroke-width:0.5px;stroke-opacity:0.5;stroke:#666666");
    }

    // dual sampas
    std::string style = "fill:none;stroke:black;stroke-width:1px";
    writePolygon(out, scale, c, box, style.c_str());

    // detection element
    writeContour(out, scale, env, box, "fill:none;stroke:blue;stroke-width:2px");
  }
}

int main(int argc, char *argv[])
{
  int detElemId = atoi(argv[1]);
  bool isBendingPlane = atoi(argv[2]);

  std::unique_ptr<Segmentation> seg{createSegmentation(detElemId, isBendingPlane)};

  std::ofstream out("seg-" + std::to_string(detElemId) + "-" + (isBendingPlane ? "B" : "NB") + ".html");
  int scale = 10;
  out << "<html><body style=\"background:white\">\n";
  asSVG(out, *seg, scale);
  out << "</svg>\n";
  out << "</body></html>\n";
  return 0;
}