#ifndef MOTIF_H
#define MOTIF_H

#include <vector>
class AliMpMotifType;
class AliMpPCB;
class AliMpSector;

std::vector<AliMpMotifType*> get_allslatmotiftypes(const std::vector<AliMpPCB *>& pcbs);
std::vector<AliMpMotifType*> get_allsectormotiftypes(const std::vector<const AliMpSector*>& sectors);

#endif

