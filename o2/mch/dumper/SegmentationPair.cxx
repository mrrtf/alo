#include "SegmentationPair.h"
#include <map>
#include <memory>

const SegmentationPair& getSegmentationPair(int detElemId)
{
  static std::map<int, std::unique_ptr<SegmentationPair>> segpairs;

  auto it = segpairs.find(detElemId);
  if (it != segpairs.end()) {
    return *(it->second);
  } else {
    segpairs.insert(std::make_pair(detElemId, std::make_unique<SegmentationPair>(detElemId)));
    return getSegmentationPair(detElemId);
  }
}

