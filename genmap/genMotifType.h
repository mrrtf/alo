#ifndef ALO_JSONMAP_READER_GENMOTIFTYPE_H
#define ALO_JSONMAP_READER_GENMOTIFTYPE_H

#include <array>
#include <algorithm>
#include <iostream>
#include <vector>

namespace o2
{
namespace mch
{
namespace mapping
{

class MotifType {
public:
  MotifType(const std::array<int,64>& berg, const std::array<int,64>& ix, const std::array<int,64>& iy, int nofPads) :
  mBerg(berg), mIx(ix), mIy(iy), mNofPads(nofPads) {
  }
  int NofPads() { return mNofPads; }
  int NofPadsX() { 
    auto result = std::minmax_element(mIx.begin(),mIx.begin()+mNofPads);
    return 1+*result.second - *result.first;
  }
  int NofPadsY() { 
    auto result = std::minmax_element(begin(mIy), end(mIy)+mNofPads);
    return 1+*result.second - *result.first;
  }
private:
  std::array<int,64> mBerg;
  std::array<int,64> mIx;
  std::array<int,64> mIy;
  int mNofPads;
};

std::vector<MotifType> MotifTypes();

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // ALO_JSONMAP_READER_GENMOTIFTYPE_H