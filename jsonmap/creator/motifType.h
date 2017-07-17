#ifndef ALO_JSONMAP_CREATOR_MOTIFTYPE_H
#define ALO_JSONMAP_CREATOR_MOTIFTYPE_H

#include <vector>
#include <string>
class AliMpMotifType;
class AliMpPCB;
class AliMpSector;
class AliMpVMotif;

std::vector<AliMpMotifType*> get_allslatmotiftypes(const std::vector<AliMpPCB *>& pcbs);
std::vector<AliMpMotifType*> get_allsectormotiftypes(const std::vector<const AliMpSector*>& sectors);
std::vector<AliMpMotifType*> get_allmotiftypes(const std::vector<AliMpPCB*>& pcbs, const std::vector<const AliMpSector*>& sectors);

int get_motiftype_index(std::string motifID, const std::vector<AliMpMotifType*>& motifTypes);

#endif

