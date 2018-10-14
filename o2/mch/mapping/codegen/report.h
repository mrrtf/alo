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


#ifndef ALO_JSONMAP_CODEGEN_REPORT_H
#define ALO_JSONMAP_CODEGEN_REPORT_H

#include <set>
#include <map>
#include "rapidjson/document.h"
#include "motifPosition.h"

namespace jsonmap {
namespace codegen {

void reportNonRegularMotifs(const std::vector <MotifPosition> &motifpositions,
                            std::set<int> &regulars, std::map<int, int> &nonreg, std::set<int> &all);

void reportNonRegularMotifs(const rapidjson::Value &segmentations, const rapidjson::Value &motiftypes, const rapidjson::Value &padsizes);

void reportIndicesRange(const rapidjson::Value &motiftypes);
}
}

#endif

