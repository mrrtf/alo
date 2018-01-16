//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType16(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{16,true,{},{},{}};
}
 return new Segmentation{16,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType16{ 
  public:
  SegmentationCreatorRegisterCreateSegType16() { registerSegmentationCreator(16,createSegType16); }
} aSegmentationCreatorRegisterCreateSegType16;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
