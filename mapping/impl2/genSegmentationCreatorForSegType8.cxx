//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType8(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{8,true,{},{},{}};
}
 return new Segmentation{8,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType8{ 
  public:
  SegmentationCreatorRegisterCreateSegType8() { registerSegmentationCreator(8,createSegType8); }
} aSegmentationCreatorRegisterCreateSegType8;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
