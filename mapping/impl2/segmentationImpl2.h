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
#include "bbox.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

class Segmentation
{
  public:

    static constexpr int InvalidPadUid{-1};
    using Polygon = o2::mch::contour::Polygon<double>;
    using BBox = o2::mch::contour::BBox<double>;

    Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups);

    Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups,
                 std::vector<PadGroupType> padGroupTypes,
                 std::vector<std::pair<float, float>> padSizes);

    /// Return the list of paduids for the pads of the given dual sampa.
    std::vector<int> getPadUids(int dualSampaIds) const;

    /// Return the list of paduids for the pads contained in the box
    std::vector<int> getPadUids(const BBox &box) const;

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

    int padDualSampaId(int paduid) const;

    int padDualSampaChannel(int paduid) const;

    BBox bbox() const
    { return mBBox; }

    std::vector<int> intersect(const BBox &box) const;

  private:
    int dualSampaIndex(int dualSampaId) const;

    const PadGroup &padGroup(int paduid) const;

    const PadGroupType &padGroupType(int paduid) const;

    std::vector<int> findPadGroupIndices(double x, double y) const;

    int padUid2padGroupIndex(int paduid) const
    {
      return paduid / mMaxFastIndex;
    }

    int padUid2padGroupTypeFastIndex(int paduid) const
    {
      return paduid - padUid2padGroupIndex(paduid) * mMaxFastIndex;
    }

    int padUid(int padGroupIndex, int padGroupTypeFastIndex) const
    {

      return padGroupIndex * mMaxFastIndex + padGroupTypeFastIndex;
    }

    int padUid(int padGroupIndex, double x, double y) const;

    std::vector<int> uniqueAndValid(std::vector<int> paduids) const;

    bool isValid(int paduid) const
    { return paduid != InvalidPadUid; }

    int findClosestX(double xstart, double ystart, double xend) const;
    int findClosestY(double xstart, double ystart, double yend) const;

  private:
    int mSegType;
    bool mIsBendingPlane;
    std::vector<PadGroup> mPadGroups;
    std::set<int> mDualSampaIds;
    std::vector<PadGroupType> mPadGroupTypes;
    std::vector<std::pair<float, float>> mPadSizes;
    std::vector<Polygon> mPadGroupContours;
    Polygon mEnvelop;
    BBox mBBox;
    int mMaxFastIndex;
};

Segmentation *createSegmentation(int detElemId, bool isBendingPlane);
}
}
}
}
#endif
