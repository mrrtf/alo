#ifndef O2_MCH_MAPPING_GENDESEGMENTATIONFACTORY_H
#define O2_MCH_MAPPING_GENDESEGMENTATIONFACTORY_H

#include "genSegmentationInterface.h"
#include <memory>

namespace o2
{
namespace mch
{
namespace mapping
{


int getSegTypeIndexFromDetElemIndex(int deIndex);

std::unique_ptr<SegmentationInterface> getDESegmentation(int deIndex, bool isBendingPlane);

  
} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENDESEGMENTATIONFACTORY_H