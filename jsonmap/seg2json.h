#ifndef SEG2JSON_H
#define SEG2JSON_H

#include "AliMpVSegmentation.h"
#include "seg.h"
#include "json.h"
#include <string>
#include "AliMpSlat.h"
#include "AliMpSlatSegmentation.h"
#include "AliMpPCB.h"
#include "AliMpMotifPosition.h"
#include <algorithm>

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

template<typename WRITER>
void segplane2json(const AliMpVSegmentation& seg, WRITER& w)
{
    w.String(seg.PlaneType()==AliMp::kBendingPlane ? "bending" : "non-bending");

    if (seg.StationType()==AliMp::kStation345) {
        const AliMpSlat* slat = slat_from_seg(seg);
        slat2json(*slat,w);
    }
    else {
        w.StartObject();
        w.Key("class");
        w.String(seg.ClassName());
        w.EndObject();
    }
}

template<typename WRITER>
void seg2json(const AliMpVSegmentation& b, const AliMpVSegmentation& nb, WRITER& w)
{
    w.StartObject();
    w.Key("id");
    std::string segtype = get_segtype(b);
    w.String(segtype.c_str());
    w.Key("is_slat");
    w.Bool(is_slat(segtype));
    segplane2json(b,w);
    segplane2json(nb,w);
    w.EndObject();
}

template<typename WRITER>
void all_seg2json(std::vector<AliMpVSegmentation*> b,
        std::vector<AliMpVSegmentation*> nb, 
        WRITER& w) 
{
    w.StartObject();
    w.Key("segs");
    w.StartArray();

    for ( std::vector<AliMpVSegmentation*>::size_type i = 0; i < b.size(); ++i ) {
        seg2json(*(b[i]),*(nb[i]),w);
    }
    w.EndArray();
    w.EndObject();
}


#endif
