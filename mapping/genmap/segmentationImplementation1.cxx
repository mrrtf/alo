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

#include "genmap_export.h"
#include "segmentationCInterface.h"
#include "segmentationInterface.h"
#include "segmentationFactory.h"
#include <string>
#include <iostream>
#include <memory>

using o2::mch::mapping::SegmentationInterface;
using o2::mch::mapping::getSegmentation;

extern "C" {

struct GENMAP_EXPORT MchSegmentation
{
    MchSegmentation(SegmentationInterface *si) : impl(si)
    {}

    std::unique_ptr<SegmentationInterface> impl;
};

struct GENMAP_EXPORT MchPad
{
    int padIndex;
};

GENMAP_EXPORT
MchSegmentationHandle mchSegmentationConstruct(int detElemId, bool isBendingPlane)
{
  return new MchSegmentation(getSegmentation(detElemId, isBendingPlane).release());
}

GENMAP_EXPORT
void mchSegmentationDestruct(MchSegmentationHandle sh)
{
  delete sh;
}

GENMAP_EXPORT
int mchSegmentationDualSampaId(MchSegmentationHandle segHandle, int dualSampaIndex)
{
  return segHandle->impl->getSampaId(dualSampaIndex);
}

GENMAP_EXPORT
int mchSegmentationNofDualSampas(MchSegmentationHandle segHandle)
{
  return segHandle->impl->nofDualSampas();
}

GENMAP_EXPORT
int mchSegmentationId(MchSegmentationHandle segHandle)
{
  return segHandle->impl->getId();
}

GENMAP_EXPORT
MchPadHandle mchSegmentationPadConstruct()
{
  return new MchPad;
}

GENMAP_EXPORT
void mchSegmentationPadDestruct(MchPadHandle ph)
{
  delete ph;
}

GENMAP_EXPORT
bool mchSegmentationFindPadByPosition(MchSegmentationHandle segHandle, double x, double y,
                                      MchPadHandle *ph)
{
  if (segHandle->impl->hasPadByPosition(x, y)) {
    // todo: should fill up ph here
    return true;
  }
  return false;
}

GENMAP_EXPORT
bool mchSegmentationFindPadByFEE(MchSegmentationHandle segHandle, int dualSampaId, int dualSampaChannel,
                                 MchPadHandle *padHandle)
{
  if (segHandle->impl->hasPadByFEE(dualSampaId,dualSampaChannel)) {
    // todo: should fill up ph here
    return true;
  }
  return false;
}

GENMAP_EXPORT
void mchForEachDetectionElement(MchDetectionElementHandler handler, void *clientData)
{
  for (auto detElemId: o2::mch::mapping::getDetElemIds()) {
    handler(clientData, detElemId);
  }
}

GENMAP_EXPORT
void mchForOneDetectionElementOfEachSegmentationType(MchDetectionElementHandler handler, void* clientData)
{
  for ( auto detElemId : {100,300,500,501,502,503,504,600,601,602,700,701,702,703,704,705,706,902,903,904,905})
  {
    handler(clientData,detElemId);
  }
}

GENMAP_EXPORT
void mchForEachPadInDualSampa(MchSegmentationHandle segHandle, int dualSampaId, MchPadHandler handler, void *clientData)
{
  auto pads = segHandle->impl->getPads(dualSampaId);
  for (auto p: pads) {
    MchPad pad{p};
    handler(clientData, &pad);
  }
}

GENMAP_EXPORT
double mchSegmentationPadPositionX(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadPosition(padHandle->padIndex, x, y);
  return x;
}

GENMAP_EXPORT
double mchSegmentationPadPositionY(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadPosition(padHandle->padIndex, x, y);
  return y;
}

GENMAP_EXPORT
double mchSegmentationPadSizeX(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadDimension(padHandle->padIndex, x, y);
  return x;
}

GENMAP_EXPORT
double mchSegmentationPadSizeY(MchSegmentationHandle segHandle, MchPadHandle padHandle)
{
  double x, y;
  segHandle->impl->getPadDimension(padHandle->padIndex, x, y);
  return y;
}

} // extern "C"