#include "Mapping.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataMap.h"
#include "AliMpDataStreams.h"
#include "AliMpDetElement.h"
#include "AliMpDDLStore.h"
#include "AliMpSegmentation.h"
#include "AliMpPlaneType.h"

struct Mapping {
  Mapping()
  {
    AliMpDataProcessor mp;
    AliMpDataMap* dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);
    ddlStore = AliMpDDLStore::ReadData(dataStreams);
    mseg = AliMpSegmentation::Instance();
  }
  AliMpDDLStore* ddlStore = nullptr;
  AliMpSegmentation* mseg = nullptr;
};

Mapping* mapping()
{
  static Mapping m;
  return &m;
}

const AliMp::CathodType getCathode(int detElemId, bool isBending)
{
  AliMpDetElement* de = mapping()->ddlStore->GetDetElement(detElemId);
  AliMp::CathodType cath = AliMp::kCath0;
  AliMp::PlaneType pt = de->GetPlaneType(cath);
  if ((pt == AliMp::kNonBendingPlane && isBending) ||
      (pt == AliMp::kBendingPlane && !isBending)) {
    cath = AliMp::kCath1;
  }
  return cath;
}

const AliMpVSegmentation* getSegmentation(int detElemId, bool isBending)
{
  return getSegmentation(detElemId, getCathode(detElemId, isBending));
}

const AliMpVSegmentation* getSegmentation(int detElemId, AliMp::CathodType cathod)
{
  return mapping()->mseg->GetMpSegmentation(detElemId, cathod);
}
