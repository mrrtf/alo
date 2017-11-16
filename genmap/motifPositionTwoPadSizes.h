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


#ifndef O2_MCH_MAPPING_MOTIFPOSITIONTWOPADSIZES_H
#define O2_MCH_MAPPING_MOTIFPOSITIONTWOPADSIZES_H

#include <vector>
#include "genMotifType.h"
#include "pad.h"
#include <ostream>

namespace o2 {
namespace mch {
namespace mapping {

class MotifPositionTwoPadSizes
{
  public:

    MotifPositionTwoPadSizes()
    {}

    MotifPositionTwoPadSizes(int f, int m, double padsizex, double padsizey, double x, double y) :
      mFECId(f), mMotifTypeId(m), mPadSizeX({padsizex}), mPadSizeY({padsizey}), mPositionX(x), mPositionY(y)
    {

    }

    MotifPositionTwoPadSizes(int f, int m, double padsizex, double padsizey, double x, double y,
                             double secondpadsizex, double secondpadsizey, std::vector<int> bergnumbers) :
      mFECId(f), mMotifTypeId(m), mPadSizeX({padsizex, secondpadsizex}), mPadSizeY({padsizey, secondpadsizey}),
      mPositionX(x), mPositionY(y), mBergNumbers(bergnumbers)
    {

    }

    int FECId() const
    { return mFECId; }

    int motifTypeId() const
    { return mMotifTypeId; }

    friend std::ostream &operator<<(std::ostream &os, const MotifPositionTwoPadSizes &position);

    double positionX() const
    { return mPositionX; }

    double positionY() const
    { return mPositionY; }

    double padSizeX(int bergNumber) const
    { return mPadSizeX[padSizeIndex(bergNumber)]; }

    double padSizeY(int bergNumber) const
    { return mPadSizeY[padSizeIndex(bergNumber)]; }

    std::vector<Pad> getPads(const MotifType &mt) const
    {
      std::vector<Pad> pads;
      double padsizex{mPadSizeX[0]};
      double padsizey{mPadSizeY[0]};
      double padx{0.0};
      double pady{0.0};
      for (int iy = 0; iy < mt.getNofPadsY(); ++iy) {
        padx = 0.0;
        for (int ix = 0; ix < mt.getNofPadsX(); ++ix) {
          int index = mt.padIdByIndices(ix, iy);
          if (index >= 0) {
            int bergNumber = mt.getBerg(index);
            padsizex = padSizeX(bergNumber);
            padsizey = padSizeY(bergNumber);
            pads.push_back({padx, pady, padx + padsizex, pady + padsizey});
          }
          padx += padsizex;
        }
        pady += padsizey;
      }
      return pads;
    }

  private:

    int padSizeIndex(int bergNumber) const
    {
      if (mPadSizeY.size() > 1) {
        if (std::find(mBergNumbers.begin(), mBergNumbers.end(), bergNumber) != mBergNumbers.end()) {
          return 1;
        }
      }
      return 0;
    }

  private:
    int mFECId;
    int mMotifTypeId;
    std::vector<double> mPadSizeX;
    std::vector<double> mPadSizeY;
    double mPositionX;
    double mPositionY;
    std::vector<int> mBergNumbers;
};

}
}
}

#endif
