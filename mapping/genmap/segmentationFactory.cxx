#include "segmentationImpl0.h"

#include <array>
#include <stdexcept>
#include <iterator>
#include <set>

#include "genSegmentationFactory.cxx"

namespace o2 {
namespace mch {
namespace mapping {

int getSegTypeIndexFromDetElemIndex(int deIndex)
{
  if (deIndex >= segtype.size() || deIndex<0) { throw std::out_of_range("deIndex is incorrect"); }
  return segtype[deIndex];
}

int getDetElemIdFromDetElemIndex(int deIndex)
{
  static auto deids = getDetElemIds();
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

std::vector<int> getOneDetElemIdPerSegmentation()
{
  // return {100,300,500,501,502,503,504,600,601,602,700,701,702,703,704,705,706,902,903,904,905};

  std::vector<int> des;
  std::set<int> segtypeset;
  auto deids{getDetElemIds()};
  for (auto i = 0; i < deids.size(); ++i) {
    for ( auto bending : {true,false}) {
      int id = segtype[i];
      if (std::find(cbegin(segtypeset),cend(segtypeset),id)==segtypeset.end()) {
        segtypeset.insert(id);
        des.push_back(deids[i]);
      }
    }
  }
  return des;
}


} // namespace mapping
} // namespace mch
} // namespace o2
