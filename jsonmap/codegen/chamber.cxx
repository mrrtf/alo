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


#include <iostream>
#include "boost/program_options.hpp"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "chamber.h"
#include "jsonReader.h"

namespace jsonmap {
namespace codegen {

void readChambers(rapidjson::Value &chambers)
{
  for (auto &v: chambers.GetArray()) {
    std::cout << v["id"].GetInt() << std::endl;
  }
}
}
}
