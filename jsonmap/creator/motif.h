#ifndef MOTIF_H
#define MOTIF_H

#include <vector>
class AliMpMotifType;
class AliMpPCB;

std::vector<AliMpMotifType*> get_allmotiftypes(const std::vector<AliMpPCB*>& pcbs);

#endif

