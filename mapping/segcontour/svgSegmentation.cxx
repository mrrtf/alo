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
/// @author  Laurent Apaduidecetche


#include "segmentation.h"
#include "svgContour.h"
#include <ostream>
#include <string>

using namespace o2::mch::contour;
using namespace o2::mch::svg;
using namespace o2::mch::mapping;

void asSVG(std::ostream &out, const Segmentation &seg, int scale)
{
  std::vector<Contour<double>> dualSampaContours;
  std::vector<Polygon<double>> polygons;
  std::vector<std::vector<Polygon<double>>> dualSampaPads;

  for (auto i = 0; i < seg.nofDualSampas(); ++i) {
    std::vector<Polygon<double>> pads;
    seg.forEachPadInDualSampa(seg.dualSampaId(i), [&pads,&seg](int paduid) {
      double x = seg.padPositionX(paduid);
      double y = seg.padPositionY(paduid);
      double dx = seg.padSizeX(paduid)/2.0;
      double dy = seg.padSizeY(paduid)/2.0;

      pads.emplace_back(Polygon<double>{{x - dx, y - dy},
                                        {x + dx, y - dy},
                                        {x + dx, y + dy},
                                        {x - dx, y + dy},
                                        {x - dx, y - dy}});
    });
    dualSampaPads.push_back(pads);
    dualSampaContours.push_back(o2::mch::contour::createContour(pads));
    for (auto& p : dualSampaContours.back().getPolygons()) {
      polygons.push_back(p);
    }
  }

  auto env = o2::mch::contour::createContour(polygons);
  auto box = getBBox(env);
  writeHeader(out, box, scale);
  for (auto i = 0; i < dualSampaContours.size(); ++i) {
    auto &c = dualSampaContours[i];
    // pads
    for (auto &p: dualSampaPads[i]) {
      writePolygon(out, scale, p, box, "fill:none;stroke-width:0.5px;stroke-opacity:0.5;stroke:#666666");
    }

    // dual sampas
    std::string style = "fill:none;stroke:black;stroke-width:1px";
    writeContour(out, scale, c, box, style.c_str());

    // detection element
    writeContour(out, scale, env, box, "fill:none;stroke:blue;stroke-width:2px");
  }
}

int main(int argc, char *argv[])
{
  int detElemId = atoi(argv[1]);
  bool isBendingPlane = atoi(argv[2]);

  Segmentation seg{detElemId, isBendingPlane};

  std::ofstream out("seg-" + std::to_string(detElemId) + "-" + (isBendingPlane ? "B" : "NB") + ".html");
  int scale = 10;
  out << "<html><body style=\"background:white\">\n";
  asSVG(out, seg, scale);
  out << "</svg>\n";
  out << "</body></html>\n";
  return 0;
}