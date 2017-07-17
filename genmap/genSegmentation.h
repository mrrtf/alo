#ifndef ALO_JSONMAP_READER_GENSEGMENTATION_H
#define ALO_JSONMAP_READER_GENSEGMENTATION_H

#include <vector>

namespace o2
{
namespace mch
{
namespace mapping
{
struct MotifPosition
{
    float x;
    float y;
    int motifTypeId;
    int padSizeId;
    int fecId;
};

template<int N, bool isBendingPlane>
class Segmentation
{
  public:
    Segmentation()
    { throw std::out_of_range("invalid segmentation initialization"); }

    int Id() const
    { return mId; }

    int IsBendingPlane() const
    { return mIsBendingPlane; }

    int NofDualSampas() const
    { return mMP.size(); }

  private:
    int mId;
    bool mIsBendingPlane;
    std::vector <MotifPosition> mMP;
};

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // ALO_JSONMAP_READER_GENSEGMENTATION_H