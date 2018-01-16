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


#ifndef O2_MCH_MAPPING_IMPL2_PADGROUPTYPECONTOUR_H
#define O2_MCH_MAPPING_IMPL2_PADGROUPTYPECONTOUR_H

#include "polygon.h"
#include "padGroupType.h"
#include <vector>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

std::vector<o2::mch::contour::Polygon<int>> computePads(const PadGroupType &pgt);

std::vector<o2::mch::contour::Polygon<int>> computeContours(const std::vector<PadGroupType> &padGroupTypes);

}
}
}
}

#endif
