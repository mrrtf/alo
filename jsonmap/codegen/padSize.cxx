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

#include "padSize.h"
#include <sstream>
#include "codeWriter.h"

std::pair<std::string, std::string> generateCodeForPadSizes(const rapidjson::Value& padsizes)
{
  assert(padsizes.IsArray());

  std::ostringstream decl;

  std::ostringstream returnType;


  decl << "#include <array>\n";
  decl << "#include <utility>\n";
  decl << mappingNamespaceBegin();
  decl << "using PadSizeArray = std::array<std::pair<float,float>," << padsizes.Size() << ">;\n";
  decl << "extern PadSizeArray arrayOfPadSizes;\n";
  decl << "inline double padSizeX(int i) { return arrayOfPadSizes[i].first; }\n";
  decl << "inline double padSizeY(int i) { return arrayOfPadSizes[i].second; }\n";
  decl << mappingNamespaceEnd();

  std::ostringstream impl;

  impl << mappingNamespaceBegin();
  impl << "PadSizeArray arrayOfPadSizes {\n";
  int n{0};
  for (auto& ps: padsizes.GetArray()) {
    impl << "/* " << n << " */ std::make_pair<float,float>(" << static_cast<float>(ps["x"].GetDouble()) << "," << static_cast<float>(ps["y"].GetDouble()) << ")";
    n++;
    if (n<padsizes.Size()) impl << ",";
    impl << "\n";
  }

  impl << "};\n";
  impl << mappingNamespaceEnd();

  return std::make_pair<std::string,std::string>(decl.str(),impl.str());

}
