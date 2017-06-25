#include "AliMpFastSegmentation.h"
#include "AliMpSector.h"
#include "AliMpSectorSegmentation.h"
#include "AliMpSegmentation.h"
#include "AliMpSlat.h"
#include "AliMpSlatMotifMap.h"
#include "AliMpSlatSegmentation.h"
#include "seg.h"
#include <iostream>

bool is_slat(std::string segtype) {
    return (segtype.find("st") == std::string::npos);
}

const AliMpSlat* slat_from_seg(const AliMpVSegmentation& seg)
{
    const AliMpFastSegmentation* fast = dynamic_cast<const AliMpFastSegmentation*>(&seg);
    if (!fast) 
    {
        return nullptr;
    }

    const AliMpSlatSegmentation* slatSegmentation = dynamic_cast<const AliMpSlatSegmentation*>(fast->GetHelper());

    if (slatSegmentation) 
    {
        return slatSegmentation->Slat();
    }
    return nullptr;
}

const AliMpSector* sector_from_seg(const AliMpVSegmentation& seg)
{
    const AliMpFastSegmentation* fast = dynamic_cast<const AliMpFastSegmentation*>(&seg);
    if (!fast) 
    {
        return nullptr;
    }

    const AliMpSectorSegmentation* sectorSegmentation = dynamic_cast<const AliMpSectorSegmentation*>(fast->GetHelper());

    if (sectorSegmentation) 
    {
        return sectorSegmentation->GetSector();
    }
    return nullptr;
}

std::string get_segtype(const AliMpVSegmentation& seg) 
{
    // this method is certainly fragile...

    const AliMpSlat* slat = slat_from_seg(seg);
    if (slat)
    {
        return slat->GetID();
    }

    const AliMpSector* sector = sector_from_seg(seg);
    if (sector)
    {
        return (sector->GetDimensionX()>50.0) ? "st2" : "st1";  
    }
    return "ARGH";
}

std::vector<AliMpVSegmentation*> get_segs(AliMpSegmentation* mseg,
        std::vector<int>& deids,
        AliMp::PlaneType planeType)
{
    std::vector<AliMpVSegmentation*> segs;

    for ( auto& d:deids) 
    {
        const AliMpVSegmentation* s = mseg->GetMpSegmentation(d,AliMp::kCath0);
        std::string segtype = get_segtype(*s);

        if ( std::find_if(segs.begin(),segs.end(),
                    [&segtype](AliMpVSegmentation* o) {
                    return (get_segtype(*o) == segtype);
                    } ) != segs.end() ) 
        {
            continue;
        }

        if ( s->PlaneType() == planeType ) 
        {
            segs.push_back(const_cast<AliMpVSegmentation*>(s));
        }
    }

    return segs;
}

