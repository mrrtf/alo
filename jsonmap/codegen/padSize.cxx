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
#include "writer.h"

namespace jsonmap {
namespace codegen {

std::string generateCodeForPadSizes(std::string ns, const rapidjson::Value &padsizes)
{
  assert(padsizes.IsArray());

  std::ostringstream code;

  code << generateInclude({"padSize.h", "utility", "array"});

  code << mappingNamespaceBegin(ns);
  code << R"(namespace {
std::array<std::pair<float, float>, 18> arrayOfPadSizes{
)";
  int n{0};
  for (auto &ps: padsizes.GetArray()) {
    code << "/* " << n << " */ std::make_pair<float,float>(" << static_cast<float>(ps["x"].GetDouble()) << ","
         << static_cast<float>(ps["y"].GetDouble()) << ")";
    n++;
    if (n < padsizes.Size()) { code << ","; }
    code << "\n";
  }

  code << R"(
};
}
double padSizeX(int i)
{ return arrayOfPadSizes[i].first; }

double padSizeY(int i)
{ return arrayOfPadSizes[i].second; }
)";
  code << mappingNamespaceEnd(ns);

  return code.str();

}

}
}



