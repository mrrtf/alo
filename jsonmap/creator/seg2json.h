#ifndef SEG2JSON_H
#define SEG2JSON_H

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
void segplane2json(const AliMpVSegmentation& seg, WRITER& w)
{
    w.String(seg.PlaneType()==AliMp::kBendingPlane ? "bending" : "non-bending");

    if (seg.StationType()==AliMp::kStation345) {
        const AliMpSlat* slat = slat_from_seg(seg);
        slat2json(*slat,w);
    }
    else {
        const AliMpSector* sector = sector_from_seg(seg);
        sector2json(*sector,w);
    }
}

template<typename WRITER>
void seg2json(const AliMpVSegmentation& b, const AliMpVSegmentation& nb, WRITER& w)
{
    static int n = 1;

    w.StartObject();
    w.Key("gid");
    w.String(Form("gid-%d",n++));

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
