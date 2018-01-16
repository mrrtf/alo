//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType15(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{15,true,{},{},{}};
}
 return new Segmentation{15,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType15{ 
  public:
  SegmentationCreatorRegisterCreateSegType15() { registerSegmentationCreator(15,createSegType15); }
} aSegmentationCreatorRegisterCreateSegType15;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
