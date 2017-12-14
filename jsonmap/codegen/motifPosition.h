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


#ifndef ALO_JSONMAP_CODEGEN_MOTIFPOSITIONS_H
#define ALO_JSONMAP_CODEGEN_MOTIFPOSITIONS_H

#include <vector>
#include <string>
#include "rapidjson/document.h"

struct MotifPosition
{
  public:
    MotifPosition(int f, int m, int p, double x, double y) :
      mFECId(f), mMotifTypeId(m), mPadSizeId(p), mPositionX(x), mPositionY(y)
    {

    }

    MotifPosition(int f, int m, int p1, double x, double y,
                  int p2, std::vector<int> list) :
      mFECId(f), mMotifTypeId(m), mPadSizeId(p1), mPositionX(x), mPositionY(y),
      mSecondPadSizeId(p2), mPadNumbers(list)
    {

    }

    int FECId() const
    { return mFECId; }

    void secondPadSize(int p2,
                       std::vector<int> padnumbers_)
    {
      mSecondPadSizeId = p2;
      mPadNumbers = padnumbers_;
    }

    bool hasTwoPadSizes() const
    { return !mPadNumbers.empty(); }

    friend std::ostream &operator<<(std::ostream &os, const MotifPosition &position);

  private:
    int mFECId;
    int mMotifTypeId;
    int mPadSizeId;
    double mPositionX;
    double mPositionY;
    int mSecondPadSizeId;
    std::vector<int> mPadNumbers;
};

std::vector<MotifPosition>
getMotifPositions(int segtype, bool bending, const rapidjson::Value &segmentations, const rapidjson::Value &motiftypes,
                  const rapidjson::Value &padsizes);

void generateCodeForMotifPositions(
  const rapidjson::Value &segmentations,
  const rapidjson::Value &motiftypes,
  const rapidjson::Value &padsizes,
  const rapidjson::Value &bergs);

#endif
