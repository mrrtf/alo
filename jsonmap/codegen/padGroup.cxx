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

#include "padGroup.h"
#include <vector>

namespace jsonmap {
namespace codegen {
namespace impl2 {

std::ostream &operator<<(std::ostream &os, const PadGroup &pg)
{
  os << "{" << pg.fecId << "," << pg.padGroupTypeId << "," << pg.padSizeId << "," << pg.x << "," << pg.y << "}";
  return os;
}

void getPosition(const MotifPosition &mp, const PadGroupType &pgt, double &x, double &y)
{
  // once again, don't fret about some hard-coded stuff, that's faster sometimes ;-)

  x = mp.mPositionX;
  y = mp.mPositionY;

  using shifts = std::tuple<std::string, int, double, double>;

  //                            motiftype, motiftypeid, shift in x, shift in y (cm)
  std::vector<shifts> special = {{"1BG", 16, 2.52 * 3, 0.0},
                                 {"1NG", 16, 0.0,      1.68 * 3},
                                 {"1NH", 12, 0.0,      1.68 * 3},
                                 {"E14", 7,  1 * 2.5,  10 * 0.5},
                                 {"E15", 4,  1 * 2.5,  5 * 0.5}};

  for (auto &s : special) {
    if (pgt.originalMotifTypeIdString == std::get<0>(s) &&
        pgt.channelId.size() == std::get<1>(s)) {
      x += std::get<2>(s);
      y += std::get<3>(s);
    }
  }
}


int getPadSizeId(const MotifPosition &mp, const PadGroupType &pgt, int channel,
                 const std::map<int, int> &manu2berg)
{
  if (mp.hasTwoPadSizes()) {
    int berg = manu2berg.find(channel)->second;
    auto it = std::find(mp.mPadNumbers.begin(), mp.mPadNumbers.end(), berg);
    if (it != mp.mPadNumbers.end()) {
      return mp.mSecondPadSizeId;
    }
  }
  return mp.mPadSizeId;
}

std::vector<PadGroup>
getPadGroups(const std::vector<MotifPosition> &motifPositions, const std::vector<PadGroupType> &padGroupTypes,
             const std::map<int, int> &manu2berg)
{
  std::vector<PadGroup> pgs;
  for (auto i = 0; i < motifPositions.size(); ++i) {
    const auto &mp = motifPositions[i];
    int motifTypeId = mp.mMotifTypeId;
    for (auto &pgt : padGroupTypes) {
      if (pgt.originalMotifTypeId == motifTypeId) {
        int padSizeId{getPadSizeId(mp, pgt, pgt.channelId[0], manu2berg)};
        double x, y;
        getPosition(mp, pgt, x, y);
        pgs.push_back({mp.mFECId,pgt.id,padSizeId,static_cast<float>(x),static_cast<float>(y)});
      }
    }
  }
  return pgs;
}
}
}
}
