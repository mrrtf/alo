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

#ifndef ALO_JSONMAP_CODEGEN_CODEWRITER_H
#define ALO_JSONMAP_CODEGEN_CODEWRITER_H

#include <string>

namespace jsonmap {
namespace codegen {

std::string mappingNamespaceBegin(const std::string ns);

std::string mappingNamespaceEnd(const std::string ns);

void outputCode(const std::string decl,
                const std::string impl,
                std::string outputFileName,
                bool withIncludeGuards = true,
                bool standalone = false,
                const std::string includeFileNameIfDifferentFromImplementation = "");

std::string generateInclude(std::initializer_list<std::string> list);
}
}
#endif

