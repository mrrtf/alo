#include "ConvertESD.h"

#include "AliESDEvent.h"
#include "AliESDMuonCluster.h"
#include "AliESDMuonPad.h"
#include "AliESDMuonTrack.h"
#include "AliMultiplicity.h"
#include "GetESDClusters.h"
#include "Run2_generated.h"
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

void writeBuffer(flatbuffers::FlatBufferBuilder &fbb, std::ofstream &out) {
  uint8_t *buf = fbb.GetBufferPointer();
  int size = fbb.GetSize(); // Returns the size of the buffer that
                            // `GetBufferPointer()` points to.
  if (size) {
    out.write(reinterpret_cast<const char *>(&size), sizeof(int));
    out.write(reinterpret_cast<const char *>(buf), sizeof(uint8_t) * size);
  }
}

void convertESD(const std::vector<int> &detElemIds, const char *esdFileName,
                OutputFileMap &outputFiles) {
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

  // tree->SetBranchStatus("*", false);
  // tree->SetBranchStatus("AliESDRun*", true);
  // tree->SetBranchStatus("AliESDHeader*", true);
  // tree->SetBranchStatus("Muon*", true);

  flatbuffers::FlatBufferBuilder fbb{1024};

  for (auto iEvent = 0; iEvent < nevents; iEvent++) {

    if (tree->GetEvent(iEvent) <= 0) {
      Error("convertESD", "no ESD object found for event %d", iEvent);
      return;
    }

    for (auto detElemId : detElemIds) {
      auto clusters = getClusters(event, detElemId);
      fbb.Clear();
      convertClusters(event, clusters, fbb);
      auto ofile = outputFiles.find(detElemId);
      if (ofile != outputFiles.end()) {
        assert(ofile->second.is_open());
        writeBuffer(fbb, ofile->second);
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

void convertClusters(AliESDEvent &event,
                     std::vector<AliESDMuonCluster *> &esdClusters,
                     flatbuffers::FlatBufferBuilder &fbb) {
  std::vector<flatbuffers::Offset<run2::Cluster>> clusters;

  for (auto esdCluster : esdClusters) {
    std::vector<flatbuffers::Offset<run2::Digit>> clusterDigits;
    for (int i = 0; i < esdCluster->GetNPads(); ++i) {
      auto esdDigitID = esdCluster->GetPadId(i);
      auto pad = event.FindMuonPad(esdDigitID);
      int detElemId = pad->GetDetElemId();
      auto manuId = pad->GetManuId();
      auto manuChannel = pad->GetManuChannel();
      auto adc = pad->GetADC();
      auto d = run2::CreateDigit(fbb, adc, detElemId, manuId, manuChannel);
      clusterDigits.push_back(d);
    }

    auto pre = run2::CreatePreClusterDirect(fbb, &clusterDigits);
    auto pos = run2::CreateClusterPos(fbb, esdCluster->GetX(),
                                      esdCluster->GetY(), esdCluster->GetZ());
    clusters.push_back(run2::CreateCluster(fbb, pre, pos));
  }

  uint16_t bc = event.GetBunchCrossNumber();
  uint32_t period = event.GetPeriodNumber();
  int ntracklets = event.GetMultiplicity()->GetNumberOfTracklets();
  bool isMB = event.GetFiredTriggerClasses().Contains("CINT7-B-NOPF-MUFAST");

  auto eventBuffer = run2::CreateEventDirect(fbb, bc, period, ntracklets, isMB, &clusters);

  fbb.Finish(eventBuffer);
}

