#ifndef O2_MCH_MAPPING_GENSEGMENTATIONIMPL0_H
#define O2_MCH_MAPPING_GENSEGMENTATIONIMPL0_H

#include <vector>
#include "genMotifType.h"
#include <tuple>
#include "genPadSize.h"
#include "genSegmentationInterface.h"

namespace o2
{
namespace mch
{
namespace mapping
{

template<int N, bool>
class SegmentationImpl0 : public SegmentationInterface
{
  public:
    SegmentationImpl0()
    { throw std::out_of_range("invalid segmentation initialization"); }

    int getId() const override
    { return mId; }

    bool isBendingPlane() const override
    { return mIsBendingPlane; }

    int nofDualSampas() const override
    { return mFecId.size(); }

    int nofPads() const override
    {
      return mNofPads;
    }

    bool hasPadByPosition(float x, float y) const override {
       return false;
    }

    bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const override {
       return false;
    }

  private:

    struct Pad {
      float xBottomLeft;
      float yBottomLeft;
      float xTopRight;
      float yTopRight;
    };

    int mId;
    bool mIsBendingPlane;
    int mNofPads;
    std::vector<Pad> mPads;
    std::vector<int> mFecId;
};

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENSEGMENTATIONIMPL0_H