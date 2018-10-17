#include "SegmentationPair.h"
#include "MappingHelper.h"
#include "AliMpDetElement.h"
#include "AliMpPlaneType.h"
#include <map>

SegmentationPair::SegmentationPair(int detElemId) : b{nullptr}, nb{nullptr} {

  AliMpDetElement *de = getDDLStore()->GetDetElement(detElemId, false);

  b = mseg()->GetMpSegmentation(detElemId,de->GetCathodType(AliMp::kBendingPlane));
  nb = mseg()->GetMpSegmentation(detElemId,de->GetCathodType(AliMp::kNonBendingPlane));
}

const SegmentationPair &getSegmentationPair(int detElemId) {
  static std::map<int, std::unique_ptr<SegmentationPair>> segpairs;

  auto it = segpairs.find(detElemId);
  if (it != segpairs.end()) {
    return *(it->second);
  } else {
    segpairs.insert(std::make_pair(
        detElemId, std::unique_ptr<SegmentationPair>(new SegmentationPair(detElemId))));
    return getSegmentationPair(detElemId);
  }
}

