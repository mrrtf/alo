//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType2(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{2,true,{},{},{}};
}
 return new Segmentation{2,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType2{ 
  public:
  SegmentationCreatorRegisterCreateSegType2() { registerSegmentationCreator(2,createSegType2); }
} aSegmentationCreatorRegisterCreateSegType2;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
