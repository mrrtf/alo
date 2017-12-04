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


#ifndef ALO_VSALIROOT_COMMONBENCH_H
#define ALO_VSALIROOT_COMMONBENCH_H

#include "benchmark/benchmark.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataMap.h"
#include "AliMpDataStreams.h"
#include "AliMpSegmentation.h"
#include "AliMpDDLStore.h"
#include <iostream>

class BenchAliRoot : public benchmark::Fixture
{
  public:
    BenchAliRoot()
    {
      if (!ddlStore) {
        AliMpDataProcessor mp;
        AliMpDataMap *dataMap = mp.CreateDataMap("data");
        AliMpDataStreams dataStreams(dataMap);
        ddlStore = AliMpDDLStore::ReadData(dataStreams);
        mseg = AliMpSegmentation::Instance();
      }
    }

    static AliMpDDLStore *ddlStore;
    static AliMpSegmentation *mseg;
};

#endif
