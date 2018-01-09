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
#include "padGroup.h"
#include <iostream>
#include <memory>
#include <set>
#include <string>

#include "genSegmentation.cxx"

using namespace o2::mch::mapping::impl2;

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

Segmentation::Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups) :
  mSegType{segType}, mIsBendingPlane{isBendingPlane}, mPadGroups{padGroups}, mDualSampaIds{}
{
  for (auto& pg: mPadGroups) {
    mDualSampaIds.insert(pg.mFECId);
  }
}

PadGroup Segmentation::padGroup(int index) const
{
  return mPadGroups[index];
}

std::vector<int> Segmentation::padGroupIndices(int dualSampaId) const
{
  std::vector<int> rv;
  for (auto i = 0; i < mPadGroups.size(); ++i) {
    if (mPadGroups[i].mFECId == dualSampaId) {
      rv.push_back(i);
    }
  }
  return rv;
}

bool Segmentation::hasPadByPosition(double x, double y) const
{
  return false;
}


}
}
}
}

