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
#include "motif2json.h"
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
#include <utility>

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
  /// convert all motif types to JSON
  std::vector<AliMpPCB*> pcbs = get_allpcbs(ddlStore,mseg);
  std::vector<const AliMpSector*> sectors = get_allsectors(mseg);

  std::vector<AliMpMotifType*> motifTypes = get_allmotiftypes(pcbs,sectors);
  all_motiftype2json(motifTypes,OF("motiftype.json").Writer());
}

void convert_motif(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg) {
  // convert all motifs (=motif type + pad size(s)) to JSON
  std::vector<AliMpPCB*> pcbs = get_allpcbs(ddlStore,mseg);
  std::vector<const AliMpSector*> sectors = get_allsectors(mseg);
  std::vector<std::pair<float,float>> padsizes = get_padsizes(ddlStore,mseg);
  std::vector<AliMpVMotif*> motifs = get_allmotifs(pcbs, sectors, padsizes);
  std::vector<AliMpMotifType*> motifTypes = get_allmotiftypes(pcbs,sectors);
  all_motif2json(motifs, motifTypes, padsizes, OF("motif.json").Writer());
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
  convert_motif(ddlStore, mseg);
  convert_pcb(ddlStore,mseg);
  convert_seg(ddlStore,mseg);
  convert_padsize(ddlStore,mseg);

  return 0;
}
