//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType18(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{18,true,{},{},{}};
}
 return new Segmentation{18,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType18{ 
  public:
  SegmentationCreatorRegisterCreateSegType18() { registerSegmentationCreator(18,createSegType18); }
} aSegmentationCreatorRegisterCreateSegType18;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
