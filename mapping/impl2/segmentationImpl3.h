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
#include "padGroupType.h"
#include <vector>
#include <set>
#include <ostream>
#include "polygon.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

class Segmentation
{
  public:

    static constexpr int InvalidPadUid{-1};
    using Contour = o2::mch::contour::Polygon<double>;

    Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups);

    Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups,
                 std::vector<PadGroupType> padGroupTypes,
                 std::vector<std::pair<float, float>> padSizes);

    std::vector<int> getPadUids(int dualSampaIds) const;

    std::set<int> dualSampaIds() const
    { return mDualSampaIds; }

    int findPadByPosition(double x, double y) const;

    int findPadByFEE(int dualSampaId, int dualSampaChannel) const;

    bool hasPadByPosition(double x, double y) const
    { return findPadByPosition(x, y) != InvalidPadUid; }

    bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const
    { return findPadByFEE(dualSampaId, dualSampaChannel) != InvalidPadUid; }

    friend std::ostream &operator<<(std::ostream &os, const Segmentation &seg);

    double padPositionX(int paduid) const;

    double padPositionY(int paduid) const;

    double padSizeX(int paduid) const;

    double padSizeY(int paduid) const;

  private:
    int dualSampaIndex(int dualSampaId) const;

    const PadGroup &padGroup(int paduid) const;

    const PadGroupType &padGroupType(int paduid) const;

    int findPadGroupIndex(double x, double y) const;

    int padUid2padGroupIndex(int paduid) const
    {
      return paduid / mMaxFastIndex;
    }

    int padUid2padGroupTypeFastIndex(int paduid) const
    {
      return paduid - padUid2padGroupIndex(paduid) * mMaxFastIndex;
    }
    
    int padUid(int padGroupIndex, int padGroupTypeFastIndex) const {

      return padGroupIndex * mMaxFastIndex + padGroupTypeFastIndex;
    }


  private:
    int mSegType;
    bool mIsBendingPlane;
    std::vector<PadGroup> mPadGroups;
    std::set<int> mDualSampaIds;
    std::vector<PadGroupType> mPadGroupTypes;
    std::vector<std::pair<float, float>> mPadSizes;
    std::vector<Contour> mPadGroupContours;
    int mMaxFastIndex;
};

Segmentation *createSegmentation(int detElemId, bool isBendingPlane);
}
}
}
}
#endif
