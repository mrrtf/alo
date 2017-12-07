//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche


#ifndef O2_MCH_MAPPING_MOTIFPOSITIONCONTOURS_H
#define O2_MCH_MAPPING_MOTIFPOSITIONCONTOURS_H

#include "pad.h"
#include "genMotifType.h"
#include "polygon.h"
#include <vector>
#include <array>
#include <contourCreator.h>
#include <set>
#include "zone.h"
#include "segmentationInterface.h"

namespace o2 {
namespace mch {
namespace mapping {

template<typename MOTIFPOSITION>
std::vector<Pad> getPads(const MOTIFPOSITION &mp, const MotifType &motifType)
{
  std::vector<Pad> motifPads{mp.getPads(motifType)};
  std::vector<Pad> pads;
  for (auto p: motifPads) {
    pads.push_back(p.translate(mp.positionX(), mp.positionY()));
  }
  return pads;
}

std::vector<o2::mch::contour::Polygon<double>> padAsPolygons(const std::vector<Pad> &pads);

template<typename MOTIFPOSITION, int N>
std::array<o2::mch::contour::Contour<double>, N>
createMotifPositionContours(const std::array<MOTIFPOSITION, N> &motifPositions, const MotifTypeArray &motifTypes)
{
  std::array<o2::mch::contour::Contour<double>, N> contours;
  for (auto i = 0; i < motifPositions.size(); ++i) {
    auto pads = padAsPolygons(getPads(motifPositions[i], motifTypes[motifPositions[i].motifTypeId()]));
    contours[i] = o2::mch::contour::createContour(pads);
  }
  return contours;
}

template<typename MOTIFPOSITION, int N>
std::vector<Zone<MOTIFPOSITION>>
createZones(const std::array<MOTIFPOSITION, N> &motifPositions, const MotifTypeArray &motifTypes)
{
  // get the number of pad sizes
  std::set<int> padsizes;
  for (auto i = 0; i < motifPositions.size(); ++i) {
    auto& mp = motifPositions[i];
    padsizes.insert(mp.padSizeId());
  }

  // create one zone per pad size and add the motifpositions
  // to their respective zone
  std::vector<Zone<MOTIFPOSITION>> zones(padsizes.size());
  for (auto i = 0; i < motifPositions.size(); ++i) {
    auto& mp = motifPositions[i];
    auto it = std::find(begin(padsizes), end(padsizes),mp.padSizeId());
    int index =  std::distance(padsizes.begin(), it);
    zones[index].motifPositions.push_back(mp);
  }

  // for each zone create the motifposition contours and
  // the zone contour

  for (auto i = 0; i < zones.size(); ++i) {
    auto& zone = zones[i];
    for ( auto j = 0; j < zone.motifPositions.size(); ++j  ) {
      auto &mp = zone.motifPositions[j];
      auto& mt = motifTypes[mp.motifTypeId()];
      zone.contours.push_back(o2::mch::contour::createContour(padAsPolygons(getPads(mp,mt))));
    }
    zone.envelop = o2::mch::contour::getEnvelop(zone.contours);
  }

  return zones;
}

//template<typename MOTIFPOSITION>
//struct Zone
//{
//    o2::mch::contour::Contour<double> envelop;
//    std::vector<o2::mch::contour::Contour<double>> contours;
//    std::vector<MOTIFPOSITION> motifPositions;
//};
}
}
}

#endif
