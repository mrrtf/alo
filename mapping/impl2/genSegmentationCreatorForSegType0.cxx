//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation *createSegType0(bool isBendingPlane)
{
  if (isBendingPlane) {
    return new Segmentation{0, true, {}, {}, {}};
  }
  return new Segmentation{0, true, {}, {}, {}};
}

class SegmentationCreatorRegisterCreateSegType0
{
  public:
    SegmentationCreatorRegisterCreateSegType0()
    { registerSegmentationCreator(0, createSegType0); }
} aSegmentationCreatorRegisterCreateSegType0;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
