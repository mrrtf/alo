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

#include "genSegmentationInterface.h"
#include <fstream>
#include "svgContour.h"

namespace o2 {
namespace mch  {
namespace svg {

void writeSegmentationInterface(const o2::mch::mapping::SegmentationInterface &seg, const char *filename, double x, double y)
{
  auto contours = seg.getSampaContours();

  std::ofstream out(filename);
  int scale = 10;
  out << "<html><body>\n";
  auto env = seg.getEnvelop();
  auto box = getBBox(env);

  x -= box.xmin();
  y -= box.ymin();

  writeHeader(out, box, scale);
  writeContour(out, scale, env, box, "fill:#eeeeee;stroke:black;stroke-width:3px");
  for (auto i = 0; i < contours.size(); ++i) {
    auto &c = contours[i];
//  for (auto &&c: contours) {
    writeContour(out, scale, c, box);
    if (c.contains(x, y)) {
      writeContour(out, scale, c, box, "fill:#aaaaaa;stroke:none");
      writeContour(out, scale, seg.getSampaPads(i), box, "fill:yellow;stroke:blue");
    }
  }

  out << "<circle cx=\"" << scale * x << "\" cy=\"" << scale * y << "\" r=\"5\"\n"
    "style=\"fill:none;stroke:black;stroke-width:0.5px;\"/>";

  out << "<circle cx=\"" << scale * x << "\" cy=\"" << scale * y << "\" r=\"1\"\n"
    "style=\"fill:none;stroke:red;stroke-width:0.5px;\"/>";

  out << "</svg>\n";
  out << "</body></html>\n";
}

}}}
