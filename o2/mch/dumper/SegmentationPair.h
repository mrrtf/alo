#ifndef SEGMENTATIONPAIR_H
#define SEGMENTATIONPAIR_H

// TODO: move to O2 asap
//
#include "MCHMappingInterface/Segmentation.h"

class SegmentationPair
{
 public:
  SegmentationPair(int detElemId)
    : b{ o2::mch::mapping::Segmentation{ detElemId, true } }, nb{ o2::mch::mapping::Segmentation{ detElemId, false } } {}

  const o2::mch::mapping::Segmentation& operator[](bool plane) const { return plane ? b : nb; }

 private:
  o2::mch::mapping::Segmentation b;
  o2::mch::mapping::Segmentation nb;
};

const SegmentationPair& getSegmentationPair(int detElemId);

#endif
