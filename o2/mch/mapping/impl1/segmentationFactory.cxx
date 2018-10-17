#include "segmentationImpl1.h"

#include <array>
#include <stdexcept>
#include <iterator>
#include <set>

#include "genSegmentationFactory.cxx"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl1 {

int getSegTypeIndexFromDetElemIndex(int deIndex)
{
  if (deIndex >= detElemIndexToSegType.size() || deIndex < 0) { throw std::runtime_error("deIndex" + std::to_string(deIndex) + " is incorrect"); }
  return detElemIndexToSegType[deIndex];
}

int getDetElemIdFromDetElemIndex(int deIndex)
{
  if (deIndex < 0 || deIndex >= detElemIndexToDetElemId.size()) { throw std::runtime_error("deIndex" + std::to_string(deIndex) + " is incorrect"); }
  return detElemIndexToDetElemId[deIndex];
}

int getDetElemIndexFromDetElemId(int deId)
{
  return std::distance(detElemIndexToDetElemId.begin(), std::find(detElemIndexToDetElemId.begin(), detElemIndexToDetElemId.end(), deId));
}

std::unique_ptr<SegmentationInterface> getSegmentationByDetElemIndex(int deIndex, bool isBendingPlane)
{
  return getSegmentationByType(getSegTypeIndexFromDetElemIndex(deIndex), isBendingPlane);
}

std::unique_ptr<SegmentationInterface>
getSegmentation(int detElemId, bool isBendingPlane)
{
  return getSegmentationByDetElemIndex(getDetElemIndexFromDetElemId(detElemId), isBendingPlane);
}

std::vector<int> getOneDetElemIdPerSegmentation()
{
  // return {100,300,500,501,502,503,504,600,601,602,700,701,702,703,704,705,706,902,903,904,905};

  std::vector<int> des;
  std::set<int> segtypeset;
  for (auto i = 0; i < detElemIndexToDetElemId.size(); ++i) {
    for (auto bending : {true, false}) {
      int id = detElemIndexToSegType[i];
      if (std::find(cbegin(segtypeset), cend(segtypeset), id) == segtypeset.end()) {
        segtypeset.insert(id);
        des.push_back(detElemIndexToDetElemId[i]);
      }
    }
  }
  return des;
}


}
}
}
}
