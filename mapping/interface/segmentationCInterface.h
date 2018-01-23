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
typedef struct MchPad *MchPadHandle;
typedef void (*MchDetectionElementHandler)(void *clientData, int detElemId);
typedef void (*MchPadHandler)(void *clientData, MchPadHandle padHandle);
typedef void (*MchDualSampaHandler)(void *clientData, int dualSampaId);

/// Create a handle to a segmentation for a given plane of a detection element
MchSegmentationHandle mchSegmentationConstruct(int detElemId, bool isBendingPlane);

/// Delete a segmentation handle
void mchSegmentationDestruct(MchSegmentationHandle segHandle);

/// Create a pad handle
MchPadHandle mchSegmentationPadConstruct();

/// Delete a pad handle
void mchSegmentationPadDestruct(MchPadHandle padHandle);

bool mchSegmentationFindPadByPosition(MchSegmentationHandle segHandle, double x, double y,
                                      MchPadHandle *padHandle);

bool mchSegmentationFindPadByFEE(MchSegmentationHandle segHandle, int dualSampaId, int dualSampaChannel,
                                 MchPadHandle *padHandle);

int mchSegmentationId(MchSegmentationHandle segHandle);

void mchForEachDetectionElement(MchDetectionElementHandler handler, void *clientData);

void mchForOneDetectionElementOfEachSegmentationType(MchDetectionElementHandler handler, void *clientData);

void mchForEachDualSampa(MchSegmentationHandle segHandle, MchDualSampaHandler handler, void* clientData);

void mchForEachPadInDualSampa(MchSegmentationHandle segHandle, int dualSampaId, MchPadHandler handler, void *clientData);

double mchSegmentationPadPositionX(MchSegmentationHandle segHandle, MchPadHandle padHandle);
double mchSegmentationPadPositionY(MchSegmentationHandle segHandle, MchPadHandle padHandle);
double mchSegmentationPadSizeX(MchSegmentationHandle segHandle, MchPadHandle padHandle);
double mchSegmentationPadSizeY(MchSegmentationHandle segHandle, MchPadHandle padHandle);
/*
int mchPadDualSampaId(MchPadHandle ph, MchErrorHandle *error);

int mchPadDualSampaChannel(MchPadHandle ph, MchErrorHandle *error);

 */

#ifdef __cplusplus
};
#endif

#endif
