#include "AliMpBusPatch.h"
#include "AliMpDataMap.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpFiles.h"
#include "AliMpSegmentation.h"
#include "berg2json.h"
#include "bp.h"
#include "bp2json.h"
#include "ch.h"
#include "ch2json.h"
#include "ddl.h"
#include "ddl2json.h"
#include "de.h"
#include "de2json.h"
#include "json.h"
#include "motif2json.h"
#include "pcb2json.h"
#include "seg.h"
#include "seg2json.h"
#include <cstdlib>
#include <fstream>

void convert_berg() {

    OF os("berg.json");
    decltype(os.Writer()) w(os.Writer());

    w.StartObject();
    berg2json(80,AliMpFiles::BergToGCFilePath(AliMp::kStation12,AliMq::kStation1),w);
    berg2json(100,AliMpFiles::BergToGCFilePath(AliMp::kStation345,AliMq::kStation1),w);
    w.EndObject();
}

void convert_bp(AliMpDDLStore* ddlStore)
{
    std::vector<int> deids = get_deids(ddlStore);
    std::vector<AliMpBusPatch*> bps = get_bps(ddlStore,deids);

    all_bp2json(bps,OF("bp.json").Writer());
}

void convert_ddl(AliMpDDLStore* ddlStore)
{
    std::vector<AliMpDDL*> ddls;
    std::vector<std::vector<int>> desperddl;

    ddl_read_mapping(ddlStore,ddls,desperddl);

    all_ddl2json(ddls,desperddl,OF("ddl.json").Writer());
}

void convert_ch(AliMpDDLStore* ddlStore)
{
    std::vector<std::vector<int>> desperch = ch_read_mapping(ddlStore);
    all_ch2json(desperch,OF("ch.json").Writer());
}

void convert_de(AliMpDDLStore* ddlStore)
{
    std::vector<AliMpDetElement*> des = get_des(ddlStore);
    std::vector<int> deids = get_deids(ddlStore);
    std::vector<std::vector<int>> bpids = get_bpids_per_de(ddlStore,deids);
    all_de2json(des,bpids,OF("de.json").Writer());
}

// void convert_motif(AliMpDDLStore* ddlStore) 
// {
//
// }

void convert_seg(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
   std::vector<int> deids = get_deids(ddlStore);
   std::vector<AliMpVSegmentation*> b;
   std::vector<AliMpVSegmentation*> nb;
   get_segs(mseg,deids,b,nb);
   all_seg2json(b,nb,OF("seg.json").Writer());
}

int main(int argc, char* argv[]) 
{
    AliMpDataProcessor mp;
    AliMpDataMap* dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);

    AliMpDDLStore* ddlStore = AliMpDDLStore::ReadData(dataStreams);
    AliMpSegmentation* mseg = AliMpSegmentation::Instance();
    assert(mseg!=nullptr);
    
    // convert_berg();
    // convert_bp(ddlStore);
    // convert_ch(ddlStore);
    // convert_ddl(ddlStore);;
    // convert_de(ddlStore);
    
    // convert_motif(ddlStore);
    // convert_pcb(ddlStore);
    convert_seg(ddlStore,mseg);

    return 0;
}
