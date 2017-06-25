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

void readChambers(const std::string &inputfile) {
  InputWrapper docw(inputfile.c_str());
  rapidjson::Document& d = docw.document();
  if (d.HasMember("chs")) {
    std::cout << "chs is an array:" << d["chs"].IsArray() << std::endl;
    for (auto& v: d["chs"].GetArray()) {
      std::cout << v["id"].GetInt() << std::endl;
    }
  }
}
