#ifndef SEG2JSON_H
#define SEG2JSON_H

#include "AliMpPlaneType.h"
#include <iostream>
#include <string>
#include <vector>
class AliMpDataStreams;
class AliMpVSegmentation;

bool is_slat(std::string segtype);

std::vector<std::string> get_segtypes();

void all_seg2json(std::ostream&);

AliMpVSegmentation* get_segmentation(std::string segtype, AliMp::PlaneType planeType, AliMpDataStreams& dataStreams);

#endif
