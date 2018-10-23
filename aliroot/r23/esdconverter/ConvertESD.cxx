#include "ConvertESD.h"

#include "AliESDEvent.h"
#include "AliESDMuonCluster.h"
#include "AliESDMuonPad.h"
#include "AliESDMuonTrack.h"
#include "Run2_generated.h"
#include "GetESDClusters.h"
#include "SegmentationPair.h"
#include "TFile.h"
#include "TTree.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

bool isBendingManu(int manuId) { return (manuId & 1024) == 0; }
int getDetElemId(unsigned int esdDigitUID) { return esdDigitUID & 0xFFF; }
int getManuId(unsigned int esdDigitUID) {
  return (esdDigitUID & 0xFFF000) >> 12;
}
int getManuChannel(unsigned int esdDigitUID) {
  return (esdDigitUID & 0x3F000000) >> 24;
}

SegmentationMap getSegmentations(const std::vector<int> &detElemIds) {
  SegmentationMap segmentations;
  for (auto detElemId : detElemIds) {
    segmentations.insert(std::make_pair(
        detElemId,
        std::unique_ptr<SegmentationPair>(new SegmentationPair(detElemId))));
  }
  return segmentations;
}

void writeBuffer(flatbuffers::FlatBufferBuilder &fbb, std::ofstream &out) {
  uint8_t *buf = fbb.GetBufferPointer();
  int size = fbb.GetSize(); // Returns the size of the buffer that
                            // `GetBufferPointer()` points to.
  if (size) {
    out.write(reinterpret_cast<const char *>(&size), sizeof(int));
    out.write(reinterpret_cast<const char *>(buf), sizeof(uint8_t) * size);
  }
}

void convertESD(const SegmentationMap &segmentations, const char *esdFileName,
                OutputFileMap &clusterFiles, OutputFileMap &digitFiles) {
  std::unique_ptr<TFile> esdFile{TFile::Open(esdFileName)};

  if (!esdFile || !esdFile->IsOpen()) {
    Error("convertESD", "opening ESD file %s failed", esdFileName);
    return;
  }

  TTree *tree = static_cast<TTree *>(esdFile->Get("esdTree"));
  if (!tree) {
    Error("convertESD", "no ESD tree found");
    return;
  }

  AliESDEvent event;
  event.ReadFromTree(tree);

  auto nevents = tree->GetEntries();

  std::cout << nevents << " entries to process\n";

  tree->SetBranchStatus("*", false);
  tree->SetBranchStatus("AliESDRun*", true);
  tree->SetBranchStatus("AliESDHeader*", true);
  tree->SetBranchStatus("Muon*", true);

  flatbuffers::FlatBufferBuilder fbb{1024};

  for (auto iEvent = 0; iEvent < nevents; iEvent++) {

    if (tree->GetEvent(iEvent) <= 0) {
      Error("convertESD", "no ESD object found for event %d", iEvent);
      return;
    }

    for (auto &segPair : segmentations) {
      auto &seg = *(segPair.second);
      auto detElemId = segPair.first;
      auto clusters = getClusters(event, detElemId);
      fbb.Clear();
      convertClusters(event, clusters, fbb);
      auto clusterFile = clusterFiles.find(detElemId);
      if (clusterFile != clusterFiles.end()) {
        assert(clusterFile->second.is_open());
        writeBuffer(fbb, clusterFile->second);
      }
      auto digitFile = digitFiles.find(detElemId);
      if (digitFile != digitFiles.end()) {
        fbb.Clear();
        convertDigits(seg, event, clusters, fbb);
        writeBuffer(fbb, digitFile->second);
      }
    }
  }
}

std::vector<unsigned long>
getDigitUIDs(std::vector<AliESDMuonCluster *> &clusters) {
  std::vector<unsigned long> esdDigitUIDs;

  for (auto c : clusters) {
    for (int i = 0; i < c->GetNPads(); ++i) {
      esdDigitUIDs.emplace_back(c->GetPadId(i));
    }
  }

  return esdDigitUIDs;
}

