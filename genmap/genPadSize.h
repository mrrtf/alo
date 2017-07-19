#ifndef O2_MCH_MAPPING_GENPADSIZE_H
#define O2_MCH_MAPPING_GENPADSIZE_H

#include <array>
#include <utility>

namespace o2
{
namespace mch
{
namespace mapping
{
using PadSizeArray = std::array<std::pair<float,float>,18>;
extern PadSizeArray arrayOfPadSizes;

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENPADSIZE_H