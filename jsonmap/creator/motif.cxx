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

#include "motif.h"
#include "seg.h"
#include "AliMpSector.h"
#include <string>
#include <iostream>
#include "AliMpMotifMap.h"
#include "AliMpMotifPosition.h"
#include "AliMpVMotif.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "padsize.h"
#include <sstream>
#include <cassert>

int get_padsize_index(float px, float py, const std::vector<std::pair<float, float>>& padsizes)
{
  auto pix = std::find_if(padsizes.begin(), padsizes.end(), [&](std::pair<float, float> p) {
    return std::abs(p.first - px) < 1E-4 && std::abs(p.second - py) < 1E-4;
  });
  return pix - padsizes.begin();
}


std::string get_motif_id(const AliMpVMotif& motif, const std::vector<std::pair<float, float>>& padsizes)
{
  int index = get_padsize_index(motif.GetPadDimensionX(0) * 2.0, motif.GetPadDimensionY(0) * 2.0, padsizes);

  std::ostringstream s;

  s << motif.GetMotifType()->GetID().Data();

  if (motif.GetNofPadDimensions()>1) {
    s << "*";
  }

  s << "-" << index;

  return s.str();
}

std::vector<AliMpVMotif*> get_allsectormotifs(const std::vector<const AliMpSector*>& sectors,
                                              const std::vector<std::pair<float, float>>& padsizes)
{
  std::vector<AliMpVMotif*> motifs;

  for (auto& s: sectors) {
    std::string prefix = get_sector_plane_prefix(*s);

    AliMpMotifMap* motifMap = s->GetMotifMap();

    for (int i = 0; i < motifMap->GetNofMotifPositions(); ++i) {
      AliMpVMotif* motif = motifMap->GetMotifPosition(i)->GetMotif();

      std::string newname = prefix + get_motif_id(*motif,padsizes);

      if (std::find_if(motifs.begin(), motifs.end(),
                       [&](AliMpVMotif* a) {
                         return newname == a->GetID().Data();
                       }
      ) == motifs.end()) {
        motifs.push_back(static_cast<AliMpVMotif*>(motif->Clone(newname.c_str())));
      }
    }
  }
  return motifs;
}

std::vector<AliMpVMotif*> get_allslatmotifs(const std::vector<AliMpPCB*>& pcbs,
                                            const std::vector<std::pair<float, float>>& padsizes)
{
  std::vector<AliMpVMotif*> motifs;
  std::vector<std::string> names;

  for (auto& pcb: pcbs) {
    for (int i = 0; i < pcb->GetSize(); ++i) {
      AliMpVMotif* motif = pcb->GetMotifPosition(i)->GetMotif();
      std::string newname = get_motif_id(*motif,padsizes);
      names.push_back(newname);
      if (std::find_if(motifs.begin(), motifs.end(),
                       [&](AliMpVMotif* a) {
                         return newname == a->GetID().Data();
                       }
      ) == motifs.end()) {
        motifs.push_back(static_cast<AliMpVMotif*>(motif->Clone(newname.c_str())));
      }
    }
  }
  return motifs;
}

std::vector<AliMpVMotif*> get_allmotifs(const std::vector<AliMpPCB*>& pcbs,
                                        const std::vector<const AliMpSector*>& sectors,
                                        const std::vector<std::pair<float, float>>& padsizes)
{
  std::vector<AliMpVMotif*> motifs = get_allslatmotifs(pcbs, padsizes);
  std::vector<AliMpVMotif*> sectorMotifs = get_allsectormotifs(sectors, padsizes);

  motifs.insert(motifs.end(), sectorMotifs.begin(), sectorMotifs.end());

  return motifs;
}

