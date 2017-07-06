#ifndef ALO_JSONMAP_CREATOR_MOTIFTYPE_H
#define ALO_JSONMAP_CREATOR_MOTIFTYPE_H

#include <vector>
class AliMpMotifType;
class AliMpPCB;
class AliMpSector;
class AliMpVMotif;

std::vector<AliMpMotifType*> get_allslatmotiftypes(const std::vector<AliMpPCB *>& pcbs);
std::vector<AliMpMotifType*> get_allsectormotiftypes(const std::vector<const AliMpSector*>& sectors);

#endif

