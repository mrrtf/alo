//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType11(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{11,true,{},{},{}};
}
 return new Segmentation{11,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType11{ 
  public:
  SegmentationCreatorRegisterCreateSegType11() { registerSegmentationCreator(11,createSegType11); }
} aSegmentationCreatorRegisterCreateSegType11;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
