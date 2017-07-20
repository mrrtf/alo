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


#ifndef ALO_JSONMAP_CREATOR_SEG2JSON_H
#define ALO_JSONMAP_CREATOR_SEG2JSON_H

#include "AliMpDirection.h"
#include "AliMpSector.h"
#include "AliMpSlat.h"
#include "AliMpVSegmentation.h"
#include "json.h"
#include "sector2json.h"
#include "seg.h"
#include "slat2json.h"
#include <algorithm>
#include <string>

template<typename WRITER>
void motifposition2json(const AliMpMotifPosition& motifPosition, std::string motifLabel, int motifId, int motifTypeId,
                        int padSizeId, WRITER& w)
{
  w.StartObject();
  w.Key("fec");
  w.Int(motifPosition.GetID());
  w.Key("x");
  w.Double(motifPosition.GetPositionX());
  w.Key("y");
  w.Double(motifPosition.GetPositionY());
  w.Key("motif(TBR)");
  w.Int(motifId);
  w.Key("motiftype");
  w.Int(motifTypeId);
  w.Key("padsize");
  w.Int(padSizeId);
  w.Key("motifLabel(TBR)");
  w.String(motifLabel.c_str());
  w.EndObject();
}

template<typename WRITER>
void segplane2json(std::string stype, std::string prefix, const std::vector<AliMpMotifPosition*>& mp,
                   const std::vector<AliMpVMotif*>& motifs,
                   const std::vector<AliMpMotifType*>& motiftypes,
                   const std::vector<std::pair<float, float>>& padsizes, WRITER& w)
{
  if (prefix == '1' || prefix == '2') {
    // sectors
    prefix += (stype == "bending" ? "B" : "N");
  }
  w.Key(stype.c_str());
  w.StartObject();
  w.Key("n(TBR)");
  w.Int(mp.size());
  w.Key("motifpositions");
  w.StartArray();
  for (auto m:mp) {
    AliMpVMotif* motif = m->GetMotif();
    std::string motifLabel = prefix + get_motif_id(*motif, padsizes);
    auto ix = std::find_if(motifs.begin(), motifs.end(),
                           [&](AliMpVMotif* m) { return motifLabel == m->GetID().Data(); });
    int motifId = std::distance(motifs.begin(), ix);
    int motifTypeId = get_motiftype_index(motifLabel, motiftypes);
    int padSizeId = get_padsize_index(motif->GetPadDimensionX(0) * 2.0, motif->GetPadDimensionY(0) * 2.0, padsizes);
    motifposition2json(*m, motifLabel, motifId, motifTypeId, padSizeId, w);
  }
  w.EndArray();
  w.EndObject();
}

template<typename WRITER>
void seg2json(std::string segname,
              const std::vector<AliMpMotifPosition*>& b,
              const std::vector<AliMpMotifPosition*>& nb,
              const std::vector<AliMpVMotif*>& motifs,
              const std::vector<AliMpMotifType*>& motiftypes,
              const std::vector<std::pair<float, float>>& padsizes,
              WRITER& w)
{
  w.StartObject();
  std::string prefix;
  auto i = segname.find_first_of("st");
  if (i != std::string::npos) {
    prefix = segname.substr(2, 1);
  }
  segplane2json("bending", prefix, b, motifs, motiftypes, padsizes, w);
  segplane2json("non-bending", prefix, nb, motifs, motiftypes, padsizes, w);
  w.EndObject();
}

template<typename WRITER>
void all_seg2json(std::string topkey,
                  const std::vector<std::string>& segnames,
                  const std::vector<std::pair<std::vector<AliMpMotifPosition*>,
                    std::vector<AliMpMotifPosition*>>>& motifPositions,
                  const std::vector<AliMpVMotif*>& motifs,
                  const std::vector<AliMpMotifType*>& motiftypes,
                  const std::vector<std::pair<float, float>>& padsizes,
                  WRITER& w)
{
  w.StartObject();
  w.Key(topkey.c_str());
  w.StartArray();

  for (auto i = 0; i < motifPositions.size(); ++i) {
    seg2json(segnames[i], motifPositions[i].first, motifPositions[i].second, motifs, motiftypes, padsizes, w);
  }

  w.EndArray();
  w.EndObject();
}

#endif
