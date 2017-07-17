#ifndef O2_MCH_MAPPING_GENMOTIFTYPE_H
#define O2_MCH_MAPPING_GENMOTIFTYPE_H

#include <array>
#include <algorithm>
#include <iostream>
#include <vector>
#include <array>

namespace o2
{
namespace mch
{
namespace mapping
{

class MotifType {
  public:
    MotifType(const std::array<int,64>& berg, const std::array<int,64>& ix, const std::array<int,64>& iy, int nofPads) :
      mBerg(berg), mIx(ix), mIy(iy), mNofPads(nofPads) {}
    int NofPads() const { return mNofPads; }
    int NofPadsX() const {
      auto result = std::minmax_element(mIx.begin(),mIx.begin()+mNofPads);
      return 1+*result.second - *result.first;
    }
    int NofPadsY() const {
      auto result = std::minmax_element(begin(mIy), end(mIy)+mNofPads);
      return 1+*result.second - *result.first;
    }
  private:
   std::array<int,64> mBerg;
   std::array<int,64> mIx;
   std::array<int,64> mIy;
   int mNofPads;
};

extern std::array<MotifType,210> ArrayOfMotifTypes;

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENMOTIFTYPE_H