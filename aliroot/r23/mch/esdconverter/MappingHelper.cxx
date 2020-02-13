#include "MappingHelper.h"
#include "AliMpDDLStore.h"
#include "AliMpDataMap.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"

AliMpDDLStore *getDDLStore() {
  static AliMpDDLStore *ddlStore{nullptr};
  if (!ddlStore) {
    AliMpDataProcessor mp;
    AliMpDataMap *dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);
    ddlStore = AliMpDDLStore::ReadData(dataStreams);
  }
  return ddlStore;
}

AliMpSegmentation *mseg() {
  static AliMpSegmentation *mseg{nullptr};
  if (!mseg) {
    getDDLStore();
    mseg = AliMpSegmentation::Instance();
  }
  return mseg;
}

