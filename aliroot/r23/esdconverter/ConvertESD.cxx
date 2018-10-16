#include "ConvertESD.h"

#include "AliESDEvent.h"
#include "AliESDMuonCluster.h"
#include "AliESDMuonPad.h"
#include "AliESDMuonTrack.h"
#include "Cluster_generated.h"
#include "Digit_generated.h"
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

using namespace o2::mch;
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
      convertClusters(seg, clusters, fbb);
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

void convertClusters(const SegmentationPair &seg,
                     std::vector<AliESDMuonCluster *> &esdClusters,
                     flatbuffers::FlatBufferBuilder &fbb) {
  std::vector<flatbuffers::Offset<o2::mch::Cluster>> clusters;

  int detElemId{0};

  for (auto esdCluster : esdClusters) {
    std::vector<int> bdigits;
    std::vector<int> nbdigits;
    for (int i = 0; i < esdCluster->GetNPads(); ++i) {
      auto esdDigitID = esdCluster->GetPadId(i);
      int de = getDetElemId(esdDigitID);
      if (detElemId == 0) {
        detElemId = de;
      }
      assert(de == detElemId);
      auto manuId = getManuId(esdDigitID);
      auto manuChannel = getManuChannel(esdDigitID);
      bool isBending = isBendingManu(manuId);
      auto pad = seg[isBending].PadByLocation(manuId, manuChannel);
      assert(pad.IsValid());
      int paduid{0}; // FIXME: compute a VDigitId here ?
      if (isBending) {
        bdigits.push_back(paduid);
      } else {
        nbdigits.push_back(paduid);
      }
    }

    auto pre = CreatePreClusterDirect(fbb, &bdigits, &nbdigits);
    auto pos = CreateClusterPos(fbb, esdCluster->GetX(), esdCluster->GetY(),
                                esdCluster->GetZ());
    clusters.push_back(CreateCluster(fbb, pre, pos));
  }

  if (clusters.empty()) {
    return;
  }

  int ts{0};
  auto tb = CreateClusterTimeBlockDirect(fbb, ts, &clusters);

  auto cde = CreateClusterDE(fbb, detElemId, fbb.CreateVector(&tb, 1));

  fbb.Finish(cde);
}

void convertDigits(const SegmentationPair &seg, AliESDEvent &event,
                   std::vector<AliESDMuonCluster *> &clusters,
                   flatbuffers::FlatBufferBuilder &fbb) {

  std::vector<flatbuffers::Offset<o2::mch::Digit>> bendingDigits;
  std::vector<flatbuffers::Offset<o2::mch::Digit>> nonBendingDigits;

  auto esdDigitUIDs = getDigitUIDs(clusters);

  int detElemId{0};

  for (auto esdDigitUID : esdDigitUIDs) {
    detElemId = getDetElemId(esdDigitUID);
    int manuId = getManuId(esdDigitUID);
    int manuChannel = getManuChannel(esdDigitUID);
    bool isBending = isBendingManu(manuId);
    auto pad = seg[isBending].PadByLocation(manuId, manuChannel);
    if (!pad.IsValid()) {
      std::cout << "got invalid pad !\n";
      std::cout << "[DE" << detElemId << " MANU" << manuId << " CH "
                << manuChannel << "]\n";
    } else {
      auto pad = event.FindMuonPad(esdDigitUID);
      auto adc = pad->GetADC();
      int paduid{0}; // FIXME: compute paduid?
      if (isBending) {
        bendingDigits.push_back(o2::mch::CreateDigit(fbb, paduid, adc));
      } else {
        nonBendingDigits.push_back(o2::mch::CreateDigit(fbb, paduid, adc));
      }
    }
  }

  if (bendingDigits.empty() && nonBendingDigits.empty()) {
    return;
  }

  flatbuffers::Offset<DigitPlane> b =
      o2::mch::CreateDigitPlaneDirect(fbb, true, &bendingDigits);
  flatbuffers::Offset<DigitPlane> nb =
      o2::mch::CreateDigitPlaneDirect(fbb, false, &nonBendingDigits);

  auto vb = fbb.CreateVector(&b, 1);
  auto vnb = fbb.CreateVector(&nb, 1);

  int timestamp = 0;
  auto tb_b = o2::mch::CreateDigitTimeBlock(fbb, timestamp, vb);
  auto tb_nb = o2::mch::CreateDigitTimeBlock(fbb, timestamp, vnb);

  std::vector<flatbuffers::Offset<DigitTimeBlock>> vtb{tb_b, tb_nb};

  auto digitDE = o2::mch::CreateDigitDE(fbb, detElemId, fbb.CreateVector(vtb));

  fbb.Finish(digitDE);
}
