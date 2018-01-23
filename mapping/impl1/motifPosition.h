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


#ifndef O2_MCH_MAPPING_IMPL1_MOTIFPOSITION_H
#define O2_MCH_MAPPING_IMPL1_MOTIFPOSITION_H

#include "pad.h"
#include <vector>
#include "motifType.h"
#include "padSize.h"
#include <ostream>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl1 {

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

    int padSizeId() const
    { return mPadSizeId; }

    std::vector<Pad> getPads(const MotifType &mt) const
    {
      const double eps{1E-4}; // artificially increase pad size by 1micron to avoid gaps between motifpositions
      std::vector<Pad> pads;
      pads.reserve(mt.getNofPads());
      double padsizex{o2::mch::mapping::impl1::padSizeX(mPadSizeId)};
      double padsizey{o2::mch::mapping::impl1::padSizeY(mPadSizeId)};
      for (int i = 0; i < mt.getNofPads(); ++i) {
        double padx = mt.getIx(i) * padsizex;
        double pady = mt.getIy(i) * padsizey;
        pads.push_back({padx - eps, pady - eps, padx + padsizex + eps, pady + padsizey + eps});
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
    { return o2::mch::mapping::impl1::padSizeX(padSizeIndex(bergNumber)); }

    double padSizeY(int bergNumber) const
    { return o2::mch::mapping::impl1::padSizeY(padSizeIndex(bergNumber)); }

    bool hasTwoPadSizes() const
    { return mPadSizeId.size() > 1; }

    int padSizeId() const
    { return mPadSizeId[0]; }

    std::vector<Pad> getPads(const MotifType &mt) const
    {
      if (hasTwoPadSizes()) {
        std::vector<Pad> pads;
        const double eps{1E-4}; // artificially increase pad size by 1micron to avoid gaps between motifpositions
        double padx{0.0};
        double pady{0.0};
        double padsizey{o2::mch::mapping::impl1::padSizeY(mPadSizeId[0])};
        double padsizex{o2::mch::mapping::impl1::padSizeX(mPadSizeId[1])};
        for (int iy = 0; iy < mt.getNofPadsY(); ++iy) {
          padx = 0.0;
          for (int ix = 0; ix < mt.getNofPadsX(); ++ix) {
            int index = mt.padIdByIndices(ix, iy);
            if (index >= 0) {
              int bergNumber = mt.getBerg(index);
              padsizex = padSizeX(bergNumber);
              padsizey = padSizeY(bergNumber);
              pads.push_back({padx - eps, pady - eps, padx + padsizex + eps, pady + padsizey + eps});
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
          return mPadSizeId[1];
        }
      }
      return mPadSizeId[0];
    }

  private:
    int mFECId;
    int mMotifTypeId;
    std::vector<int> mPadSizeId;
    double mPositionX;
    double mPositionY;
    std::vector<int> mBergNumbers;
};

template<int VAL, typename MP=MotifPosition, int (*F)(int) = nullptr>
struct MotifPositionBaseTrait
{
    static constexpr int value{VAL};
    using array_type = std::array<MP, value>;
    using type = MP;

    static constexpr int (*func)(int) = F;
};

template<int, bool>
struct MotifPositionTrait
{
};

template<int SEGTYPE, bool ISBENDING>
typename MotifPositionTrait<SEGTYPE, ISBENDING>::array_type getMotifPositions();

#include "genMotifPosition.h"

}
}
}
}

#endif
