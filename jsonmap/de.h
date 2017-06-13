#ifndef DE_H
#define DE_H

#include <vector>

class AliMpDDLStore;
class AliMpDetElement;
class AliMpBusPatch;

std::vector<int> get_deids(AliMpDDLStore* ddlStore);

std::vector<int> get_deids(AliMpDDLStore* ddlStore, int chamber);

std::vector<AliMpDetElement*> get_des(AliMpDDLStore* ddlStore);

std::vector<std::vector<AliMpBusPatch*>> get_bps_per_de(AliMpDDLStore* ddlStore, 
        const std::vector<int>& deids);

std::vector<std::vector<int>> get_bpids_per_de(AliMpDDLStore* ddlStore, 
        const std::vector<int>& deids);
#endif

