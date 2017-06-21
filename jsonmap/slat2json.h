#ifndef SLAT2JSON_H
#define SLAT2JSON_H

#include "json.h"
#include "AliMpPCB.h"
#include "AliMpMotifPosition.h"

template<typename WRITER>
void pcb2json(const AliMpPCB& pcb, WRITER& w)
{
    w.StartObject();
    w.Key("id");
    w.String(pcb.GetID());
    std::vector<int> manus;
    for ( int i = 0 ; i < pcb.GetSize(); ++i ) {
        AliMpMotifPosition* mp = pcb.GetMotifPosition(i);
        manus.push_back(mp->GetID());
        // break;
    }
    WriteArray(w,"manus",manus);
    w.EndObject();
}

template<typename WRITER>
void slat2json(const AliMpSlat& slat, WRITER& w) 
{
    w.StartObject();
    w.Key("pcbs");
    w.StartArray();
    for ( int i = 0; i < slat.GetSize(); ++i ) {
        AliMpPCB* pcb = slat.GetPCB(i);
        pcb2json(*pcb,w);
    }
    w.EndArray();
    w.EndObject();
}

#endif
