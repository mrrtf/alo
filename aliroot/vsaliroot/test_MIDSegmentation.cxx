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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

// #include "AliCDBManager.h"
// #include "AliMpCDB.h"
#include "AliMpDDLStore.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpPad.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include "MIDBase/LegacyUtility.h"
#include "MIDBase/Mapping.h"
#include "MIDBase/MpArea.h"
#include <algorithm>
#include <boost/format.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using namespace o2::mid;

struct MAPPING {
  MAPPING() {
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
  static Mapping o2Segmentation;
};

AliMpDDLStore *MAPPING::ddlStore{nullptr};
AliMpSegmentation *MAPPING::mseg{nullptr};
Mapping MAPPING::o2Segmentation;

BOOST_AUTO_TEST_SUITE(o2_mid_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentation, MAPPING)

bool comparePads(AliMpPad &pad, MpArea &o2Pad, double boardOrX, double stripOrY,
                 int icath, int ich) {
  bool isOk = true;
  std::stringstream info;
  info << "Board (or x) " << boardOrX << "  strip (or y) " << stripOrY
       << "  chamber " << ich << "  cath " << icath;
  if (pad.IsValid() != o2Pad.isValid()) {
    isOk = false;
    info << "  valid: " << pad.IsValid() << " != " << o2Pad.isValid();
  } else if (!pad.IsValid())
    return isOk;

  // Compare pad from aliroot and equivalent from o2 mapping
  std::array<Double_t, 4> val{{pad.GetPositionX(), pad.GetPositionY(),
                               pad.GetDimensionX(), pad.GetDimensionY()}};
  std::array<double, 4> o2Val{{o2Pad.getCenterX(), o2Pad.getCenterY(),
                               o2Pad.getHalfSizeX(), o2Pad.getHalfSizeY()}};
  std::array<std::string, 4> valName{{"posX", "posY", "dimX", "dimY"}};
  for (int ival = 0; ival < 4; ++ival) {
    if (std::abs(val[ival] - o2Val[ival]) > 1.e-3) {
      if (isOk)
        info << " => (AliRoot; O2) => ";
      isOk = false;
      info << " " << valName[ival] << ": (" << val[ival] << " ; " << o2Val[ival]
           << ")";
    }
  } // loop on values
  if (!isOk) {
    for (int ival = 0; ival < 4; ++ival) {
      info << "\n  Aliroot: (" << val[0] - val[2] << ", " << val[1] - val[3]
           << "), (" << val[0] + val[2] << ", " << val[1] + val[3] << ")";
    }
    for (int ival = 0; ival < 4; ++ival) {
      info << "\n       02: (" << o2Val[0] - o2Val[2] << ", "
           << o2Val[1] - o2Val[3] << "), (" << o2Val[0] + o2Val[2] << ", "
           << o2Val[1] + o2Val[3] << ")";
    }
    std::cout << info.str() << std::endl;
  }
  return isOk;
}

bool samePadByLocation() {
  // Check whether aliroot and o2 implementations give the same pad positions

  // AliMpDataProcessor mp;
  // AliMpDataMap *dataMap = mp.CreateDataMap("data");
  // AliMpDataStreams dataStreams(dataMap);
  // AliMpDDLStore* ddlStore = AliMpDDLStore::ReadData(dataStreams);

  bool isOk = true;

  for (int ich = 0; ich < 4; ++ich) {
    for (int iboard = 1; iboard <= 234; ++iboard) {
      int detElemId =
          AliMpDDLStore::Instance()->GetDEfromLocalBoard(iboard, 10 + ich);
      for (int icath = 0; icath < 2; ++icath) {
        const AliMpVSegmentation *seg =
            AliMpSegmentation::Instance()->GetMpSegmentation(
                detElemId, AliMp::GetCathodType(icath));
        for (int istrip = 0; istrip < 16; ++istrip) {
          AliMpPad pad = seg->PadByLocation(iboard, istrip, false);
          if (!pad.IsValid()) {
            continue;
          }
          MpArea o2Pad = MAPPING::o2Segmentation.stripByLocationInBoard(
              istrip, icath, iboard, ich);
          if (!comparePads(pad, o2Pad, iboard, istrip, icath, ich)) {
            isOk = false;
          }
        } // loop on strips
      }   // loop on cathodes
    }     // loop on boards
  }       // loop on chambers
  return isOk;
}

bool samePadByPosition(int cathode, int detElemId, int ntimes) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distX(-127.5, 127.5);
  std::uniform_real_distribution<double> distY(-40., 40.);

  const AliMpVSegmentation *seg =
      AliMpSegmentation::Instance()->GetMpSegmentation(
          detElemId, AliMp::GetCathodType(cathode));

  int deIndex = LegacyUtility::convertFromLegacyDeId(detElemId);

  bool isOk = true;

  for (int itime = 0; itime < ntimes; ++itime) {
    double xs = distX(mt);
    double ys = distY(mt);
    AliMpPad pad = seg->PadByPosition(xs, ys, false);
    Mapping::MpStripIndex stripIndex = MAPPING::o2Segmentation.stripByPosition(
        xs, ys, cathode, deIndex, false);
    MpArea o2Pad = (stripIndex.isValid())
                       ? MAPPING::o2Segmentation.stripByLocation(
                             stripIndex.strip, cathode, stripIndex.line,
                             stripIndex.column, deIndex, false)
                       : MpArea();
    if (!comparePads(pad, o2Pad, xs, ys, cathode, detElemId)) {
      isOk = false;
    }
  }

  return isOk;
}

BOOST_AUTO_TEST_CASE(IsSameByLocation) {
  BOOST_TEST_CHECK(samePadByLocation());
}

std::vector<int> getDetElemIds() {
  std::vector<int> detElemIdList;
  for (int ich = 0; ich < 4; ++ich) {
    for (int irpc = 0; irpc < 18; ++irpc) {
      int detElemId = (11 + ich) * 100 + irpc;
      detElemIdList.push_back(detElemId);
    }
  }
  return detElemIdList;
}

constexpr int NTRIALS{10000};

// BOOST_AUTO_TEST_CASE(IsSameByPosition) {
BOOST_DATA_TEST_CASE(IsSameByPosition,
                     boost::unit_test::data::make(getDetElemIds()), detElemId) {
  BOOST_TEST_CHECK(samePadByPosition(0, detElemId, NTRIALS));
  BOOST_TEST_CHECK(samePadByPosition(1, detElemId, NTRIALS));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
