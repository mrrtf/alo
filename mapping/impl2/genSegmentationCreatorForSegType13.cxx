//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
#include "segmentationCreator.h"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {
Segmentation* createSegType13(bool isBendingPlane) {
 if (isBendingPlane) { 
   return new Segmentation{13,true,{},{},{}};
}
 return new Segmentation{13,true,{},{},{} };
}
class SegmentationCreatorRegisterCreateSegType13{ 
  public:
  SegmentationCreatorRegisterCreateSegType13() { registerSegmentationCreator(13,createSegType13); }
} aSegmentationCreatorRegisterCreateSegType13;

} // namespace impl2
} // namespace mapping
} // namespace mch
} // namespace o2
