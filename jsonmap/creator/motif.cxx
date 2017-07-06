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
#include "AliMpMotifMap.h"
#include "AliMpMotifPosition.h"
#include "AliMpVMotif.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"

std::vector<AliMpVMotif*> get_allsectormotifs(const std::vector<const AliMpSector*>& sectors)
{
  std::vector<AliMpVMotif*> motifs;

  for (auto& s: sectors) {
    std::string prefix = get_sector_plane_prefix(*s);

    AliMpMotifMap* motifMap = s->GetMotifMap();

    for (int i = 0; i < motifMap->GetNofMotifPositions(); ++i) {
      AliMpVMotif* motif = motifMap->GetMotifPosition(i)->GetMotif();
      AliMpMotifType* mt = motif->GetMotifType();
      TString id(Form("%s-%s-%e-%e",mt->GetID().Data(),motif->GetID().Data(),motif->GetPadDimensionX(0)*2.0,motif->GetPadDimensionY(0)*2.0));
      std::string newname = prefix + id.Data();
      if ( motif->GetNofPadDimensions() > 1 ) {
        newname += "(S)";
      }
      if (std::find_if(motifs.begin(), motifs.end(),
                       [&](AliMpVMotif* a) {
                         return newname == a->GetID();
                       }
      ) == motifs.end()) {
        motifs.push_back(static_cast<AliMpVMotif*>(motif->Clone(newname.c_str())));
      }
    }
  }
  return motifs;
}

std::vector<AliMpVMotif*> get_allslatmotifs(const std::vector<AliMpPCB*>& pcbs)
{
  std::vector<AliMpVMotif*> motifs;

  for (auto& pcb: pcbs) {
    for (int i = 0; i < pcb->GetSize(); ++i) {
      AliMpVMotif* motif = pcb->GetMotifPosition(i)->GetMotif();
      if (std::find_if(motifs.begin(), motifs.end(),
                       [&motif](AliMpVMotif* a) {
                         return strcmp(a->GetID(), motif->GetID()) == 0;
                       }
      ) == motifs.end()) {
        motifs.push_back(motif);
      }
    }
  }
  return motifs;
}
