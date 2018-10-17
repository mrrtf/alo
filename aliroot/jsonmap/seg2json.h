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
#include "padsize.h"
#include "sector2json.h"
#include "seg.h"
#include "slat2json.h"
#include <algorithm>
#include <string>

namespace {
bool areEqual(double a, double b)
{
  return std::fabs(b - a) < 1E-4; // 1E-4 cm = 1 micron
}
}

template<typename WRITER>
void WritePadBergNumbers(int i, const AliMpVMotif &motif, WRITER &w)
{
  std::vector<int> pads;
  AliMpMotifType *mt = motif.GetMotifType();
  for (int ix = 0; ix < mt->GetNofPadsX(); ++ix) {
    for (int iy = 0; iy < mt->GetNofPadsY(); ++iy) {
      AliMpConnection *connection = mt->FindConnectionByLocalIndices(ix, iy);
      if (connection) {
        double x, y;
        motif.GetPadDimensionsByIndices(ix, iy, x, y);
        if (areEqual(x, motif.GetPadDimensionX(i)) &&
            areEqual(y, motif.GetPadDimensionY(i))) {
          pads.push_back(connection->GetBergNum());
        }
      }
    }
  }
  WriteArray(w, "pads", pads);
}

template<typename WRITER>
void motifposition2json(const AliMpMotifPosition &motifPosition, std::string motifLabel, int motifId, int motifTypeId,
                        const std::vector<PadSize> &padsizes, WRITER &w)
{
  w.StartObject();
  w.Key("fec");
  w.Int(motifPosition.GetID());
  w.Key("x");
  w.Double(motifPosition.GetPositionX());
  w.Key("y");
  AliMpVMotif *motif = motifPosition.GetMotif();
  if ( motifLabel.find("2Bn1") != std::string::npos ||
    motifLabel.find("2Nn1") != std::string::npos ) {
    w.Double(motifPosition.GetPositionY() + motif->GetPadDimensionY(0) * 2.0);
  }
  else {
    w.Double(motifPosition.GetPositionY());
  }
  w.Key("motif(TBR)");
  w.Int(motifId);
  w.Key("motiftype");
  w.Int(motifTypeId);
  w.Key("padsize");
  auto px = motif->GetPadDimensionX(0) * 2.0;
  auto py = motif->GetPadDimensionY(0) * 2.0;
  int padSizeIndex = get_padsize_index(px, py, padsizes);
  if (motif->GetNofPadDimensions() == 1) {
    w.Int(padSizeIndex);
  } else {
    w.StartArray();
    if (motif->GetNofPadDimensions() != 2) {
      throw std::out_of_range("this code assumes a maximum of two pad sizes per motif");
    }
    auto px = motif->GetPadDimensionX(1) * 2.0;
    auto py = motif->GetPadDimensionY(1) * 2.0;
    int secondPadSizeIndex = get_padsize_index(px, py, padsizes);
    w.Int(padSizeIndex);
    w.Int(secondPadSizeIndex);
    w.StartObject();
    w.Key("x(TBR)");
    w.Double(px);
    w.Key("y(TBR)");
    w.Double(py);
    WritePadBergNumbers(1, *motif, w);
    w.EndObject();
    w.EndArray();
  }
  w.Key("motifLabel(TBR)");
  w.String(motifLabel.c_str());
  w.EndObject();
}

template<typename WRITER>
void segplane2json(std::string stype, std::string prefix, const std::vector<AliMpMotifPosition *> &mp,
                   const std::vector<AliMpVMotif *> &motifs,
                   const std::vector<AliMpMotifType *> &motiftypes,
                   const std::vector<PadSize> &padsizes, WRITER &w)
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
    AliMpVMotif *motif = m->GetMotif();
    std::string motifLabel = prefix + get_motif_id(*motif, padsizes);
    auto ix = std::find_if(motifs.begin(), motifs.end(),
                           [&](AliMpVMotif *m) { return motifLabel == m->GetID().Data(); });
    int motifId = std::distance(motifs.begin(), ix);
    int motifTypeId = get_motiftype_index(motifLabel, motiftypes);
    motifposition2json(*m, motifLabel, motifId, motifTypeId, padsizes, w);
  }
  w.EndArray();
  w.EndObject();
}

template<typename WRITER>
void seg2json(std::string segname,
              const std::vector<AliMpMotifPosition *> &b,
              const std::vector<AliMpMotifPosition *> &nb,
              const std::vector<AliMpVMotif *> &motifs,
              const std::vector<AliMpMotifType *> &motiftypes,
              const std::vector<PadSize> &padsizes,
              WRITER &w)
{
  w.StartObject();
  w.Key("segtype");
  w.String(segname.c_str());
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
                  const std::vector<std::string> &segnames,
                  const std::vector<std::pair<std::vector<AliMpMotifPosition *>,
                    std::vector<AliMpMotifPosition *>>> &motifPositions,
                  const std::vector<AliMpVMotif *> &motifs,
                  const std::vector<AliMpMotifType *> &motiftypes,
                  const std::vector<PadSize> &padsizes,
                  WRITER &w)
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
