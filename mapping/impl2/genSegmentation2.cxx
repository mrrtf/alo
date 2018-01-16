//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
// This implementation file cannot be used standalone, i.e. it is intended to be included
// into another implementation file.
//
#include <array>
#include <stdexcept>
#include <vector>
#include "segmentation.h"
#include "segmentationImpl2.h"
#include "segmentationCreator.h"
#include "genDetElemId2SegType.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

Segmentation *createSegmentation2(int detElemId, bool isBendingPlane)
{
  int segType = detElemId2SegType(detElemId);
  SegmentationCreator creator = getSegmentationCreator(segType);
  if (!creator) {
    return nullptr;
  }
  return creator(isBendingPlane);
}

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
