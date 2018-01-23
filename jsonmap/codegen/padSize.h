#ifndef ALO_JSONMAP_CODEGEN_PADSIZE_H
#define ALO_JSONMAP_CODEGEN_PADSIZE_H

#include "rapidjson/document.h"
#include <string>

namespace jsonmap {
namespace codegen {

std::ostream &operator<<(std::ostream &out, const std::pair<float,float>& padsize);

std::vector<std::pair<float, float>> getPadSizes(const rapidjson::Value &jsonPadSizes);

std::string generateCodeForPadSizes(std::string ns, const rapidjson::Value &jsonPadSizes);

}
}
#endif

