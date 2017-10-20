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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "AliMpDDLStore.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpManuIterator.h"
#include "AliMpMotif.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <utility>
#include "genSegmentationFactory.h"

using namespace o2::mch::mapping;

struct MAPPING
{
    MAPPING()
    {
      if (!ddlStore) {
        AliMpDataProcessor mp;
        AliMpDataMap* dataMap = mp.CreateDataMap("data");
        AliMpDataStreams dataStreams(dataMap);
        ddlStore = AliMpDDLStore::ReadData(dataStreams);
        mseg = AliMpSegmentation::Instance();
      }
    }

    static AliMpDDLStore* ddlStore;
    static AliMpSegmentation* mseg;
};

AliMpDDLStore* MAPPING::ddlStore{nullptr};
AliMpSegmentation* MAPPING::mseg{nullptr};

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(segmentation)


BOOST_AUTO_TEST_CASE(One)
{
  BOOST_TEST(false);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
