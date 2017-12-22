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


#ifndef O2_MCH_MAPPING_IMPL2_SEGMENTATION_H
#define O2_MCH_MAPPING_IMPL2_SEGMENTATION_H

#include "padGroup.h"
#include <vector>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

class Segmentation {

  public:
    Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups);

    int nofDualSampas() const { return mDualSampaId.size();  }

    int dualSampaId(int dualSampaIndex) const {
      return mDualSampaId[dualSampaIndex];
    }

    std::vector<int> padGroupIndices(int dualSampaId) const;

    PadGroup padGroup(int index) const;

  private:
    int mSegType;
    bool mIsBendingPlane;
    std::vector<PadGroup> mPadGroups;
    std::vector<int> mDualSampaId;
};

Segmentation* createSegmentation(int detElemId, bool isBendingPlane);

}
}
}
}
#endif
