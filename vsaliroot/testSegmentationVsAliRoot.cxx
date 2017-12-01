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
#include "AliMpDetElement.h"
#include "AliMpManuIterator.h"
#include "AliMpMotif.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include "contour.h"
#include "genDESegmentationFactory.h"
#include "genDetectionElement.h"
#include "genSegmentationFactory.h"
#include "genSegmentationInterface.h"
#include "svgSegmentationInterface.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using namespace o2::mch::mapping;

struct MAPPING
{
    MAPPING()
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

AliMpDDLStore *MAPPING::ddlStore{nullptr};
AliMpSegmentation *MAPPING::mseg{nullptr};

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentation, MAPPING)

void sameHasPadByPosition(const AliMpVSegmentation &alseg, const SegmentationInterface &seg, double x, double y,
                          double step, int ntimes, int &naliroot, int &no2)
{
  // Check whether aliroot and o2 implementations give the same answer to
  // hasPadByPosition for a small box around (x,y)
  //
  // (the test is not done on one point only to avoid edge effects)

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distX(x, x + step);
  std::uniform_real_distribution<double> distY(y, y + step);

  naliroot = no2 = 0;
  int ndiff{0};

  for (int i = 0; i < ntimes; ++i) {
    double xs = distX(mt);
    double ys = distY(mt);
    bool aliroot = alseg.PadByPosition(xs, ys, false).IsValid();
    bool o2 = seg.hasPadByPosition(xs, ys);

    if (aliroot) { ++naliroot; }
    if (o2) { ++no2; }

  }
}

std::pair<const AliMpVSegmentation *, SegmentationInterface *>
getSegmentations(AliMpSegmentation *mseg, int detElemId, int type, bool isBendingPlane)
{
  AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);

  auto al = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

  auto o2 = getSegmentation(type, isBendingPlane);

  return {al, o2.release()};
}

bool checkHasPadByPosition(AliMpSegmentation *mseg, int detElemId, int type, bool isBendingPlane, double step,
                           int ntimes)
{
  auto pair = getSegmentations(mseg, detElemId, type, isBendingPlane);
  auto al = pair.first;
  auto o2 = pair.second;

  auto bbox = getBBox(o2->getEnvelop());

  bool same{true};

  int ndiff{0};

  for (double x = bbox.xmin() + step; x < bbox.xmax() && same; x += step) {
    for (double y = bbox.ymin() + step; y < bbox.ymax() && same; y += step) {
      int naliroot, no2;
      sameHasPadByPosition(*al, *o2, x, y, step, ntimes, naliroot, no2);
      double diff{std::fabs(1.0 * (no2 - naliroot) / ntimes)};
      if (no2 < naliroot || (diff > 0.02 && no2 > ntimes / 2.0 && naliroot > ntimes / 2.0)) {
        same = false;
      }
      if (!same) {
        std::cout << "diff(%)=" << diff * 100.0 << " for x=" << x << " and y=" << y << "\n";
        std::cout << "o2=" << no2 << " and aliroot=" << naliroot << " for x=" << x << " and y=" << y << "\n";
        std::cout << "detElemId=" << detElemId << "\n";
        std::cout << "type=" << type << "\n";
        std::cout << "isBendingPlane=" << isBendingPlane << "\n";
        std::ostringstream filename;
        filename << "bug-" << type << "-" << (isBendingPlane ? "B" : "NB") << "-" << ndiff << ".html";
        ++ndiff;
        auto seg = getSegmentation(type, isBendingPlane);
        o2::mch::svg::writeSegmentationInterface(*seg, filename.str().c_str(), x, y);
      }
    }
  }
  return same;
}

//BOOST_DATA_TEST_CASE(HasPadByPositionIsTheSameForAliRootAndO2, boost::unit_test::data::make(
//  {0, 8, 16, 17, 18, 19, 20, 34, 35, 36, 52, 53, 54, 55, 56, 57, 58, 106, 107, 108, 109}), deIndex)
BOOST_DATA_TEST_CASE(HasPadByPositionIsTheSameForAliRootAndO2, boost::unit_test::data::make(
  {0}), deIndex)
{
  double step{1}; // cm
  int deId = o2::mch::mapping::getDetElemIdFromDetElemIndex(deIndex);
  int segTypeIndex = o2::mch::mapping::getSegTypeIndexFromDetElemIndex(deIndex);
  BOOST_TEST(checkHasPadByPosition(mseg, deId, segTypeIndex, true, step, 100));
  BOOST_TEST(checkHasPadByPosition(mseg, deId, segTypeIndex, false, step, 100));
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
