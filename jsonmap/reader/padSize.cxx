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
  returnType << "std::array<std::pair<int,int>," << padsizes.Size() << ">";

  decl << "#include <array>\n";
  decl << "#include <utility>\n";
  decl << mappingNamespaceBegin();
  decl << returnType.str() << " padSizes();\n";
  decl << mappingNamespaceEnd();

  std::ostringstream impl;

  impl << mappingNamespaceBegin();
  impl << returnType.str() << " padSizes() {\n";
  impl << " return " << returnType.str() << "{ ";
  int n{0};
  for (auto& ps: padsizes.GetArray()) {
    impl << "std::make_pair<int,int>(" << ps["x"].GetInt() << "," << ps["y"].GetInt() << ")";
    n++;
    if (n<padsizes.Size()) impl << ",";
  }
  impl << "};\n";
  impl << "}\n";
  impl << mappingNamespaceEnd();

  return std::make_pair<std::string,std::string>(decl.str(),impl.str());

}
