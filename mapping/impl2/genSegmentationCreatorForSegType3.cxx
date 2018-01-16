//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType3(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{3,true,{},{},{}};
}
 return new Segmentation{3,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType3{ 
  public:
  SegmentationCreatorRegisterCreateSegType3() { registerSegmentationCreator(3,createSegType3); }
} aSegmentationCreatorRegisterCreateSegType3;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
