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


#include "motifPosition.h"
#include "seg.h"
#include "TArrayI.h"
#include "AliMpVSegmentation.h"

std::vector<AliMpMotifPosition*> get_motifpositions(const AliMpVSegmentation& seg)
{
  std::vector<AliMpMotifPosition*> mp;

  TArrayI ecn;
  seg.GetAllElectronicCardIDs(ecn);
  for (int i = 0; i < ecn.GetSize(); ++i) {
    mp.push_back(seg.MotifPosition(ecn[i]));
  }
  return mp;
}

std::vector<AliMpMotifPosition*> get_motifpositions(const AliMpVSegmentation& b, const AliMpVSegmentation& nb)
{
  std::vector<AliMpMotifPosition*> m = get_motifpositions(b);
  std::vector<AliMpMotifPosition*> mpnb = get_motifpositions(nb);

  m.insert(m.end(), mpnb.begin(), mpnb.end());
  return m;
}


std::vector<AliMpMotifPosition*>
get_motifpositions(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, std::string segname)
{
  AliMpVSegmentation* b = get_seg(ddlStore, mseg, segname, AliMp::kBendingPlane);
  AliMpVSegmentation* nb = get_seg(ddlStore, mseg, segname, AliMp::kNonBendingPlane);
  return get_motifpositions(*b, *nb);
}

std::vector<std::pair<std::vector<AliMpMotifPosition*>, std::vector<AliMpMotifPosition*>>>
get_motifpositions(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, const std::vector<std::string>& segnames)
{
  // return a vector of (vector of mp, vector of mp), i.e. a pair of motif position vectors per segname

  std::vector<std::pair<std::vector<AliMpMotifPosition*>, std::vector<AliMpMotifPosition*>>> mp;

  for (auto segname: segnames ) {
    AliMpVSegmentation* b_seg = get_seg(ddlStore,mseg,segname,AliMp::kBendingPlane);
    AliMpVSegmentation* nb_seg = get_seg(ddlStore,mseg,segname,AliMp::kNonBendingPlane);
    std::vector<AliMpMotifPosition*> b = get_motifpositions(*b_seg);
    std::vector<AliMpMotifPosition*> nb = get_motifpositions(*nb_seg);
    mp.push_back(std::make_pair(b,nb));
  }
  return mp;
}

