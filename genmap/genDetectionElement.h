#ifndef O2_MCH_MAPPING_GENDETECTIONELEMENT_H
#define O2_MCH_MAPPING_GENDETECTIONELEMENT_H

#include <array>

namespace o2
{
namespace mch
{
namespace mapping
{

int getDetElemIdFromDetElemIndex(int deIndex);
int getDetElemIndexFromDetElemId(int deId);

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENDETECTIONELEMENT_H