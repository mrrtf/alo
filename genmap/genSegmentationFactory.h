#ifndef O2_MCH_MAPPING_GENSEGMENTATIONFACTORY_H
#define O2_MCH_MAPPING_GENSEGMENTATIONFACTORY_H

#include "genSegmentationInterface.h"
#include "genMotifType.h"
#include <memory>

namespace o2
{
namespace mch
{
namespace mapping
{


std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane);

  
} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENSEGMENTATIONFACTORY_H