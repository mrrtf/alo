#ifndef ALO_JSONMAP_READER_PADSIZE_H
#define ALO_JSONMAP_READER_PADSIZE_H

#include "rapidjson/document.h"
#include <string>

std::string generateCodeForPadSizes(const rapidjson::Value& padsizes);

#endif

