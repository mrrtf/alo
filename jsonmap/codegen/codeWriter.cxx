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
#include "codeWriter.h"
#include <fstream>
#include <sstream>

std::string mappingNamespaceBegin() {
  return "\nnamespace o2\n{\nnamespace mch\n{\nnamespace mapping\n{\n";
}

std::string mappingNamespaceEnd() {
  return "\n} // namespace mapping\n} // namespace mch\n} // namespace o2\n";
}

std::string includeGuardName(const std::string& filename) {
  std::string rv = "ALO_JSONMAP_READER_" + filename;
  std::transform(rv.begin(), rv.end(), rv.begin(), [](unsigned char c) { return std::toupper(c); });
  rv += "_H";
  return rv;
}

std::string includeGuardBegin(const std::string& filename) {
  std::ostringstream s;
  s <<  "#ifndef " << includeGuardName(filename) << "\n";
  s <<  "#define " << includeGuardName(filename) << "\n\n";
  return s.str();
}

std::string includeGuardEnd(const std::string& filename) {
  return "\n#endif // " + includeGuardName(filename);
}

void outputCode(const std::string decl, const std::string &impl, const std::string &outputFileName) {

  std::string includeFileName = outputFileName + ".h";

  std::ofstream declFile(includeFileName);
  declFile << includeGuardBegin(outputFileName);
  declFile << decl;
  declFile << includeGuardEnd(outputFileName);

  std::ofstream implFile(outputFileName + ".cxx");
  implFile << "#include \"" << includeFileName << "\"\n";
  implFile << impl;
}

