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

#include "motifPositionContours.h"

namespace o2 {
namespace mch {
namespace mapping {

std::vector<o2::mch::contour::Polygon<double>> padAsPolygons(const std::vector<Pad> &pads)
{
  std::vector<o2::mch::contour::Polygon<double>> cpads;
  for (auto &&p: pads) {
    cpads.push_back({
                      {p.xBottomLeft, p.yBottomLeft},
                      {p.xTopRight,   p.yBottomLeft},
                      {p.xTopRight,   p.yTopRight},
                      {p.xBottomLeft, p.yTopRight},
                      {p.xBottomLeft, p.yBottomLeft}
                    });
  }
  return cpads;
}

}
}
}
