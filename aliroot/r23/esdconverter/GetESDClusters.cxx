#include <vector>
#include "AliESDEvent.h"
#include "AliESDMuonTrack.h"
#include "AliESDMuonCluster.h"

bool isGoodTrack(const AliESDMuonTrack& track) {

  auto eta = track.Eta();
  bool goodEta = ( eta > -4.0 && eta < -2.5 );

  auto thetaAbsDeg = TMath::ATan(track.GetRAtAbsorberEnd()/505.0)*TMath::RadToDeg();
  bool goodRabs = ( thetaAbsDeg > 2.0 && thetaAbsDeg < 10.0 );

  bool goodTrigger = track.GetMatchTrigger() > 1;

  return goodEta && goodRabs && goodTrigger;
}

std::vector<AliESDMuonTrack*> getTracks(AliESDEvent& event, bool goodTracksOnly)
{
  std::vector<AliESDMuonTrack*> tracks;

  auto nTracks = event.GetNumberOfMuonTracks();

  for (auto iTrack = 0; iTrack < nTracks; iTrack++) {
    AliESDMuonTrack* track = event.GetMuonTrack(iTrack);
    if (track->ContainTrackerData()) {
      bool keep = true;
      if ( goodTracksOnly ) {
              keep = isGoodTrack(*track);
      }
      if (keep) {
              tracks.push_back(track);
      }
    }
  }
  return tracks;
}

std::pair<std::vector<AliESDMuonCluster*>,int> getClusters(AliESDEvent& event, int detElemId, bool goodTracksOnly)
{
  std::vector<AliESDMuonCluster*> clusters;

  auto tracks = getTracks(event,goodTracksOnly);

  for (auto track : tracks) {

    auto nClusters = track->GetNClusters();

    for (auto iCl = 0; iCl < nClusters; iCl++) {
      auto cluster = event.FindMuonCluster(track->GetClusterId(iCl));
      if (cluster->GetDetElemId() == detElemId) {
        clusters.push_back(cluster);
      }
    }
  }
  return { clusters, tracks.size() };
}

