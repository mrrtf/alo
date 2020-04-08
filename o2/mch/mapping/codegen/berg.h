#ifndef ALO_O2_MCH_MAPPING_CODEGEN_BERG_H
#define ALO_O2_MCH_MAPPING_CODEGEN_BERG_H

#include <array>
#include "rapidjson/document.h"

std::array<int, 64> getManu2Ds(const char* bergRun2, const char* bergRun3, bool st345);

void dump(rapidjson::Value& berg, bool is80pins);

#endif
