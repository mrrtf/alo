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


#ifndef ALO_JSONMAP_CODEGEN_PADGROUPTYPE_H
#define ALO_JSONMAP_CODEGEN_PADGROUPTYPE_H

#include "rapidjson/document.h"
#include <map>
#include <string>
#include <vector>

namespace jsonmap {
namespace codegen {
namespace impl2 {

struct PadGroupType
{
    int id;
    int originalMotifTypeId;
    std::string originalMotifTypeIdString;
    bool isSplit;
    std::vector<int> channelId;
    std::vector<int> ix;
    std::vector<int> iy;

    int nofPadsX() const;
    int nofPadsY() const;

    std::vector<int> fastIndex() const;

    int getIndex(int ix, int iy) const;
    
    void clear()
    {
      channelId.clear();
      ix.clear();
      iy.clear();
    }
};

std::map<int, int> getBerg2Manu(const rapidjson::Value &berg, bool is80pins);

std::vector<PadGroupType> getPadGroupTypes(const rapidjson::Value &motiftypes, const rapidjson::Value& bergs);

std::string generateCodeForPadGroupTypes(const rapidjson::Value &motiftypes, const rapidjson::Value& bergs);
}
}
}

#endif

