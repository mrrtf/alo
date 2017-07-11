#ifndef ALO_JSONMAP_CREATOR_DE_H
#define ALO_JSONMAP_CREATOR_DE_H

#include <vector>
#include <string>

class AliMpDDLStore;
class AliMpDetElement;
class AliMpBusPatch;
class AliMpSegmentation;

std::vector<int> get_deids(AliMpDDLStore* ddlStore);

std::vector<int> get_deids(AliMpDDLStore* ddlStore, int chamber);

std::vector<AliMpDetElement*> get_des(AliMpDDLStore* ddlStore);

std::vector<std::vector<AliMpBusPatch*>> get_bps_per_de(AliMpDDLStore* ddlStore, 
        const std::vector<int>& deids);

std::vector<std::vector<int>> get_bpids_per_de(AliMpDDLStore* ddlStore, 
        const std::vector<int>& deids);

std::vector<std::string> get_de_seg(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg);

std::vector<int> get_deids_per_segname(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, std::string segname);

#endif

