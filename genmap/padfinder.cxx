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

#include <cmath>
#include "padfinder.h"
#include "MotifPosition.h"
#include "genPadSize.h"

namespace o2 {
namespace mch {
namespace mapping {

int padIdByPosition(const MotifPosition& mp, const MotifTypeArray& motifTypes, const PadSizeArray& padsizes, float x,
                    float y)
{
  std::cout << mp.getFecId() << std::endl;
  
  int localX = std::floor((x - mp.getX())*1E4);
  if (localX < 0 ){
    return -1;
  }

  int localY = std::floor((y - mp.getY())*1E4);
  if (localY < 0 ){
    return -1;
  }

  std::pair<int,int> size = padsizes[mp.getPadSizeId()];

  int px = padsizes[mp.getPadSizeId()].first;
  int py = padsizes[mp.getPadSizeId()].second;

  int ix = localX/px;
  int iy = localY/py;

  const MotifType& mt = motifTypes[mp.getMotifTypeId()];

  return mt.padIdByIndices(ix,iy);
}

}
}
}
