//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType12(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{12,true,{},{},{}};
}
 return new Segmentation{12,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType12{ 
  public:
  SegmentationCreatorRegisterCreateSegType12() { registerSegmentationCreator(12,createSegType12); }
} aSegmentationCreatorRegisterCreateSegType12;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
