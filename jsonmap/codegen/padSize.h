#ifndef ALO_JSONMAP_CODEGEN_PADSIZE_H
#define ALO_JSONMAP_CODEGEN_PADSIZE_H

#include "rapidjson/document.h"
#include <string>

namespace jsonmap {
namespace codegen {

std::string generateCodeForPadSizes(const rapidjson::Value &padsizes);
}
}
#endif

