#include "padSize.h"
#include <utility>
#include <array>

namespace o2 {
namespace mch {
namespace mapping {
namespace {
std::array<std::pair<float, float>, 18> arrayOfPadSizes{
/* 0 */ std::make_pair<float, float>(0.63, 0.42),
/* 1 */ std::make_pair<float, float>(0.84, 0.42),
/* 2 */ std::make_pair<float, float>(0.75, 0.5),
/* 3 */ std::make_pair<float, float>(1.26, 0.42),
/* 4 */ std::make_pair<float, float>(0.63, 0.84),
/* 5 */ std::make_pair<float, float>(1.5, 0.5),
/* 6 */ std::make_pair<float, float>(0.75, 1),
/* 7 */ std::make_pair<float, float>(2.52, 0.42),
/* 8 */ std::make_pair<float, float>(0.63, 1.68),
/* 9 */ std::make_pair<float, float>(2.5, 0.5),
/* 10 */ std::make_pair<float, float>(0.75, 2),
/* 11 */ std::make_pair<float, float>(3, 0.5),
/* 12 */ std::make_pair<float, float>(0.714286, 2.5),
/* 13 */ std::make_pair<float, float>(0.63, 3.36),
/* 14 */ std::make_pair<float, float>(5, 0.5),
/* 15 */ std::make_pair<float, float>(0.714286, 5),
/* 16 */ std::make_pair<float, float>(10, 0.5),
/* 17 */ std::make_pair<float, float>(0.714286, 10)
};
}

double padSizeX(int i)
{ return arrayOfPadSizes[i].first; }

double padSizeY(int i)
{ return arrayOfPadSizes[i].second; }

} // namespace mapping
} // namespace mch
} // namespace o2
