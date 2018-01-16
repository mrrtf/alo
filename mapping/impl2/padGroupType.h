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
#include <ostream>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

class PadGroupType
{
  public:
    PadGroupType(int nofPadsX, int nofPadsY, const std::vector<int>& ids);

    int getNofPads() const
    { return mNofPads; }

    int getNofPadsX() const
    {
      return mNofPadsX;
    }

    int getNofPadsY() const
    {
        return mNofPadsY;
    }

    /// Return the index of the pad with indices = (ix,iy)
    /// or -1 if not found
    int padIdByIndices(int ix, int iy) const;

    /// Whether pad with given id exists
    bool hasPadById(int id) const;

    friend std::ostream &operator<<(std::ostream &os, const PadGroupType &type);

  private:
    int getIndex(int ix, int iy) const;

    std::vector<int> mFastId;
    int mNofPads;
    int mNofPadsX;
    int mNofPadsY;
};

PadGroupType getPadGroupType(int i);

}
}
}
}

#endif
