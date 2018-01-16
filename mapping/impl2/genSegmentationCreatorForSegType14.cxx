//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType14(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{14,true,{},{},{}};
}
 return new Segmentation{14,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType14{ 
  public:
  SegmentationCreatorRegisterCreateSegType14() { registerSegmentationCreator(14,createSegType14); }
} aSegmentationCreatorRegisterCreateSegType14;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
