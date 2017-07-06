#ifndef ALO_JSONMAP_CREATOR_MOTIFTYPE2JSON_H
#define ALO_JSONMAP_CREATOR_MOTIFTYPE2JSON_H

#include "AliMpConnection.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpVMotif.h"
#include "json.h"
#include "pcb2json.h"
#include <cassert>
#include <string>


template<typename WRITER>
void connection2json(const AliMpConnection& c, WRITER& w) 
{
    w.StartObject();
    w.Key("berg");
    w.Int(c.GetBergNum());
    w.Key("ix");
    w.Int(c.GetLocalIx());
    w.Key("iy");
    w.Int(c.GetLocalIy());
    w.EndObject();
}

template<typename WRITER>
void motif2json(const AliMpMotifType& mt, WRITER& w)
{
  w.StartObject();
  w.Key("id");
  w.String(mt.GetID());
  w.Key("is_full");
  w.Bool(mt.IsFull());
  w.Key("pads");
  w.StartArray();
  int n = 0;
  for ( int i = 1; i <= 100; ++i ) {
    AliMpConnection* c = mt.FindConnectionByPadNum(i);
    if (!c) continue;
    connection2json(*c,w);
    ++n;
  }
  w.EndArray();
  w.Key("nof_pads");
  w.Int(n);
  w.EndObject();
}

template<typename WRITER>
void all_motif2json(const std::vector<AliMpMotifType*>& motifTypes,
        WRITER& w) 
{
    w.StartObject();
    w.Key("motifs");
    w.StartArray();

    for (auto& mt: motifTypes) {
        motif2json(*mt,w);
    }
    w.EndArray();
    w.EndObject();
}

#endif
