#ifndef SEGMENTATIONPAIR_H
#define SEGMENTATIONPAIR_H

#include "AliMpVSegmentation.h"

class SegmentationPair
{
 public:
  SegmentationPair(int detElemId);

  const AliMpVSegmentation& operator[](bool plane) const { return plane ? *b : *nb; }

 private:
  const AliMpVSegmentation* b;
  const AliMpVSegmentation* nb;
};

const SegmentationPair& getSegmentationPair(int detElemId);

#endif
