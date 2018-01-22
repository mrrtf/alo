//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In apying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche

#include "segmentationContours.h"
#include "contourCreator.h"
#include "polygon.h"

namespace o2 {
namespace mch {
namespace mapping {

std::vector<o2::mch::contour::Contour<double>> getSampaContours(const Segmentation& seg)
{
  std::vector<o2::mch::contour::Contour<double>> contours;
  for (auto i = 0; i < seg.nofDualSampas(); ++i) {
    std::vector<o2::mch::contour::Polygon<double>> cpads;
    seg.forEachPadInDualSampa(seg.dualSampaId(i),
                              [&](PadHandle ph) {
                                double x = seg.padPositionX(ph);
                                double y = seg.padPositionY(ph);
                                double dx = seg.padSizeX(ph) / 2.0;
                                double dy = seg.padSizeY(ph) / 2.0;
                                double xBottomLeft = x - dx;
                                double yBottomLeft = y - dy;
                                double xTopRight = x + dx;
                                double yTopRight = y + dy;
                                cpads.push_back({
                                                  {xBottomLeft, yBottomLeft},
                                                  {xTopRight,   yBottomLeft},
                                                  {xTopRight,   yTopRight},
                                                  {xBottomLeft, yTopRight},
                                                  {xBottomLeft, yBottomLeft}
                                                });
                              });
    contours.push_back(o2::mch::contour::createContour(cpads));
  }
  return contours;
}

}
}
}
