#include "segmentationImpl0.h"

#include <array>
#include <stdexcept>
#include <iterator>

#include "genSegmentationFactory.cxx"

namespace o2 {
namespace mch {
namespace mapping {

int getSegTypeIndexFromDetElemIndex(int deIndex)
{
  if (deIndex >= segtype.size()) { throw std::out_of_range("deIndex is incorrect"); }
  return segtype[deIndex];
}

int getDetElemIdFromDetElemIndex(int deIndex)
{
  auto deids = getDetElemIds();
  if (deIndex < 0 || deIndex >= deids.size()) { throw std::out_of_range("deIndex out of range"); }
  return deids[deIndex];
}

int getDetElemIndexFromDetElemId(int deId)
{
  auto deids = getDetElemIds();
  return std::distance(deids.begin(), std::find(deids.begin(), deids.end(), deId));
}

std::unique_ptr<SegmentationInterface> getSegmentationByDetElemIndex(int deIndex, bool isBendingPlane)
{
  return getSegmentationByType(getSegTypeIndexFromDetElemIndex(deIndex), isBendingPlane);
}

std::unique_ptr<SegmentationInterface> getSegmentation(int detElemId, bool isBendingPlane)
{
  return getSegmentationByDetElemIndex(getDetElemIndexFromDetElemId(detElemId), isBendingPlane);
}

} // namespace mapping
} // namespace mch
} // namespace o2
