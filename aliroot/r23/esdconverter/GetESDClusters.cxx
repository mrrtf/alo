#include <vector>
#include "AliESDEvent.h"
#include "AliESDMuonTrack.h"
#include "AliESDMuonCluster.h"

std::vector<AliESDMuonTrack*> getTracks(AliESDEvent& event)
{
  std::vector<AliESDMuonTrack*> tracks;

  auto nTracks = event.GetNumberOfMuonTracks();

  for (auto iTrack = 0; iTrack < nTracks; iTrack++) {
    AliESDMuonTrack* track = event.GetMuonTrack(iTrack);
    if (track->ContainTrackerData()) {
      tracks.push_back(track);
    }
  }
  return tracks;
}

std::vector<AliESDMuonCluster*> getClusters(AliESDEvent& event, int detElemId)
{
  std::vector<AliESDMuonCluster*> clusters;

  auto tracks = getTracks(event);

  for (auto track : tracks) {

    auto nClusters = track->GetNClusters();

    for (auto iCl = 0; iCl < nClusters; iCl++) {
      auto cluster = event.FindMuonCluster(track->GetClusterId(iCl));
      if (cluster->GetDetElemId() == detElemId) {
        clusters.push_back(cluster);
      }
    }
  }
  return clusters;
}

