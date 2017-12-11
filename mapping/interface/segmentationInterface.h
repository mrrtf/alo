#ifndef O2_MCH_MAPPING_GENSEGMENTATIONINTERFACE_H
#define O2_MCH_MAPPING_GENSEGMENTATIONINTERFACE_H

#include <vector>

namespace o2 {
namespace mch {
namespace mapping {

///
/// SegmentationInterface is able to retrieve pad information
/// (position, sizes, etc...) within a plane of a detection element
///
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

    virtual std::vector<int> getPads(int dualSampaId) const = 0;

    virtual void getPadPosition(int ph, double& x, double& y) const = 0;

    virtual void getPadDimension(int ph, double& dx, double& dy) const = 0;

    /* TODO
    virtual int getPad(double x, double y) const = 0
    virtual int getPad(int dualSampaId, int dualSampaChannel) const = 0
    virtual std::vector<int> getPads(Area area) const = 0;
     */
};

/// Get the segmentation for the given plane of a detection element
std::unique_ptr<SegmentationInterface> getSegmentation(int detElemId, bool isBendingPlane);

/// Return the list of valid detection element ids
std::vector<int> getDetElemIds();

/// Return a vector of detection element id
/// where each detection element corresponds to a different segmentation
/// Mostly needed by tests.
inline std::vector<int> getOneDetElemIdPerSegmentation()
{
  std::vector<int> des;
  std::vector<int> segtypes;
  for (auto deid : getDetElemIds()) {
    for ( auto bending : {true,false}) {
      auto seg = getSegmentation(deid, bending);
      if (std::find(cbegin(segtypes),cend(segtypes),seg->getId())==segtypes.end()) {
        segtypes.push_back(seg->getId());
        des.push_back(deid);
      }
    }
  }
  return des;
}

} // namespace mapping
} // namespace mch
} // namespace o2

#endif