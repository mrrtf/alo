#include "AliMpDataMap.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpDEStore.h"
#include "AliMpDDLStore.h"
#include "bp2json.h"
#include "ch2json.h"
#include "ddl2json.h"
#include "de2json.h"
#include "json.h"
#include "seg2json.h"
#include <iostream>

int main() {

    AliMpDataProcessor mp;
    AliMpDataMap* dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);

    AliMpDEStore::ReadData(dataStreams,kFALSE);
    AliMpDDLStore::ReadData(dataStreams);

    std::ofstream ddlout("ddl.json"); 
    curlybrace(ddlout,all_ddl2json);
    
    std::ofstream chout("ch.json");
    curlybrace(chout,all_ch2json);

    std::ofstream deout("de.json");
    curlybrace(deout,all_de2json);

    std::ofstream bpout("bp.json");
    curlybrace(bpout,all_bp2json);

    std::ofstream segout("seg.json");
    curlybrace(segout,all_seg2json);

    return 0;
}

