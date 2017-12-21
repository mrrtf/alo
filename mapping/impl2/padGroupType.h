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


#ifndef O2_MCH_MAPPING_PADGROUPTYPE_H
#define O2_MCH_MAPPING_PADGROUPTYPE_H

#include <vector>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

class PadGroupType
{
  public:
    PadGroupType(const std::vector<int> &ids, const std::vector<int> &ix, const std::vector<int> &iy);

    int getNofPads() const
    { return mId.size(); }

    int getNofPadsX() const
    {
      return mNofPadsX;
    }

    int getNofPadsY() const;

    int getId(int i) const
    { return mId[i]; }

    int getIx(int i) const
    { return mIx[i]; }

    int getIy(int i) const
    { return mIy[i]; }

    /// Return the index of the pad with indices = (ix,iy)
    /// or -1 if not found
    int padIdByIndices(int ix, int iy) const;

  private:
    int getIndex(int ix, int iy) const;

  private:
    std::vector<int> mId;
    std::vector<int> mIx;
    std::vector<int> mIy;
    std::vector<int> mIndex;
    int mNofPadsX;
};

}
}
}
}

#endif
