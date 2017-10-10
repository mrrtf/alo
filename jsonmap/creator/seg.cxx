#include "AliMpFastSegmentation.h"
#include "AliMpSector.h"
#include "AliMpSectorSegmentation.h"
#include "AliMpSegmentation.h"
#include "AliMpSlat.h"
#include "AliMpSlatMotifMap.h"
#include "AliMpSlatSegmentation.h"
#include "seg.h"
#include "de.h"
#include <iostream>
#include <cassert>
#include <set>
#include <algorithm>

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

bool is_station2(const AliMpSector &sector) {
  return (sector.GetDimensionX()>50.0);

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
        return (is_station2(*sector)) ? "st2" : "st1";
    }
    return "ARGH";
}

std::vector<AliMpVSegmentation*> get_segs(AliMpSegmentation* mseg,
        const std::vector<int>& deids,
        AliMp::PlaneType planeType)
{
    std::vector<AliMpVSegmentation*> segs;

    for ( auto& d:deids) 
    {
        const AliMpVSegmentation* s = mseg->GetMpSegmentation(d,AliMp::kCath0);
        if (s->PlaneType() != planeType) {
          s = mseg->GetMpSegmentation(d,AliMp::kCath1);
          assert(s->PlaneType()==planeType);
        }
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
std::vector<const AliMpSector *> get_allsectors(AliMpSegmentation* mseg) {
  std::vector<const AliMpSector*> sectors;

  std::vector<int> deids { 100,300 };

  std::vector<AliMpVSegmentation*> segs = get_segs(mseg,deids,AliMp::kBendingPlane);
  std::vector<AliMpVSegmentation*> nb = get_segs(mseg,deids,AliMp::kNonBendingPlane);
  segs.insert(segs.end(),nb.begin(),nb.end());
  for ( auto& s: segs) {
    sectors.push_back(sector_from_seg(*s));
  }
  return sectors;
}


std::string get_sector_plane_prefix(const AliMpSector& sector)
{
  std::string prefix;

  if (is_station2(sector)) {
    prefix = "2";
  } else {
    prefix = "1";
  }

  prefix += (sector.GetPlaneType() == AliMp::kBendingPlane) ? "B" : "N";

  return prefix;
}

std::vector<std::string> get_all_segmentation_names(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  std::vector<std::string> segnames;

  std::vector<int> deids = get_deids(ddlStore);

  std::vector<AliMpVSegmentation*> segs = get_segs(mseg,deids,AliMp::kNonBendingPlane);

  for ( auto s: segs ) {
    std::string name = get_segtype(*s);
    if ( std::find(segnames.begin(),segnames.end(),name)==segnames.end() ) {
      segnames.push_back(name);
    }
  }
  return segnames;
}

AliMpVSegmentation* get_seg(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, std::string segname, AliMp::PlaneType planeType)
{
  std::vector<int> deids = get_deids_per_segname(ddlStore,mseg,segname);

  std::vector<AliMpVSegmentation*> segs = get_segs(mseg,deids,planeType);

  return segs[0];
}



