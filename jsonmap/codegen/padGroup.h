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

#ifndef ALO_JSONMAP_CODEGEN_PADGROUP_H
#define ALO_JSONMAP_CODEGEN_PADGROUP_H

#include "rapidjson/document.h"
#include "motifPosition.h"
#include "padGroupType.h"
#include <map>
#include <vector>
#include <ostream>

namespace jsonmap {
namespace codegen {

class PadSize;

namespace impl2 {


struct PadGroup
{
    int fecId;
    int padGroupTypeId;
    int padSizeId;
    float x;
    float y;
};

std::vector<PadGroup>
getPadGroups(const std::vector<MotifPosition> &motifPositions, const std::vector<PadGroupType> &padGroupTypes,
             const std::map<int, int> &manu2berg);

std::vector<PadGroupType>
getPadGroupTypes(const std::vector<PadGroup> &padGroups, const std::vector<PadGroupType> &padGroupTypes);

std::vector<PadSize>
getPadSizes(const std::vector<PadGroup> &padGroups, const std::vector<PadSize> &allPadSizes);

std::vector<PadGroup> remap(const std::vector<PadGroup> &padGroups, const std::vector<PadGroupType> &padGroupTypes,
                            const std::vector<PadSize> &padSizes);

std::ostream &operator<<(std::ostream &os, const PadGroup &pg);

}
}
}
#endif
