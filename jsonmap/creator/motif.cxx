#include "motif.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpMotifPosition.h"
#include <algorithm>
#include <iostream>
#include "seg.h"
#include "AliMpPlaneType.h"
#include "AliMpSector.h"
#include "AliMpMotifMap.h"

std::vector<AliMpMotifType*> get_allslatmotiftypes(const std::vector<AliMpPCB *> &pcbs) {

    std::vector<AliMpMotifType*> motifTypes;

  for (auto& pcb: pcbs) {
    for ( int i = 0; i < pcb->GetSize(); ++i ) {
      AliMpMotifType* mt = pcb->GetMotifPosition(i)->GetMotif()->GetMotifType();
      if ( std::find_if(motifTypes.begin(),motifTypes.end(),
                        [&mt](AliMpMotifType* a) {
                          return strcmp(a->GetID(),mt->GetID())==0; }
      ) == motifTypes.end()) {
        motifTypes.push_back(mt);
      }
    }
  }

    return motifTypes;
}

std::vector<AliMpMotifType *> get_allsectormotiftypes(const std::vector<const AliMpSector *>& sectors) {

  std::vector<AliMpMotifType *> motifTypes;

  for (auto &s: sectors) {
    std::string prefix;

    if (is_station2(*s)) {
      prefix = "2";
    } else {
      prefix = "1";
    }

    prefix += (s->GetPlaneType() == AliMp::kBendingPlane) ? "B" : "N";

    AliMpMotifMap *motifMap = s->GetMotifMap();

    for (int i = 0; i < motifMap->GetNofMotifPositions(); ++i) {
      AliMpMotifType* mt = motifMap->GetMotifPosition(i)->GetMotif()->GetMotifType();
      std::string newname = prefix + mt->GetID().Data();
      if (std::find_if(motifTypes.begin(), motifTypes.end(),
                       [&](AliMpMotifType *a) {
                         return newname == a->GetID();
                       }
      ) == motifTypes.end()) {
        motifTypes.push_back(static_cast<AliMpMotifType*>(mt->Clone(newname.c_str())));
      }
    }
  }
  return motifTypes;
}

