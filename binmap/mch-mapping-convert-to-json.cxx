#include "AliMpDDLStore.h"
#include "AliMpDEStore.h"
#include "AliMpDataMap.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "bp2json.h"
#include "ch2json.h"
#include "ddl2json.h"
#include "de2json.h"
#include "json.h"
#include "pcb2json.h"
#include "motif2json.h"
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

    std::ofstream pcbout("pcb.json");
    curlybrace(pcbout,all_pcb2json);

    std::ofstream motifout("motif.json");
    curlybrace(motifout,all_motif2json);

    return 0;
}

