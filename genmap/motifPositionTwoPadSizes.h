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
#include "motifPosition.h"

namespace o2 {
namespace mch {
namespace mapping {

class MotifPositionTwoPadSizes
{
  public:

    MotifPositionTwoPadSizes()
    {
    }

    MotifPositionTwoPadSizes(int f, int m, int p, double x, double y) :
      mFECId(f), mMotifTypeId(m), mPadSizeId({p}), mPositionX(x), mPositionY(y)
    {

    }

    MotifPositionTwoPadSizes(int f, int m, int p1, double x, double y,
                             int p2, std::vector<int> bergnumbers) :
      mFECId(f), mMotifTypeId(m), mPadSizeId({p1, p2}),
      mPositionX(x), mPositionY(y), mBergNumbers(bergnumbers)
    {
//      if (o2::mch::mapping::padSizeX(mPadSizeId[0]) > o2::mch::mapping::padSizeX(mPadSizeId[1])
//          || o2::mch::mapping::padSizeX(mPadSizeId[0]) > o2::mch::mapping::padSizeX(mPadSizeId[1])) {
//        std::swap(mPadSizeId[0], mPadSizeId[1]);
//      }
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
    { return o2::mch::mapping::padSizeX(padSizeIndex(bergNumber)); }

    double padSizeY(int bergNumber) const
    { return o2::mch::mapping::padSizeY(padSizeIndex(bergNumber)); }

    bool hasTwoPadSizes() const
    { return mPadSizeId.size() > 1; }

    std::vector<Pad> getPads(const MotifType &mt) const
    {
      if (hasTwoPadSizes()) {
        std::cout << "nx=" << mt.getNofPadsX() << " ny=" << mt.getNofPadsY() << "\n";
        std::vector<Pad> pads;
        const double eps{1E-4}; // artificially increase pad size by 1micron to avoid gaps between motifpositions
        double padx{0.0};
        double pady{0.0};
        double padsizey{0.0};
        double padsizex{0.0};
        for (int iy = 0; iy < mt.getNofPadsY(); ++iy) {
          padx = 0.0;
          for (int ix = 0; ix < mt.getNofPadsX(); ++ix) {
            int index = mt.padIdByIndices(ix, iy);
            if (1==1) { //index >= 0) {
              int bergNumber = mt.getBerg(index);
              padsizex = padSizeX(bergNumber);
              padsizey = padSizeY(bergNumber);
              pads.push_back({padx - eps, pady - eps, padx + padsizex + eps, pady + padsizey + eps});
              std::cout << "ix=" << ix << " iy=" << iy << " padx=" << padx << " pady=" << pady << "\n";
            }
            padx += padsizex;
          }
          pady += padsizey;
        }
        return pads;
      } else {
        MotifPosition mp(FECId(), motifTypeId(), mPadSizeId[0], positionX(), positionY());
        return mp.getPads(mt);
      }
    }

  private:

    int padSizeIndex(int bergNumber) const
    {
      if (mPadSizeId.size() > 1) {
        if (std::find(mBergNumbers.begin(), mBergNumbers.end(), bergNumber) != mBergNumbers.end()) {
          return 1;
        }
      }
      return 0;
    }

  private:
    int mFECId;
    int mMotifTypeId;
    std::vector<int> mPadSizeId;
    double mPositionX;
    double mPositionY;
    std::vector<int> mBergNumbers;
};

}
}
}

#endif
