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


#ifndef ALO_PADSIZE2JSON_H
#define ALO_PADSIZE2JSON_H

#include "json.h"
#include <vector>
#include <utility>

template<typename WRITER>
void all_padsizes(std::string topkey, const std::vector<std::pair<float, float>>& padsizes, WRITER& w)
{
  w.StartObject();
  w.Key(topkey.c_str());
  w.StartArray();
  int n = 0;
  float unit = 1E4; // original sizes in AliRoot are in cm
  for (const auto& p: padsizes) {
    w.StartObject();
    w.Key("id");
    w.Int(n++);
    w.Key("unit");
    w.String("micron");
    w.Key("x");
    w.Int(std::round(p.first * unit));
    w.Key("y");
    w.Int(std::round(p.second * unit));
    w.EndObject();
  }
  w.EndArray();
  w.EndObject();
}

#endif
