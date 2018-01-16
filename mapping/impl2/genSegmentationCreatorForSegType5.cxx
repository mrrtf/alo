//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType5(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{5,true,{},{},{}};
}
 return new Segmentation{5,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType5{ 
  public:
  SegmentationCreatorRegisterCreateSegType5() { registerSegmentationCreator(5,createSegType5); }
} aSegmentationCreatorRegisterCreateSegType5;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
