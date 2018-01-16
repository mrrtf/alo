//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType9(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{9,true,{},{},{}};
}
 return new Segmentation{9,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType9{ 
  public:
  SegmentationCreatorRegisterCreateSegType9() { registerSegmentationCreator(9,createSegType9); }
} aSegmentationCreatorRegisterCreateSegType9;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
