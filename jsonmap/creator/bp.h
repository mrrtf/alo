#ifndef BP_H
#define BP_H

#include <vector>

class AliMpDDLStore;
class AliMpBusPatch;

std::vector<AliMpBusPatch*> get_bps(AliMpDDLStore* ddlStore, const std::vector<int> deids);

std::vector<int> get_bpids(AliMpDDLStore* ddlStore, const std::vector<int> deids);

#endif
