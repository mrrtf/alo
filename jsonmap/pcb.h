#ifndef PCB_H
#define PCB_H

#include <vector>

class AliMpPCB;
class AliMpSegmentation;
class AliMpDDLStore;

std::vector<AliMpPCB*> get_allpcbs(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg);

#endif

