//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType20(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{20,true,{},{},{}};
}
 return new Segmentation{20,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType20{ 
  public:
  SegmentationCreatorRegisterCreateSegType20() { registerSegmentationCreator(20,createSegType20); }
} aSegmentationCreatorRegisterCreateSegType20;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
