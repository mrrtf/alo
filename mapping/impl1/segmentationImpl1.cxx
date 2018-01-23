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

#include "impl1_export.h"
#include "segmentationCInterface.h"
#include "segmentationFactory.h"
#include <string>
#include <iostream>
#include <memory>

using o2::mch::mapping::impl1::SegmentationInterface;
using o2::mch::mapping::impl1::getSegmentation;

extern "C" {

struct IMPL1_EXPORT MchSegmentation
{
    MchSegmentation(SegmentationInterface *si) : impl(si)
    {}

    std::unique_ptr<SegmentationInterface> impl;
};

struct IMPL1_EXPORT MchPad
{
    int padIndex;
};

IMPL1_EXPORT
MchSegmentationHandle mchSegmentationConstruct(int detElemId, bool isBendingPlane)
{
  return new MchSegmentation(getSegmentation(detElemId, isBendingPlane).release());
}

IMPL1_EXPORT
void mchSegmentationDestruct(MchSegmentationHandle sh)
{
  delete sh;
}

IMPL1_EXPORT
int mchSegmentationId(MchSegmentationHandle segHandle)
{
  return segHandle->impl->getId();
}

IMPL1_EXPORT
MchPadHandle mchSegmentationPadConstruct()
{
  return new MchPad;
}

IMPL1_EXPORT
void mchSegmentationPadDestruct(MchPadHandle ph)
{
  delete ph;
}

IMPL1_EXPORT
bool mchSegmentationFindPadByPosition(MchSegmentationHandle segHandle, double x, double y,
                                      MchPadHandle *ph)
{
  if (segHandle->impl->hasPadByPosition(x, y)) {
    // todo: should fill up ph here
    return true;
  }
  return false;
}

IMPL1_EXPORT
bool mchSegmentationFindPadByFEE(MchSegmentationHandle segHandle, int dualSampaId, int dualSampaChannel,
                                 MchPadHandle *padHandle)
{
  if (segHandle->impl->hasPadByFEE(dualSampaId,dualSampaChannel)) {
    // todo: should fill up ph here
    return true;
  }
  return false;
}

IMPL1_EXPORT
void mchForEachDetectionElement(MchDetectionElementHandler handler, void *clientData)
{
  for (auto i = 0; i < 156; ++i ) {
    handler(clientData, o2::mch::mapping::impl1::getDetElemIdFromDetElemIndex(i));
  }
}

IMPL1_EXPORT
void mchForEachDualSampa(MchSegmentationHandle segHandle, MchDualSampaHandler handler, void* clientData)
{
  for (auto i = 0; i < segHandle->impl->nofDualSampas(); ++i )
  {
    handler(clientData,segHandle->impl->getSampaId(i));
  }
}

IMPL1_EXPORT
void mchForOneDetectionElementOfEachSegmentationType(MchDetectionElementHandler handler, void* clientData)
{
  for ( auto detElemId : {100,300,500,501,502,503,504,600,601,602,700,701,702,703,704,705,706,902,903,904,905})
  {
    handler(clientData,detElemId);
  }
}

IMPL1_EXPORT
void mchForEachPadInDualSampa(MchSegmentationHandle segHandle, int dualSampaId, MchPadHandler handler, void *clientData)
{
  auto pads = segHandle->impl->getPads(dualSampaId);
  for (auto p: pads) {
    MchPad pad{p};
    handler(clientData, &pad);
  }
}

IMPL1_EXPORT
double mchSegmentationPadPositionX(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadPosition(padHandle->padIndex, x, y);
  return x;
}

IMPL1_EXPORT
double mchSegmentationPadPositionY(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadPosition(padHandle->padIndex, x, y);
  return y;
}

IMPL1_EXPORT
double mchSegmentationPadSizeX(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadDimension(padHandle->padIndex, x, y);
  return x;
}

IMPL1_EXPORT
double mchSegmentationPadSizeY(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadDimension(padHandle->padIndex, x, y);
  return y;
}

} // extern "C"