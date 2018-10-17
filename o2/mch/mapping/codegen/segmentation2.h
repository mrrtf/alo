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


#ifndef ALO_JSONMAP_CODEGEN_SEGMENTATION2_H
#define ALO_JSONMAP_CODEGEN_SEGMENTATION2_H

#include <iostream>
#include <map>
#include <string>
#include "padGroupType.h"
#include "rapidjson/document.h"

namespace jsonmap {
namespace codegen {
namespace impl2 {

void generateCodeForSegmentations(const std::string& ns,
                                  const rapidjson::Value &segmentations,
                                  const rapidjson::Value &motiftypes,
                                  const rapidjson::Value &padsizes,
                                  const rapidjson::Value &detection_elements,
                                  const rapidjson::Value &bergs);

std::map<int, int> getManu2Berg(const rapidjson::Value &bergs, bool is80pins);

}
}
}

#endif
