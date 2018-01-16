//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType4(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{4,true,{},{},{}};
}
 return new Segmentation{4,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType4{ 
  public:
  SegmentationCreatorRegisterCreateSegType4() { registerSegmentationCreator(4,createSegType4); }
} aSegmentationCreatorRegisterCreateSegType4;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
