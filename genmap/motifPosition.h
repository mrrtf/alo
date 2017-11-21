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
#include "genMotifType.h"
#include <ostream>

namespace o2 {
namespace mch {
namespace mapping {

class MotifPosition
{
  public:

    MotifPosition()
    {}

    MotifPosition(int f, int m, double padsizex, double padsizey, double x, double y) :
      mFECId(f), mMotifTypeId(m), mPadSizeX(padsizex), mPadSizeY(padsizey), mPositionX(x), mPositionY(y)
    {

    }

    int FECId() const
    { return mFECId; }

    int motifTypeId() const
    { return mMotifTypeId; }

    bool hasTwoPadSizes() const
    { return false; }

    friend std::ostream &operator<<(std::ostream &os, const MotifPosition &position);

    double padSizeX() const
    { return mPadSizeX; }

    double padSizeY() const
    { return mPadSizeY; }

    double positionX() const
    { return mPositionX; }

    double positionY() const
    { return mPositionY; }

    std::vector<Pad> getPads(const MotifType &mt) const
    {
      const double eps{1E-4}; // artificially increase pad size by 1micron to avoid gaps between motifpositions
      std::vector<Pad> pads;
      double padsizex{padSizeX()};
      double padsizey{padSizeY()};
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
    double mPadSizeX;
    double mPadSizeY;
    double mPositionX;
    double mPositionY;
};

}
}
}

#endif
