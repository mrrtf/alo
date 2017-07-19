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
namespace o2 {
namespace mch {
namespace mapping {

class MotifPosition
{
  public:
    explicit MotifPosition(float x, float y, int motifTypeId, int padSizeId, int fecId);

    float getX() const { return mX; }
    float getY() const { return mY; }
    int getMotifTypeId() const { return mMotifTypeId; }
    int getPadSizeId() const { return mPadSizeId; }
    int getFecId() const { return mFecId; }


    static int getMotifTypeIdMax()
    {
      return gMotifTypeIdMax;
    }

    static int getPadSizeIdMax()
    {
      return gPadSizeIdMax;
    }

    static int getFecIdMax()
    {
      return gFecIdMax;
    }

  private:
    float mX;
    float mY;
    int mMotifTypeId;
    int mPadSizeId;
    int mFecId;

    static int gMotifTypeIdMax;
    static int gPadSizeIdMax;
    static int gFecIdMax;
};


}
}
}

#endif
