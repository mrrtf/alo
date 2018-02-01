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


#ifndef O2_MCH_SEGMENTATIONCINTERFACE_H
#define O2_MCH_SEGMENTATIONCINTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct MchSegmentation *MchSegmentationHandle;
typedef void (*MchDetectionElementHandler)(void *clientData, int detElemId);
typedef void (*MchPadHandler)(void *clientData, int paduid);
typedef void (*MchDualSampaHandler)(void *clientData, int dualSampaId);

/// Create a handle to a segmentation for a given plane of a detection element
MchSegmentationHandle mchSegmentationConstruct(int detElemId, bool isBendingPlane);

/// Delete a segmentation handle
void mchSegmentationDestruct(MchSegmentationHandle segHandle);

/// Return > 0 if paduid is a valid one or <= 1 if not
int mchSegmentationIsPadValid(MchSegmentationHandle segHandle, int paduid);

int mchSegmentationFindPadByPosition(MchSegmentationHandle segHandle, double x, double y);

int mchSegmentationFindPadByFEE(MchSegmentationHandle segHandle, int dualSampaId, int dualSampaChannel);

int mchSegmentationId(MchSegmentationHandle segHandle);

void mchForEachDetectionElement(MchDetectionElementHandler handler, void *clientData);

void mchForOneDetectionElementOfEachSegmentationType(MchDetectionElementHandler handler, void *clientData);

void mchForEachDualSampa(MchSegmentationHandle segHandle, MchDualSampaHandler handler, void* clientData);

void mchForEachPadInDualSampa(MchSegmentationHandle segHandle, int dualSampaId, MchPadHandler handler, void *clientData);

double mchSegmentationPadPositionX(MchSegmentationHandle segHandle, int paduid);
double mchSegmentationPadPositionY(MchSegmentationHandle segHandle, int paduid);
double mchSegmentationPadSizeX(MchSegmentationHandle segHandle, int paduid);
double mchSegmentationPadSizeY(MchSegmentationHandle segHandle, int paduid);

void mchSegmentationForEachPadInArea(MchSegmentationHandle segHAndle, double xmin, double ymin, double xmax, double ymax, void* clientData);
int mchSegmentationPadDualSampaId(MchSegmentationHandle segHandle, int paduid);
int mchSegmentationPadDualSampaChannel(MchSegmentationHandle segHandle, int paduid);

#ifdef __cplusplus
};
#endif

#endif
