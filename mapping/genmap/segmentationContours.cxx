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

#include "segmentationContours.h"
#include "segmentationInterface.h"
#include "motifPositionContours.h"
#include "segmentation.h"

namespace o2 {
namespace mch {
namespace mapping {

std::vector<o2::mch::contour::Contour<double>> getSampaContours(const o2::mch::mapping::SegmentationInterface &seg)
{
  std::vector<o2::mch::contour::Contour<double>> contours;
  for (auto i = 0; i < seg.nofDualSampas(); ++i) {
    auto padIndices = seg.getPads(seg.getSampaId(i));
    std::vector<Pad> pads;
    pads.reserve(padIndices.size());
    for (auto ph: padIndices) {
      double x,y;
      double dx, dy;
      seg.getPadPosition(ph,x, y);
      seg.getPadDimension(ph,dx,dy);
      pads.push_back({x - dx/2.0, y - dy/2.0, x + dx/2.0, y + dy/2.0});
    };
    contours.push_back(o2::mch::contour::createContour(padAsPolygons(pads)));
  }
  return contours;
}

std::vector<o2::mch::contour::Contour<double>> getSampaContours(const o2::mch::mapping::Segmentation &seg)
{
  std::vector<o2::mch::contour::Contour<double>> contours;
  for (auto i = 0; i < seg.nofDualSampas(); ++i) {
    std::vector<Pad> pads;
    seg.forEachPadInDualSampa(seg.dualSampaId(i),
                              [&](PadHandle ph) {
                                double x = seg.padPositionX(ph);
                                double y = seg.padPositionY(ph);
                                double dx = seg.padSizeX(ph)/2.0;
                                double dy = seg.padSizeY(ph)/2.0;
                                pads.push_back({x - dx, y - dy, x + dx, y + dy});
                              });
    contours.push_back(o2::mch::contour::createContour(padAsPolygons(pads)));
  }
  return contours;
}
}
}
}
