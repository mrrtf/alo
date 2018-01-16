//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType6(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{6,true,{},{},{}};
}
 return new Segmentation{6,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType6{ 
  public:
  SegmentationCreatorRegisterCreateSegType6() { registerSegmentationCreator(6,createSegType6); }
} aSegmentationCreatorRegisterCreateSegType6;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
