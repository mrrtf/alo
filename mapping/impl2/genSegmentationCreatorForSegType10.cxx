//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType10(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{10,true,{},{},{}};
}
 return new Segmentation{10,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType10{ 
  public:
  SegmentationCreatorRegisterCreateSegType10() { registerSegmentationCreator(10,createSegType10); }
} aSegmentationCreatorRegisterCreateSegType10;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