void convertClusters(AliESDEvent& event,
                     std::vector<AliESDMuonCluster *> &esdClusters,
                     flatbuffers::FlatBufferBuilder &fbb) {
  std::vector<flatbuffers::Offset<run2::Cluster>> clusters;

  int detElemId{0};

  for (auto esdCluster : esdClusters) {
    std::vector<flatbuffers::Offset<run2::Digit>> digits;
    for (int i = 0; i < esdCluster->GetNPads(); ++i) {
      auto esdDigitID = esdCluster->GetPadId(i);
      int de = getDetElemId(esdDigitID);
      if (detElemId == 0) {
        detElemId = de;
      }
      assert(de == detElemId);
      auto pad = event.FindMuonPad(esdDigitID);
      auto manuId = pad->GetManuId();
      auto manuChannel = pad->GetManuChannel();
      auto adc = pad->GetADC();
      auto d = run2::CreateDigit(fbb,adc,detElemId,manuId,manuChannel);
      digits.push_back(d);
    }

    auto pre = run2::CreatePreClusterDirect(fbb, &digits);
    auto pos = run2::CreateClusterPos(fbb, esdCluster->GetX(), esdCluster->GetY(),
                                esdCluster->GetZ());
    clusters.push_back(run2::CreateCluster(fbb, pre, pos));
  }

  if (clusters.empty()) {
    return;
  }

  uint16_t bc = 0;
  uint32_t period = 0;
  float centrality = 0;
  bool isMB = true;

  auto eventBuffer = run2::CreateEventDirect(fbb,
                  bc, period, centrality, isMB, nullptr, &clusters);

  fbb.Finish(eventBuffer);
}

void convertDigits(const SegmentationPair &seg, AliESDEvent &event,
                   std::vector<AliESDMuonCluster *> &clusters,
                    flatbuffers::FlatBufferBuilder &fbb) {
        }
//   std::vector<flatbuffers::Offset<run2::Digit>> bendingDigits;
//   std::vector<flatbuffers::Offset<run2::Digit>> nonBendingDigits;
//
//   auto esdDigitUIDs = getDigitUIDs(clusters);
//
//   int detElemId{0};
//
//   for (auto esdDigitUID : esdDigitUIDs) {
//     detElemId = getDetElemId(esdDigitUID);
//     int manuId = getManuId(esdDigitUID);
//     int manuChannel = getManuChannel(esdDigitUID);
//     bool isBending = isBendingManu(manuId);
//     auto pad = seg[isBending].PadByLocation(manuId, manuChannel);
//     if (!pad.IsValid()) {
//       std::cout << "got invalid pad !\n";
//       std::cout << "[DE" << detElemId << " MANU" << manuId << " CH "
//                 << manuChannel << "]\n";
//     } else {
//       auto pad = event.FindMuonPad(esdDigitUID);
//       auto adc = pad->GetADC();
//       int paduid{0}; // FIXME: compute paduid?
//       if (isBending) {
//         bendingDigits.push_back(run2::CreateDigit(fbb, paduid, adc));
//       } else {
//         nonBendingDigits.push_back(run2::CreateDigit(fbb, paduid, adc));
//       }
//     }
//   }
//
//   if (bendingDigits.empty() && nonBendingDigits.empty()) {
//     return;
//   }
//
//   flatbuffers::Offset<DigitPlane> b =
//       run2::CreateDigitPlaneDirect(fbb, true, &bendingDigits);
//   flatbuffers::Offset<DigitPlane> nb =
//       run2::CreateDigitPlaneDirect(fbb, false, &nonBendingDigits);
//
//   auto vb = fbb.CreateVector(&b, 1);
//   auto vnb = fbb.CreateVector(&nb, 1);
//
//   int timestamp = 0;
//   auto tb_b = run2::CreateDigitTimeBlock(fbb, timestamp, vb);
//   auto tb_nb = run2::CreateDigitTimeBlock(fbb, timestamp, vnb);
//
//   std::vector<flatbuffers::Offset<DigitTimeBlock>> vtb{tb_b, tb_nb};
//
//   auto digitDE = run2::CreateDigitDE(fbb, detElemId, fbb.CreateVector(vtb));
//
//   fbb.Finish(digitDE);
// }
