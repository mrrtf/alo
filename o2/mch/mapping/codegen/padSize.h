#ifndef ALO_JSONMAP_CODEGEN_PADSIZE_H
#define ALO_JSONMAP_CODEGEN_PADSIZE_H

#include "rapidjson/document.h"
#include <string>
#include <vector>
#include <utility>
#include <ostream>

namespace jsonmap {
namespace codegen {

struct PadSize {
    double x;
    double y;
};

std::ostream &operator<<(std::ostream &out, const PadSize& padsize);

std::vector<PadSize> getPadSizes(const rapidjson::Value &jsonPadSizes);

std::string generateCodeForPadSizes(std::string ns, const rapidjson::Value &jsonPadSizes);

}
}
#endif

