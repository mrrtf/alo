#ifndef ALO_ALIROOT_R23_LEGACY_MAPPING_H
#define ALO_ALIROOT_R23_LEGACY_MAPPING_H

#include "AliMpCathodType.h"
#include "AliMpVSegmentation.h"

const AliMp::CathodType getCathode(int detElemId, bool isBending);
const AliMpVSegmentation* getSegmentation(int detElemId, bool isBending);
const AliMpVSegmentation* getSegmentation(int detElemId, AliMp::CathodType cathod);

#endif
