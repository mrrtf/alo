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
#include "legacyseg2json.h"
#include "motif.h"
#include "motifPosition.h"
#include "motifType.h"
#include "motiftype2json.h"
#include "padsize.h"
#include "padsize2json.h"
#include "pcb.h"
#include "pcb2json.h"
#include "readmapping.h"
#include "seg.h"
#include "seg2json.h"
#include <cstdlib>
#include "segnumbers.h"
#include <utility>

void convert_berg(std::string key)
{
  OF os(key + ".json");
  decltype(os.Writer()) w(os.Writer());
  w.StartObject();
  w.Key(key.c_str());
  w.StartArray();
  berg2json(80, AliMpFiles::BergToGCFilePath(AliMp::kStation12, AliMq::kStation1), w);
  berg2json(100, AliMpFiles::BergToGCFilePath(AliMp::kStation345, AliMq::kStation1), w);
  w.EndArray();
  w.EndObject();
}

void convert_bp(std::string key, AliMpDDLStore* ddlStore)
{
  std::vector<int> deids = get_deids(ddlStore);
  std::vector<AliMpBusPatch*> bps = get_bps(ddlStore, deids);
  all_bp2json(key, bps, OF(key + ".json").Writer());
}

void convert_ddl(std::string key, AliMpDDLStore* ddlStore)
{
  std::vector<AliMpDDL*> ddls;
  std::vector<std::vector<int>> desperddl;
  ddl_read_mapping(ddlStore, ddls, desperddl);
  all_ddl2json(key, ddls, desperddl, OF(key + ".json").Writer());
}

void convert_ch(std::string key, AliMpDDLStore* ddlStore)
{
  std::vector<std::vector<int>> desperch = get_desperch(ddlStore);
  all_ch2json(key, desperch, OF(key + ".json").Writer());
}

void convert_de(std::string key, AliMpDDLStore* ddlStore)
{
  std::vector<AliMpDetElement*> des = get_des(ddlStore);
  std::vector<int> deids = get_deids(ddlStore);
  std::vector<std::vector<int>> bpids = get_bpids_per_de(ddlStore, deids);
  all_de2json(key, des, bpids, OF(key + ".json").Writer());
}

void convert_motiftype(std::string key, AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  /// convert all motif types to JSON
  std::vector<AliMpPCB*> pcbs = get_allpcbs(ddlStore, mseg);
  std::vector<const AliMpSector*> sectors = get_allsectors(mseg);
  std::vector<AliMpMotifType*> motifTypes = get_allmotiftypes(pcbs, sectors);
  all_motiftype2json(key, motifTypes, OF(key + ".json").Writer());
}

void convert_pcb(std::string key, AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<AliMpPCB*> pcbs = get_allpcbs(ddlStore, mseg);
  auto padsizes = get_padsizes(ddlStore, mseg);
  all_pcb2json(key, pcbs, padsizes, OF(key + ".json").Writer());
}

void convert_legacyseg(std::string key, AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<int> deids = get_deids(ddlStore);
  std::vector<AliMpVSegmentation*> b = get_segs(mseg, deids, AliMp::kBendingPlane);
  std::vector<AliMpVSegmentation*> nb = get_segs(mseg, deids, AliMp::kNonBendingPlane);
  all_legacyseg2json(key,b, nb, OF(key+".json").Writer());
}

void convert_seg(std::string key, AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<std::string> segnames = get_all_segmentation_names(ddlStore, mseg);
  std::vector<std::pair<std::vector<AliMpMotifPosition*>, std::vector<AliMpMotifPosition*>>> motifpositions = get_motifpositions(
    ddlStore, mseg, segnames);
  std::vector<AliMpPCB*> pcbs = get_allpcbs(ddlStore, mseg);
  std::vector<const AliMpSector*> sectors = get_allsectors(mseg);
  auto padsizes = get_padsizes(ddlStore, mseg);
  std::vector<AliMpVMotif*> motifs = get_allmotifs(pcbs, sectors, padsizes);
  std::vector<AliMpMotifType*> motiftypes = get_allmotiftypes(pcbs, sectors);
  all_seg2json(key,segnames, motifpositions, motifs, motiftypes, padsizes, OF(key + ".json").Writer());
}

void convert_padsize(std::string key, AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  all_padsizes(key, get_padsizes(ddlStore, mseg), OF(key + ".json").Writer());
}

int main(int argc, char* argv[])
{
  AliMpDDLStore* ddlStore;
  AliMpSegmentation* mseg;
  readMapping(ddlStore, mseg);

  assert(mseg != nullptr);

  convert_berg("bergs");
  convert_bp("buspatches",ddlStore);
  convert_ch("chambers",ddlStore);
  convert_ddl("ddls",ddlStore);
  convert_de("detection_elements",ddlStore);
  convert_motiftype("motiftypes",ddlStore, mseg);
  convert_pcb("pcbs",ddlStore, mseg);
  convert_legacyseg("legacyseg",ddlStore, mseg);
  convert_seg("segmentations",ddlStore, mseg);
  convert_padsize("padsizes",ddlStore, mseg);

  return 0;
}
