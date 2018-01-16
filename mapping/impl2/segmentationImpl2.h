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
#include "contour.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

class Segmentation
{
  public:
    using Contour = o2::mch::contour::Polygon<double>;

    Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups);

    Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups,
                 std::vector<PadGroupType> padGroupTypes,
                 std::vector<std::pair<float, float>> padSizes);

    std::vector<int> padGroupIndices(int dualSampaId) const;

    int nofPads(const PadGroup &padGroup) const;

    PadGroup padGroup(int index) const
    { return mPadGroups[index]; }

    Contour padGroupContour(const PadGroup &padGroup) const;

    std::set<int> dualSampaIds() const
    { return mDualSampaIds; }

    bool hasPadByPosition(double x, double y) const;

    bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const;

  private:
    int dualSampaIndex(int dualSampaId) const;

  private:
    int mSegType;
    bool mIsBendingPlane;
    std::set<int> mDualSampaIds;
    std::vector<PadGroupType> mPadGroupTypes;
    std::vector<std::pair<float, float>> mPadSizes;
    std::vector<PadGroup> mPadGroups;
    std::vector<Contour> mPadGroupContours;
};

Segmentation *createSegmentation(int detElemId, bool isBendingPlane);
Segmentation *createSegmentation2(int detElemId, bool isBendingPlane);

}
}
}
}
#endif
