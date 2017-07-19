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

#include <algorithm>
#include "MotifPosition.h"

namespace o2 {
namespace mch {
namespace mapping {

 int MotifPosition::gMotifTypeIdMax{0};
 int MotifPosition::gPadSizeIdMax{0};
 int MotifPosition::gFecIdMax{0};

MotifPosition::MotifPosition(float x, float y, int motifTypeId, int padSizeId, int fecId) :
  mX(x), mY(y), mMotifTypeId(motifTypeId), mPadSizeId(padSizeId), mFecId(fecId)
{
  gMotifTypeIdMax = std::max(gMotifTypeIdMax,mMotifTypeId);
  gPadSizeIdMax = std::max(gPadSizeIdMax,mPadSizeId);
  gFecIdMax = std::max(gFecIdMax,mFecId);
}


}
}
}
