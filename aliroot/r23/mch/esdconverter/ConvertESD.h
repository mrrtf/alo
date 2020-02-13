#ifndef CONVERTESD_H
#define CONVERTESD_H

#include "flatbuffers/flatbuffers.h"
#include <fstream>
#include <map>
#include <memory>

class AliESDMuonCluster;
class AliESDEvent;

using OutputFileMap = std::map<int, std::ofstream>;

void convertESD(const std::vector<int> &detElemIds, const char *esdFileName,
                OutputFileMap &files, bool goodTracksOnly, int& nclusters, int& ntracks);

void convertClusters(AliESDEvent &event,
                     std::vector<AliESDMuonCluster *> &clusters,
                     flatbuffers::FlatBufferBuilder &fbb);

#endif

