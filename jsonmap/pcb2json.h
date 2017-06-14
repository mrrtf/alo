#ifndef PCB2JSON_H
#define PCB2JSON_H

#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpVMotif.h"
#include "json.h"
#include "pcb2json.h"
#include "seg2json.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

template<typename WRITER>
void pcbmotifs2json(const AliMpPCB& pcb, WRITER& w)
{
    w.StartObject();
    w.Key("id");
    w.String(pcb.GetID());
    w.Key("padsize");
    w.StartObject();
    w.Key("x"); 
    w.Double(pcb.PadSizeX());
    w.Key("y"); 
    w.Double(pcb.PadSizeY());
    w.Key("unit");
    w.String("cm");
    w.EndObject();
    w.Key("motifs");
    w.StartArray();
    for ( int i = 0; i < pcb.GetSize(); ++i ) {
        w.StartObject();
        AliMpMotifPosition* mp = pcb.GetMotifPosition(i);
        AliMpVMotif* m = mp->GetMotif();
        AliMpMotifType* mt = m->GetMotifType();
        assert(m->GetNofPadDimensions()==1);
        int ix = std::round((mp->GetPositionX()-m->DimensionX()-pcb.Xmin()) / (2*m->GetPadDimensionX(0)));
        int iy = (mp->GetPositionY()-m->DimensionY()) / (2*m->GetPadDimensionY(0));
        w.Key("id");
        w.String(mt->GetID());
        w.Key("pos");
        w.StartObject();
        w.Key("ix");
        w.Int(ix);
        w.Key("iy");
        w.Int(iy);
        w.EndObject();
        w.EndObject();
    }
    w.EndArray();
    w.EndObject();
}

template<typename WRITER>
void all_pcb2json(const std::vector<AliMpPCB*>& pcbs,
        WRITER& w) 
{

    w.StartObject();
    w.Key("pcbs");
    w.StartArray();
    for ( auto& s: pcbs ) {
        pcbmotifs2json(*s,w);
    } 
    w.EndArray();
    w.EndObject();
}

#endif

