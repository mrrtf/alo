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


#ifndef O2_MCH_MAPPING_MOTIFPOSITION_H
#define O2_MCH_MAPPING_MOTIFPOSITION_H

#include "pad.h"
#include <vector>
#include "motifType.h"
#include "padSize.h"
#include <ostream>

namespace o2 {
namespace mch {
namespace mapping {

class MotifPosition
{
  public:

    MotifPosition()
    {}

    MotifPosition(int f, int m, int p, double x, double y) :
      mFECId(f), mMotifTypeId(m), mPadSizeId(p), mPositionX(x), mPositionY(y)
    {

    }

    int FECId() const
    { return mFECId; }

    int motifTypeId() const
    { return mMotifTypeId; }

    bool hasTwoPadSizes() const
    { return false; }

    friend std::ostream &operator<<(std::ostream &os, const MotifPosition &position);

    double positionX() const
    { return mPositionX; }

    double positionY() const
    { return mPositionY; }

    int padSizeId() const { return mPadSizeId; }

    std::vector<Pad> getPads(const MotifType &mt) const
    {
      const double eps{1E-4}; // artificially increase pad size by 1micron to avoid gaps between motifpositions
      std::vector<Pad> pads;
      pads.reserve(mt.getNofPads());
      double padsizex{o2::mch::mapping::padSizeX(mPadSizeId)};
      double padsizey{o2::mch::mapping::padSizeY(mPadSizeId)};
      for (int i = 0; i < mt.getNofPads(); ++i) {
        double padx = mt.getIx(i) * padsizex;
        double pady = mt.getIy(i) * padsizey;
        pads.push_back({padx-eps, pady-eps, padx + padsizex + eps, pady + padsizey +eps});
      }
      return pads;
    }

  private:
    int mFECId;
    int mMotifTypeId;
    int mPadSizeId;
    double mPositionX;
    double mPositionY;
};

}
}
}

#endif
