
#include "segmentationInterface.h"
#include <array>

namespace o2 {
namespace mch {
namespace mapping {

int getSegTypeIndexFromDetElemIndex(int deIndex)
{

  static std::array<int, 156> segtype{
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 3, 4, 5, 6, 6, 5, 4, 3, 7, 8, 9, 5, 6,
    6, 5, 9, 8, 7, 8, 9, 5, 6, 6, 5, 9, 8, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15,
    16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15,
    14, 13, 12, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17,
    11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11
  };

  if (deIndex >= segtype.size()) { throw std::out_of_range("deIndex is incorrect"); }
  return segtype[deIndex];
}

std::unique_ptr<SegmentationInterface> getDESegmentation(int deIndex, bool isBendingPlane)
{
  return getSegmentation(getSegTypeIndexFromDetElemIndex(deIndex), isBendingPlane);
}

} // namespace mapping
} // namespace mch
} // namespace o2
