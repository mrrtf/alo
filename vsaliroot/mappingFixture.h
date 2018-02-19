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

#ifndef ALO_VSALIROOT_MAPPINGFIXTURE_H
#define ALO_VSALIROOT_MAPPINGFIXTURE_H

class AliMpDDLStore;

class AliMpSegmentation;

class AliMpVSegmentation;

class AliMpPad;

#include "segmentation.h"

#include <vector>
#include <array>

struct MAPPING
{
    MAPPING();

    static AliMpDDLStore *ddlStore;
    static AliMpSegmentation *mseg;

    //static std::vector<int> detElemIds;

    static constexpr std::array<int, 2> st12DetElemIds{100, 300};
    static constexpr std::array<int, 19> st345DetElemIds{
      500, 501, 502, 503, 504, 600, 601, 602, 700, 701, 702, 703, 704, 705, 706, 902, 903, 904, 905
    };
    static constexpr std::array<int, 21> detElemIds{
      100, 300, 500, 501, 502, 503, 504, 600, 601, 602, 700, 701, 702, 703, 704, 705, 706, 902, 903, 904, 905
    };

};

const AliMpVSegmentation *alirootSegmentation(AliMpSegmentation *mseg, int detElemId, bool isBendingPlane);

bool comparePads(const AliMpPad &alPad, const o2::mch::mapping::Segmentation &o2Seg, int paduid);

#endif //ALO_MAPPINGFIXTURE_H
