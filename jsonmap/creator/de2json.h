#ifndef ALO_JSONMAP_READER_DE2JSON_H
#define ALO_JSONMAP_READER_DE2JSON_H

#include <iostream>
#include <vector>
#include "AliMpDetElement.h"
#include "json.h"
#include <cassert>

template<typename WRITER>
void de2json(const AliMpDetElement& de,
        const std::vector<int>& bps,
        WRITER& w)
{
    w.StartObject();
    w.Key("id");
    w.Int(de.GetId());
    w.Key("name");
    w.String(de.GetDEName());
    w.Key("segtype");
    w.String(de.GetSegType());
    w.Key("segname");
    w.StartArray();
    w.String(de.GetSegName(AliMp::kCath0));
    w.String(de.GetSegName(AliMp::kCath1));
    w.EndArray();
    w.Key("cath0_is_bending_plane");
    w.Int((de.GetPlaneType(AliMp::kCath0)==AliMp::kBendingPlane)?1:0);
    WriteArray(w,"bps",bps);
    w.EndObject();
}

template<typename WRITER>
void all_de2json(std::string topkey, const std::vector<AliMpDetElement*>& des,
        const std::vector<std::vector<int>>& bpsperde, WRITER& w)
{
    assert(des.size()==bpsperde.size());
    w.StartObject();
    w.Key(topkey.c_str());
    w.StartArray();

    int i = 0;
    for ( auto& d: des) {
        de2json(*d,bpsperde[i++],w); 
    }
    w.EndArray();
    w.EndObject();
}


#endif

