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

namespace o2 {
namespace mch {
namespace mapping {

std::vector<o2::mch::contour::Contour<double>> getSampaContours(const o2::mch::mapping::SegmentationInterface &seg)
{
  std::vector<o2::mch::contour::Contour<double>> contours;
  for (auto i = 0; i < seg.nofDualSampas(); ++i) {
    auto pads = seg.getPads(seg.getSampaId(i));
    contours.push_back(o2::mch::contour::createContour(padAsPolygons(pads)));
  }
  return contours;
}

}
}
}
