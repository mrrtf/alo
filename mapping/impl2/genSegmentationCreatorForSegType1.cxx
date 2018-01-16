//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType1(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{1,true,{},{},{}};
}
 return new Segmentation{1,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType1{ 
  public:
  SegmentationCreatorRegisterCreateSegType1() { registerSegmentationCreator(1,createSegType1); }
} aSegmentationCreatorRegisterCreateSegType1;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
