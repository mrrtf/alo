#ifndef SEGMENTATIONPAIR_H
#define SEGMENTATIONPAIR_H

// TODO: move to O2 asap
//
#include "MCHMappingInterface/CathodeSegmentation.h"

class SegmentationPair
{
 public:
  SegmentationPair(int detElemId)
    : b{ o2::mch::mapping::CathodeSegmentation{ detElemId, true } }, nb{ o2::mch::mapping::CathodeSegmentation{ detElemId, false } } {}

  const o2::mch::mapping::CathodeSegmentation& operator[](bool plane) const { return plane ? b : nb; }

 private:
  o2::mch::mapping::CathodeSegmentation b;
  o2::mch::mapping::CathodeSegmentation nb;
};

const SegmentationPair& getSegmentationPair(int detElemId);

#endif
