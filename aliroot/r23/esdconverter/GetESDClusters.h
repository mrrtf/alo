#ifndef GETESDCLUSTERS_H
#define GETESDCLUSTERS_H

#include <vector>
#include <utility>

class AliESDEvent;
class AliESDMuonCluster;

std::pair<std::vector<AliESDMuonCluster*>,int> getClusters(AliESDEvent& event, int detElemId, bool goodTracksOnly);

#endif
