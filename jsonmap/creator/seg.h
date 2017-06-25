#ifndef SEG_H
#define SEG_H

#include <vector>
#include <string>
#include "AliMpPlaneType.h"

class AliMpVSegmentation;
class AliMpSegmentation;
class AliMpSlat;
class AliMpSector;

const AliMpSlat* slat_from_seg(const AliMpVSegmentation& seg);
const AliMpSector* sector_from_seg(const AliMpVSegmentation& seg);

bool is_slat(std::string segtype);

std::string get_segtype(const AliMpVSegmentation& seg);

std::vector<AliMpVSegmentation*> get_segs(AliMpSegmentation* mseg, std::vector<int>& deids, AliMp::PlaneType planeType);


#endif

