//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche


#ifndef O2_MCH_MAPPING_SEGMENTATION_H
#define O2_MCH_MAPPING_SEGMENTATION_H

#include "segmentationCInterface.h"
#include <stdexcept>
#include <memory>
#include <iostream>

namespace o2 {
namespace mch {
namespace mapping {

using PadHandle = ::MchPadHandle;

class Segmentation
{

  public:
    Segmentation(int detElemId, bool isBendingPlane) : mImpl(nullptr)
    {
      mImpl = mchSegmentationConstruct(detElemId, isBendingPlane);
      if (!mImpl) {
        throw std::runtime_error("Can not create segmentation for DE " + std::to_string(detElemId) + (isBendingPlane ? " Bending":" NonBending"));
      }
    }

    ~Segmentation()
    { mchSegmentationDestruct(mImpl); }

    bool findPadByPosition(double x, double y, PadHandle* padHandle=nullptr) const
    {
      padHandle = nullptr;
      return mchSegmentationFindPadByPosition(mImpl, x, y, padHandle);
    }

    bool findPadByFEE(int dualSampaId, int dualSampaChannel, PadHandle* padHandle=nullptr) const
    {
      padHandle = nullptr;
      return mchSegmentationFindPadByFEE(mImpl, dualSampaId, dualSampaChannel, padHandle);
    }

    int id() const
    { return mchSegmentationId(mImpl); }

    int nofDualSampas() const
    { return mchSegmentationNofDualSampas(mImpl); }

    int dualSampaId(int dualSampaIndex) const
    { return mchSegmentationDualSampaId(mImpl, dualSampaIndex); }

    double padPositionX(PadHandle ph) const
    {
      return mchSegmentationPadPositionX(mImpl, ph);
    }

    double padPositionY(PadHandle ph) const
    {
      return mchSegmentationPadPositionY(mImpl,ph);
    }

    double padSizeX(PadHandle ph) const
    {
      return mchSegmentationPadSizeX(mImpl,ph);
    }

    double padSizeY(PadHandle ph) const
    {
      return mchSegmentationPadSizeY(mImpl,ph);
    }

    int nofPads() const {
      int n{0};
      for ( auto i = 0; i < nofDualSampas(); ++i ) {
        forEachPadInDualSampa(dualSampaId(i),[&n](PadHandle /*ph*/){++n;});
      }
      return n;
    }

    template<typename CALLABLE>
    void forEachPadInDualSampa(int dualSampaId, CALLABLE &&func) const;

  private:
    MchSegmentationHandle mImpl;
};

template<typename CALLABLE>
void Segmentation::forEachPadInDualSampa(int dualSampaId, CALLABLE &&func) const
{
  auto callback = [](void *data, PadHandle ph) {
    auto fn = static_cast<decltype(&func)>(data);
    (*fn)(ph);
  };
  mchForEachPadInDualSampa(mImpl,dualSampaId,callback,&func);
}

template<typename CALLABLE>
void forEachDetectionElement(CALLABLE &&func)
{
  auto callback = [](void *data, int detElemId) {
    auto fn = static_cast<decltype(&func)>(data);
    (*fn)(detElemId);
  };
  mchForEachDetectionElement(callback,&func);
}

template<typename CALLABLE>
void forOneDetectionElementOfEachSegmentationType(CALLABLE &&func)
{
  auto callback = [](void *data, int detElemId) {
    auto fn = static_cast<decltype(&func)>(data);
    (*fn)(detElemId);
  };
  mchForOneDetectionElementOfEachSegmentationType(callback,&func);
}

}
}
}
#endif
