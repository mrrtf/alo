#include "AliMpBusPatch.h"
#include "AliMpFiles.h"
#include "AliMpMotifType.h"
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
#include "motif.h"
#include "motifType.h"
#include "motiftype2json.h"
#include "padsize.h"
#include "padsize2json.h"
#include "pcb.h"
#include "pcb2json.h"
#include "seg.h"
#include "seg2json.h"
#include "readmapping.h"
#include <cstdlib>

void convert_berg() {

  OF os("berg.json");
  decltype(os.Writer()) w(os.Writer());

  w.StartObject();
  w.Key("bergs");
  w.StartArray();
  berg2json(80,AliMpFiles::BergToGCFilePath(AliMp::kStation12,AliMq::kStation1),w);
  berg2json(100,AliMpFiles::BergToGCFilePath(AliMp::kStation345,AliMq::kStation1),w);
  w.EndArray();
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
  std::vector<std::vector<int>> desperch = get_desperch(ddlStore);
  all_ch2json(desperch,OF("ch.json").Writer());
}

void convert_de(AliMpDDLStore* ddlStore)
{
  std::vector<AliMpDetElement*> des = get_des(ddlStore);
  std::vector<int> deids = get_deids(ddlStore);
  std::vector<std::vector<int>> bpids = get_bpids_per_de(ddlStore,deids);
  all_de2json(des,bpids,OF("de.json").Writer());
}

void convert_motiftype(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<AliMpPCB*> pcbs = get_allpcbs(ddlStore,mseg);

  std::vector<AliMpMotifType*> motifTypes = get_allslatmotiftypes(pcbs);

  std::cout << motifTypes.size() << " motif types in slats" << std::endl;
  for ( auto& x : motifTypes ) {
    std::cout << x->GetID() << ",";
  }
  std::cout << std::endl;

  std::vector<const AliMpSector*> sectors = get_allsectors(mseg);

  std::cout << sectors.size() << " sectors" << std::endl;

  std::vector<AliMpMotifType*> mt = get_allsectormotiftypes(sectors);

  std::cout << mt.size() << " motif types in sectors" << std::endl;

  motifTypes.insert(motifTypes.end(),mt.begin(),mt.end());

  std::cout << motifTypes.size() << " motif types in total" << std::endl;

  std::sort(motifTypes.begin(),motifTypes.end(),
            [](AliMpMotifType* a, AliMpMotifType* b) {
              return strcmp(a->GetID().Data(),b->GetID().Data())<0; }
  );

  std::ofstream out("motiftypes.aliroot.txt");

  for ( const auto& mt : motifTypes ) {
    int n = 0;
    for ( int i = 1; i <= 100; ++i ) {
      if ( mt->FindConnectionByPadNum(i)) ++n;
    }
    out << mt->GetID().Data() << " " << n << " " << mt->GetNofPadsX() << " " << mt->GetNofPadsY() << std::endl;
  }

  out.close();

  all_motif2json(motifTypes,OF("motif.json").Writer());

  std::vector<AliMpVMotif*> motifs = get_allslatmotifs(pcbs);
  std::vector<AliMpVMotif*> sectorMotifs = get_allsectormotifs(sectors);
  std::cout << motifs.size() << " different motifs in slats" << std::endl;
  std::cout << sectorMotifs.size() << " different motifs in sectors" << std::endl;

  motifs.insert(motifs.end(),sectorMotifs.begin(),sectorMotifs.end());

  out.open("motifs.list.txt");
  for ( const auto& m: motifs ) {
    out << m->GetID() << std::endl;
  }
  std::cout << motifs.size() << " different motifs in total" << std::endl;
}

void convert_pcb(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<AliMpPCB*> pcbs = get_allpcbs(ddlStore,mseg);
  std::vector<std::pair<float, float>> padsizes = get_padsizes(ddlStore, mseg);
  all_pcb2json(pcbs,padsizes, OF("pcb.json").Writer());
}

void convert_seg(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<int> deids = get_deids(ddlStore);
  std::vector<AliMpVSegmentation*> b = get_segs(mseg,deids,AliMp::kBendingPlane);
  std::vector<AliMpVSegmentation*> nb = get_segs(mseg,deids,AliMp::kNonBendingPlane);
  all_seg2json(b,nb,OF("seg.json").Writer());
}

void convert_padsize(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<std::pair<float, float>> padsizes = get_padsizes(ddlStore, mseg);
  all_padsizes(padsizes, OF("padsize.json").Writer());
}

int main(int argc, char* argv[])
{
  AliMpDDLStore *ddlStore;
  AliMpSegmentation *mseg;
  readMapping(ddlStore, mseg);

  assert(mseg!=nullptr);

  convert_berg();
  convert_bp(ddlStore);
  convert_ch(ddlStore);
  convert_ddl(ddlStore);
  convert_de(ddlStore);
  convert_motiftype(ddlStore, mseg);
  convert_pcb(ddlStore,mseg);
  convert_seg(ddlStore,mseg);
  convert_padsize(ddlStore,mseg);

  return 0;
}
