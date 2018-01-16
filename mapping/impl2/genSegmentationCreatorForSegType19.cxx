//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType19(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{19,true,{},{},{}};
}
 return new Segmentation{19,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType19{ 
  public:
  SegmentationCreatorRegisterCreateSegType19() { registerSegmentationCreator(19,createSegType19); }
} aSegmentationCreatorRegisterCreateSegType19;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
