#ifndef CONVERTESD_H
#define CONVERTESD_H

#include <map>
#include <memory>
#include <fstream>
#include "flatbuffers/flatbuffers.h"
#include "SegmentationPair.h"

class AliESDMuonCluster;
class AliESDEvent;

using SegmentationMap = std::map<int, std::unique_ptr<SegmentationPair>>;
using OutputFileMap = std::map<int, std::ofstream>;

void convertESD(const SegmentationMap& segmentations,
                const char* esdFileName,
                OutputFileMap& clusterFiles,
                OutputFileMap& digitFiles);

SegmentationMap getSegmentations(const std::vector<int>& detElemIds);

void convertDigits(const SegmentationPair& seg,
                AliESDEvent& event,
                   std::vector<AliESDMuonCluster*>& clusters, flatbuffers::FlatBufferBuilder& fbb);

void convertClusters(AliESDEvent& event,
                     std::vector<AliESDMuonCluster*>& clusters, flatbuffers::FlatBufferBuilder& fbb);

#endif

