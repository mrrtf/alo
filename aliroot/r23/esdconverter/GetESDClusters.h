#ifndef GETESDCLUSTERS_H
#define GETESDCLUSTERS_H

#include <vector>
class AliESDEvent;
class AliESDMuonCluster;

std::vector<AliESDMuonCluster*> getClusters(AliESDEvent& event, int detElemId);

#endif
