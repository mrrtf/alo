#include "AliMpFastSegmentation.h"
#include "AliMpSector.h"
#include "AliMpSectorSegmentation.h"
#include "AliMpSegmentation.h"
#include "AliMpSlat.h"
#include "AliMpSlatMotifMap.h"
#include "AliMpSlatSegmentation.h"
#include "AliMpVSegmentation.h"
#include "seg.h"
#include <cassert>
#include <string>

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
    // this method is certainly fragile, as it assumes a lot 
    // about the hierarchy of classes...

    const AliMpSlat* slat = slat_from_seg(seg);
    if (slat)
    {
        return slat->GetID();
    }

    const AliMpSector* sector = sector_from_seg(seg);
    if (sector)
    {
        const AliMpSectorSegmentation* sectorSegmentation = dynamic_cast<const AliMpSectorSegmentation*>(dynamic_cast<const AliMpFastSegmentation*>(&seg)->GetHelper());
        std::string st = AliMp::StationTypeName(sectorSegmentation->StationType()).Data();
        st += AliMp::PlaneTypeName(sector->GetPlaneType()).Data();
        return st;
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

// void get_segs(AliMpSegmentation* mseg, std::vector<int>& deids,
//         std::vector<AliMpVSegmentation*>& b,
//         std::vector<AliMpVSegmentation*>& nb)
// {
//     std::vector<AliMpVSegmentation*> segs;
//
//     for ( auto& d:deids) 
//     {
//         const AliMpVSegmentation* s = mseg->GetMpSegmentation(d,AliMp::kCath0);
//         std::string segtype = get_segtype(*s);
//
//         if ( std::find_if(b.begin(),b.end(),
//                     [&segtype](AliMpVSegmentation* o) {
//                     return (get_segtype(*o) == segtype);
//                     } ) != b.end() ) 
//         {
//             continue;
//         }
//
//         if ( s->PlaneType() == AliMp::kBendingPlane )
//         {
//             b.push_back(const_cast<AliMpVSegmentation*>(s));
//             nb.push_back(const_cast<AliMpVSegmentation*>(mseg->GetMpSegmentation(d,AliMp::kCath1)));
//         }
//         else
//         {
//             nb.push_back(const_cast<AliMpVSegmentation*>(s));
//             b.push_back(const_cast<AliMpVSegmentation*>(mseg->GetMpSegmentation(d,AliMp::kCath1)));
//         }
//     }
// }
//
