#include "AliMpDDLStore.h"
#include "AliMpDEIterator.h"
#include <vector>
#include <AliMpCathodType.h>
#include "de.h"
#include "bp.h"
#include "seg.h"
#include "AliMpSegmentation.h"
#include <algorithm>

std::vector<int> get_deids(AliMpDDLStore* /*ddlStore*/)
{
  AliMpDEIterator it;
  it.First();

  std::vector<int> deids;

  while (!it.IsDone()) {
    if (it.CurrentDEId() < 1100) {
      deids.push_back(it.CurrentDEId());
    }
    it.Next();
  }

  std::sort(deids.begin(), deids.end());
  return deids;
}

std::vector<int> get_deids(AliMpDDLStore* /*ddlStore*/, int chamber)
{
  AliMpDEIterator it;
  it.First(chamber);

  std::vector<int> deids;

  while (!it.IsDone()) {
    if (it.CurrentDEId() < 1100) {
      deids.push_back(it.CurrentDEId());
    }
    it.Next();
  }

  std::sort(deids.begin(), deids.end());
  return deids;
}

std::vector<AliMpDetElement*> get_des(AliMpDDLStore* ddlStore)
{
  std::vector<AliMpDetElement*> des;
  std::vector<int> deids = get_deids(ddlStore);
  for (auto& d:deids) {
    des.push_back(ddlStore->GetDetElement(d));
  }
  return des;
}

std::vector<std::vector<AliMpBusPatch*>> get_bps_per_de(AliMpDDLStore* ddlStore,
                                                        const std::vector<int>& deids)
{
  std::vector<std::vector<AliMpBusPatch*>> bps;
  for (auto& d : deids) {
    bps.push_back(get_bps(ddlStore, std::vector<int>{d}));
  }
  return bps;
}

std::vector<std::vector<int>> get_bpids_per_de(AliMpDDLStore* ddlStore,
                                               const std::vector<int>& deids)
{
  std::vector<std::vector<int>> bpids;
  for (auto& d : deids) {
    bpids.push_back(get_bpids(ddlStore, std::vector<int>{d}));
  }
  return bpids;
}

std::vector<std::string> get_de_seg(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<std::string> segperde;

  std::vector<int> deids = get_deids(ddlStore);

  for (auto d: deids) {
    const AliMpVSegmentation* seg = mseg->GetMpSegmentation(d, AliMp::kCath0);
    segperde.push_back(get_segtype(*seg));
  }
  return segperde;
}

std::vector<int> get_deids_per_segname(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, std::string segname)
{
  std::vector<std::string> segnames = get_de_seg(ddlStore, mseg);
  std::vector<int> deids = get_deids(ddlStore);
  std::vector<int> idperseg;
  for (auto i = 0; i < segnames.size(); i++) {
    if (segnames[i] == segname) {
      idperseg.push_back(deids[i]);
    }
  }
  return idperseg;
}
