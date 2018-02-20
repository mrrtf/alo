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

#include "MappingFixture.h"

#include "AliMpDDLStore.h"
#include "AliMpDetElement.h"
#include "AliMpSegmentation.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataMap.h"
#include "AliMpDataStreams.h"
#include "AliMpPad.h"

AliMpDDLStore *MAPPING::ddlStore{nullptr};
AliMpSegmentation *MAPPING::mseg{nullptr};

constexpr std::array<int,21> MAPPING::detElemIds;
constexpr std::array<int,2> MAPPING::st12DetElemIds;
constexpr std::array<int,19> MAPPING::st345DetElemIds;

MAPPING::MAPPING()
{
  if (!ddlStore) {
    AliMpDataProcessor mp;
    AliMpDataMap *dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);
    ddlStore = AliMpDDLStore::ReadData(dataStreams);
    mseg = AliMpSegmentation::Instance();
  }
}

const AliMpVSegmentation *alirootSegmentation(AliMpSegmentation *mseg, int detElemId, bool isBendingPlane)
{
  return mseg->GetMpSegmentation(detElemId, AliMpDDLStore::Instance()->GetDetElement(detElemId)->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));
}

bool comparePads(const AliMpPad &alPad, const o2::mch::mapping::Segmentation &o2Seg, int paduid)
{
  if (alPad.IsValid() != o2Seg.isValid(paduid)) {
    return false;
  }

  if (alPad.IsValid()) {
    if (alPad.GetManuId() != o2Seg.padDualSampaId(paduid) ||
        alPad.GetManuChannel() != o2Seg.padDualSampaChannel(paduid)) {
      return false;
    }
  }
  return true;
}

