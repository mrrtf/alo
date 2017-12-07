#ifndef O2_MCH_MAPPING_GENSEGMENTATIONINTERFACE_H
#define O2_MCH_MAPPING_GENSEGMENTATIONINTERFACE_H

#include "pad.h"

namespace o2 {
namespace mch {
namespace mapping {

class SegmentationInterface
{
  public:
    virtual bool isBendingPlane() const = 0;

    virtual int getId() const = 0;

    virtual int nofDualSampas() const = 0;

    virtual int nofPads() const = 0;

    virtual int getSampaId(int dualSampaIndex) const = 0;

    virtual bool hasPadByPosition(double x, double y) const = 0;

    virtual bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const = 0;

    virtual std::vector<Pad> getPads(int dualSampaId) const = 0;

    /* TODO
    virtual Pad getPad(double x, double y) const = 0
    virtual Pad getPad(int dualSampaId, int dualSampaChannel) const = 0
    virtual std::vector<Pad> getPads(Area area) const = 0;
     */
};

int getNumberOfSegmentations();

std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane);

int getSegTypeIndexFromDetElemIndex(int deIndex);

int getDetElemIdFromDetElemIndex(int deIndex);

int getDetElemIndexFromDetElemId(int deId);

std::unique_ptr<SegmentationInterface> getDESegmentation(int deIndex, bool isBendingPlane);


} // namespace mapping
} // namespace mch
} // namespace o2

#endif