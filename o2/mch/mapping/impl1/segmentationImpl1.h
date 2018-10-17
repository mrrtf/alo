#ifndef O2_MCH_MAPPING_IMPL1_SEGMENTATIONIMPL0_H
#define O2_MCH_MAPPING_IMPL1_SEGMENTATIONIMPL0_H

#include "contourCreator.h"
#include "motifPosition.h"
#include "motifPositionContours.h"
#include "motifType.h"
#include "pad.h"
#include "zone.h"
#include <algorithm>
#include <array>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl1 {

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

    virtual void getPadPosition(int ph, double &x, double &y) const = 0;

    virtual void getPadDimension(int ph, double &dx, double &dy) const = 0;

    virtual void getPad(int ph, int& dualSampaId, int& dualSampaChannel) const = 0;

    virtual int findPadByFEE(int dualSampaId, int dualSampaChannel) const = 0;
};

/// Get the segmentation for the given plane of a detection element
std::unique_ptr<SegmentationInterface> getSegmentation(int detElemId, bool isBendingPlane);

/// Return the list of valid detection element ids
std::vector<int> getDetElemIds();

/// Return a vector of detection element id
/// where each detection element corresponds to a different segmentation
/// Mostly needed by tests.
std::vector<int> getOneDetElemIdPerSegmentation();

template<int SEGID, bool BENDINGPLANE>
class Segmentation : public SegmentationInterface
{
  public:
    using MOTIFPOSITION = typename MotifPositionTrait<SEGID, BENDINGPLANE>::type;
    static const int NFEC{MotifPositionTrait<SEGID, BENDINGPLANE>::value};

    int (*berg2channel)(int) = MotifPositionTrait<SEGID, BENDINGPLANE>::func;

    Segmentation(const MotifTypeArray &motifTypes) : mId(SEGID),
                                                          mIsBendingPlane(BENDINGPLANE),
                                                          mNofPads{0},
                                                          mMotifPositions(getMotifPositions<SEGID, BENDINGPLANE>())
    {
      populatePads(motifTypes);
      createContours(motifTypes);
    }

    int getId() const override
    { return mId; }

    bool isBendingPlane() const override
    { return mIsBendingPlane; }

    int nofDualSampas() const override
    { return NFEC; }

    int nofPads() const override
    {
      return mNofPads;
    }

    int getSampaId(int dualSampaIndex) const override
    {
      if (dualSampaIndex > mMotifPositions.size()) {
        throw std::out_of_range("dualSampaIndex should be between 0 and " + std::to_string(mMotifPositions.size()));
      }
      return mMotifPositions[dualSampaIndex].FECId();
    }

    int getSampaIndex(int dualSampaId) const
    {
      auto it = std::find_if(begin(mMotifPositions), end(mMotifPositions),
                             [&](const MOTIFPOSITION &mp) { return mp.FECId() == dualSampaId; });
      if (it == mMotifPositions.end()) {
        throw std::out_of_range("dualSampaId" + std::to_string(dualSampaId) + " is not part of this segmentation");
      }
      return std::distance(mMotifPositions.begin(), it);
    }

    bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const override
    {
      return findPadByFEE(dualSampaId,dualSampaChannel) != -1;
    }

    int findPadByFEE(int dualSampaId, int dualSampaChannel) const override
    {
      if (dualSampaChannel < 0 || dualSampaChannel > 63) {
        throw std::out_of_range("dualSampaChannel should be between 0 and 63");
      }
      int index = getSampaIndex(dualSampaId);
      int padIndex = index * 64 + dualSampaChannel;
      return mPads[padIndex].isValid() ? padIndex : -1;
    }

    bool hasPadByPosition(double x, double y) const override
    {
//      for ( const auto& c: mFEContours) {
//        if (c.contains(x,y)) {
//          return true;
//        }
//      }
      for (const auto &z : mZones) {
        if (z.contains(x, y)) {
          return true;
        }
      }
      return false;
    }

    void getPadPosition(int ph, double &x, double &y) const override
    {
      if (ph < mPads.size()) {
        x = mPads[ph].positionX();
        y = mPads[ph].positionY();
      } else {
        throw std::range_error("incorrect pad handle");
      }
    }

    void getPadDimension(int ph, double &dx, double &dy) const override
    {
      if (ph < mPads.size()) {
        dx = mPads[ph].dimensionX();
        dy = mPads[ph].dimensionY();
      } else {
        throw std::range_error("incorrect pad handle");
      }
    }

    std::vector<int> getPads(int dualSampaId) const override
    {
      int index = getSampaIndex(dualSampaId);
      std::vector<int> pads;
      for (int i = index * 64; i < (index + 1) * 64; ++i) {
        if (mPads[i].isValid()) {
          pads.push_back(i);
        }
      }
      return pads;
    }

    void getPad(int ph, int& dualSampaId, int& dualSampaChannel) const override {
      //int padIndex = index * 64 + fecChannel;
      int index = ph/64;
      dualSampaId = mMotifPositions[index].FECId();
      dualSampaChannel = ph - index*64;
    }

  private:

    void populatePadsForOneMotifPosition(int index, const MOTIFPOSITION &mp, const MotifTypeArray &motifTypes)
    {
      const MotifType &mt = motifTypes[mp.motifTypeId()];
      auto pads = o2::mch::mapping::impl1::getPads(mp, mt);
      for (auto i = 0; i < pads.size(); ++i) {
        int fecChannel = (*berg2channel)(mt.getBerg(i));
        int padIndex = index * 64 + fecChannel;
        mPads[padIndex] = pads[i];
      }
    }

    void populatePads(const MotifTypeArray &motifTypes)
    {
      for (int index = 0; index < mMotifPositions.size(); ++index) {
        const MOTIFPOSITION &mp = mMotifPositions[index];
        const MotifType &mt = motifTypes[mp.motifTypeId()];
        populatePadsForOneMotifPosition(index, mp, motifTypes);
        mNofPads += mt.getNofPads();
      }
    }

    void createContours(const MotifTypeArray &motifTypes)
    {
      mFEContours = o2::mch::mapping::impl1::createMotifPositionContours<MOTIFPOSITION, NFEC>(mMotifPositions, motifTypes);
      mZones = o2::mch::mapping::impl1::createZones<MOTIFPOSITION, NFEC>(mMotifPositions, motifTypes);
    }

    int mId;
    bool mIsBendingPlane;
    int mNofPads;
    std::array<Pad, NFEC * 64> mPads;
    typename MotifPositionTrait<SEGID, BENDINGPLANE>::array_type mMotifPositions;
    std::vector<Zone<MOTIFPOSITION>> mZones;
    std::array<o2::mch::contour::Contour<double>, NFEC> mFEContours;
};

}
}
}
}

#endif
