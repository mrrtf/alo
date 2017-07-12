#ifndef ALO_JSONMAP_CREATOR_LEGACYSEG2JSON_H
#define ALO_JSONMAP_CREATOR_LEGACYSEG2JSON_H

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
void legacysegplane2json(const AliMpVSegmentation& seg, WRITER& w)
{
  w.String(seg.PlaneType() == AliMp::kBendingPlane ? "bending" : "non-bending");

  if (seg.StationType() == AliMp::kStation345) {
    const AliMpSlat* slat = slat_from_seg(seg);
    slat2json(*slat, w);
  } else {
    const AliMpSector* sector = sector_from_seg(seg);
    sector2json(*sector, w);
  }
}

template<typename WRITER>
void legacyseg2json(const AliMpVSegmentation& b, const AliMpVSegmentation& nb, WRITER& w)
{
  static int n = 1;

  w.StartObject();
  w.Key("gid");
  w.String(Form("gid-%d", n++));

  w.Key("id");
  std::string segtype = get_segtype(b);
  w.String(segtype.c_str());
  w.Key("is_slat");
  w.Bool(is_slat(segtype));
  legacysegplane2json(b, w);
  legacysegplane2json(nb, w);
  w.EndObject();
}

template<typename WRITER>
void all_legacyseg2json(std::string topkey, std::vector<AliMpVSegmentation*> b,
                        std::vector<AliMpVSegmentation*> nb,
                        WRITER& w)
{
  w.StartObject();
  w.Key(topkey.c_str());
  w.StartArray();

  for (std::vector<AliMpVSegmentation*>::size_type i = 0; i < b.size(); ++i) {
    legacyseg2json(*(b[i]), *(nb[i]), w);
  }
  w.EndArray();
  w.EndObject();
}

#endif
