#ifndef ALO_JSONMAP_CREATOR_MOTIF2JSON_H
#define ALO_JSONMAP_CREATOR_MOTIF2JSON_H

#include "AliMpConnection.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpVMotif.h"
#include "json.h"
#include "pcb2json.h"
#include "motifType.h"
#include <cassert>
#include <string>

template<typename WRITER>
void motif2json(int index, const AliMpVMotif& motif, const std::vector<AliMpMotifType*>& motifTypes,
                std::vector<std::pair<float, float>>& padsizes, WRITER& w)
{
  w.StartObject();
  w.Key("index(TBR)");
  w.Int(index);
  w.Key("id(TBR)");
  w.String(motif.GetID());
  w.Key("motifType");
  w.Int(get_motiftype_index(motif.GetID().Data(),motifTypes));
  w.Key("padsize");
  w.StartArray();
  for (int i = 0; i < motif.GetNofPadDimensions(); ++i) {
    float px = motif.GetPadDimensionX(i) * 2.0;
    float py = motif.GetPadDimensionY(i) * 2.0;
    int pix = get_padsize_index(px,py,padsizes);
    w.Int(pix);
    w.StartObject();
    w.Key("x(TBR)");
    w.Double(px);
    w.Key("y(TBR)");
    w.Double(py);
    w.EndObject();
  }
  w.EndArray();
  w.EndObject();
}

template<typename WRITER>
void all_motif2json(std::string topkey, const std::vector<AliMpVMotif*>& motifs, const std::vector<AliMpMotifType*>& motifTypes,
                    std::vector<std::pair<float, float>>& padsizes, WRITER& w)
{
  // convert a vector of motifs to JSON

  w.StartObject();
  w.Key(topkey.c_str());
  w.StartArray();

  for (auto i = 0; i < motifs.size(); ++i) {
    motif2json(i,*motifs[i], motifTypes, padsizes, w);
  }
  w.EndArray();
  w.EndObject();
}

#endif
