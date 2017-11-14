#include "genDESegmentationFactory.h"

#include "genSegmentationFactory.h"

namespace o2
{
namespace mch
{
namespace mapping
{

  std::array<int,156> segTypeFromDEIndex{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,3,4,5,6,6,5,4,3,2,3,4,5,6,6,5,4,3,7,8,9,5,6,6,5,9,8,7,8,9,5,6,6,5,9,8,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,17,18,19,20,15,15,20,19,18,17,11,10,11,17,18,19,20,15,15,20,19,18,17,11,10,11,17,18,19,20,15,15,20,19,18,17,11,10,11,17,18,19,20,15,15,20,19,18,17,11};

  std::unique_ptr<SegmentationInterface> getDESegmentation(int deindex, bool isBendingPlane) {
      if (deindex >= segTypeFromDEIndex.size()) throw std::out_of_range("deindex is incorrect");
      return getSegmentation(segTypeFromDEIndex[deindex],isBendingPlane);
  }

} // namespace mapping
} // namespace mch
} // namespace o2
