#ifndef O2_MCH_MAPPING_GENSEGMENTATIONINTERFACE_H
#define O2_MCH_MAPPING_GENSEGMENTATIONINTERFACE_H


#include "contour.h"

namespace o2
{
namespace mch
{
namespace mapping
{


class SegmentationInterface {
  public:
    virtual bool isBendingPlane() const = 0;
    virtual int getId() const = 0;
    virtual int nofDualSampas() const = 0;
    virtual int nofPads() const = 0;
    virtual bool hasPadByPosition(double x, double y) const = 0;
    virtual bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const = 0;
    virtual o2::mch::contour::Contour<double> getEnvelop() const = 0;
    virtual std::vector<o2::mch::contour::Contour<double>> getSampaContours() const = 0;
};

} // namespace mapping
} // namespace mch
} // namespace o2

#endif // O2_MCH_MAPPING_GENSEGMENTATIONINTERFACE_H