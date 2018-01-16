//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType17(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{17,true,{},{},{}};
}
 return new Segmentation{17,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType17{ 
  public:
  SegmentationCreatorRegisterCreateSegType17() { registerSegmentationCreator(17,createSegType17); }
} aSegmentationCreatorRegisterCreateSegType17;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
