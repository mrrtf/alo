//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "writer.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace jsonmap {
namespace codegen {

std::string copyright()
{
  return R"(// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.)";
}

std::string mappingNamespaceBegin(std::string ns)
{
  return "\nnamespace o2 {\nnamespace mch {\nnamespace mapping {\nnamespace " + ns + " {\n";
}

std::string mappingNamespaceEnd(std::string ns)
{
  return "\n} // namespace " + ns + "\n} // namespace mapping\n} // namespace mch\n} // namespace o2\n";
}

std::string includeGuardName(const std::string &filename)
{
  std::string rv = "O2_MCH_MAPPING_" + filename;
  std::transform(rv.begin(), rv.end(), rv.begin(), [](unsigned char c) { return std::toupper(c); });
  rv += "_H";
  return rv;
}

std::string includeGuardBegin(const std::string &filename)
{
  std::ostringstream s;
  s << "#ifndef " << includeGuardName(filename) << "\n";
  s << "#define " << includeGuardName(filename) << "\n\n";
  return s.str();
}

std::string includeGuardEnd(const std::string &filename)
{
  return "\n#endif // " + includeGuardName(filename);
}

std::string generateNotice(bool standalone)
{
  std::string notice{
    R"(//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//)"
  };

  if (!standalone) {
    notice += R"(
// This implementation file cannot be used standalone, i.e. it is intended to be included
// into another implementation file.
//)";
  }
  notice += "\n";
  return notice;
}

void outputCode(const std::string decl, const std::string impl, std::string outputFileName,
                bool withIncludeGuards, bool standalone,
                const std::string includeFileNameIfDifferentFromImplementation)
{
  const std::string genPrefix="Gen";

  outputFileName = genPrefix + outputFileName;
  std::string includeFileName = outputFileName + ".h";

  if (!decl.empty()) {
    std::ofstream declFile(includeFileName);
    declFile << copyright() << "\n";
    if (withIncludeGuards) {
      declFile << includeGuardBegin(outputFileName);
    }
    declFile << generateNotice(standalone);
    declFile << decl;
    if (withIncludeGuards) {
      declFile << includeGuardEnd(outputFileName);
    }
  }

  if (impl.empty()) {
    return;
  }
  std::ofstream implFile(outputFileName + ".cxx");
  implFile << copyright() << "\n";
  if (!decl.empty()) {
    implFile << "#include \"" << ((includeFileNameIfDifferentFromImplementation.empty()) ? includeFileName
                                                                                         : includeFileNameIfDifferentFromImplementation)
             << "\"\n";
  }
  implFile << generateNotice(standalone);
  implFile << impl;
}

std::string generateInclude(std::initializer_list<std::string> list)
{
  std::ostringstream code;

  for (auto h: list) {
    if (h.find('.') != std::string::npos) {
      code << "#include \"" << h << "\"\n";
    } else {
      code << "#include <" << h << ">\n";
    }
  }

  return code.str();
}

}
}
