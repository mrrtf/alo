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


#ifndef O2_MCH_MAPPING_SEGMENTATION_H
#define O2_MCH_MAPPING_SEGMENTATION_H

#include "HasIntegerIdWithinGivenRange.h"

namespace o2 {
namespace mch {
namespace mapping {

class Segmentation : public HasIntegerIdWithinGivenRange<0, 20>
{
  public:

    enum PlaneType { kBendingPlane, kNonBendingPlane };

    explicit Segmentation(int id, PlaneType planeType) : HasIntegerIdWithinGivenRange(id), mPlaneType(planeType) {}

    int NofDualSampas() const;

  private:
    PlaneType mPlaneType;
};

}
}
}

#endif
