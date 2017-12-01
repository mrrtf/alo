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
    int getNofPads() const { return mNofPads; }
    int getNofPadsX() const {
      auto result = std::minmax_element(mIx.begin(),mIx.end());
      return 1+*result.second - *result.first;
    }
    int getNofPadsY() const {
      auto result = std::minmax_element(begin(mIy), end(mIy));
      return 1+*result.second - *result.first;
    }
    int getBerg(int i) const { return mBerg[i]; }
    int getIx(int i) const { return mIx[i]; }
    int getIy(int i) const { return mIy[i]; }

    /// Return the index of the pad with connector number = berg
    /// or -1 if not found
    int padIdByBerg(int berg) const { int r = std::distance(begin(mBerg),std::find(begin(mBerg),end(mBerg),berg)); if (r<getNofPads()) return r; else return -1; }

    bool hasPadByBerg(int berg) const { int f = padIdByBerg(berg); return f>=0 && f < getNofPads(); }

    /// Return the index of the pad with indices = (ix,iy)
    /// or -1 if not found
    int padIdByIndices(int ix, int iy) const {
      for ( auto i = 0; i < mIx.size(); ++i ) {
        if (mIx[i]==ix && mIy[i]==iy) {
          return i;
        }
      }
      return -1;
    }

    bool hasPadByIndices(int ix, int iy) const { int f = padIdByIndices(ix,iy); return f>=0 && f < getNofPads(); }

  private:
   std::array<int,64> mBerg;
   std::array<int,64> mIx;
   std::array<int,64> mIy;
   int mNofPads;
};

using MotifTypeArray = std::array<MotifType,210>;
extern MotifTypeArray arrayOfMotifTypes;

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENMOTIFTYPE_H