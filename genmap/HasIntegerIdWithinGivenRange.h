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


#ifndef O2_MUON_MCH_MAPPING_HASINTEGERIDWITHINGIVENRANGE_H
#define O2_MUON_MCH_MAPPING_HASINTEGERIDWITHINGIVENRANGE_H

#include <stdexcept>

namespace o2 {
namespace mch {
namespace mapping {

template<int MIN, int MAX>
class HasIntegerIdWithinGivenRange
{
  public:
    explicit HasIntegerIdWithinGivenRange(int id) : mId(id)
    {
      if (!IsValidId(mId)) { throw std::out_of_range{"id is not in the expected range"}; }
    }

    int GetId() const
    { return mId; }

    static bool IsValidId(int id)
    { return gMin <= id && id <= gMax; }

    static int GetMinValidId()
    { return gMin; }

    static int GetMaxValidId()
    { return gMax; }

  private:
    static constexpr int gMin = MIN;
    static constexpr int gMax = MAX;
    int mId;
};

}
}
}

#endif
