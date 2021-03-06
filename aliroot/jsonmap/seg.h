#ifndef SEG_H
#define SEG_H

#include <vector>
#include <string>
#include "AliMpPlaneType.h"

class AliMpVSegmentation;
class AliMpSegmentation;
class AliMpSlat;
class AliMpSector;
class AliMpDDLStore;

const AliMpSlat* slat_from_seg(const AliMpVSegmentation& seg);
const AliMpSector* sector_from_seg(const AliMpVSegmentation& seg);

bool is_slat(std::string segtype);

std::string get_segtype(const AliMpVSegmentation& seg);

std::vector<std::string> get_all_segmentation_names(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg);

std::vector<AliMpVSegmentation*> get_segs(AliMpSegmentation* mseg, const std::vector<int>& deids, AliMp::PlaneType planeType);

std::vector<const AliMpSector*> get_allsectors(AliMpSegmentation* mseg);

bool is_station2(const AliMpSector& sector);

std::string get_sector_plane_prefix(const AliMpSector& sector);

AliMpVSegmentation* get_seg(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, std::string segname, AliMp::PlaneType planeType);

#endif

