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
#include "AliMpPad.h"
#include "AliMpSegmentation.h"
#include "AliMpVPadIterator.h"
#include "AliMpVSegmentation.h"
#include "contour.h"
#include "contourCreator.h"
#include "svgContour.h"
#include "segmentationContours.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <utility>
#include <vector>
#include <segmentation.h>

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

    static std::vector<int> detElemIds;
};

AliMpDDLStore *MAPPING::ddlStore{nullptr};
AliMpSegmentation *MAPPING::mseg{nullptr};
std::vector<int> MAPPING::detElemIds
  {
    100, 300, 500, 501, 502, 503, 504, 600, 601, 602, 700, 701, 702, 703, 704, 705, 706, 902, 903, 904, 905
  };

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentationvsaliroot2, MAPPING)

void sameHasPadByPosition(const AliMpVSegmentation &alseg, const Segmentation &seg, double x, double y,
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
    bool o2 = seg.findPadByPosition(xs, ys);

    if (aliroot) { ++naliroot; }
    if (o2) { ++no2; }

  }
}

bool checkHasPadByPosition(AliMpSegmentation *mseg, int detElemId, bool isBendingPlane, double step,
                           int ntimes)
{
  auto al = mseg->GetMpSegmentation(detElemId, AliMpDDLStore::Instance()->GetDetElement(detElemId)->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));
  Segmentation o2seg{detElemId, isBendingPlane};

  auto contours = o2::mch::mapping::getSampaContours(detElemId,isBendingPlane);
  auto bbox = o2::mch::contour::getBBox(o2::mch::contour::getEnvelop(contours));

  bool same{true};

  int ndiff{0};

  for (double x = bbox.xmin() + step; x < bbox.xmax() && same; x += step) {
    for (double y = bbox.ymin() + step; y < bbox.ymax() && same; y += step) {
      int naliroot, no2;
      sameHasPadByPosition(*al, o2seg, x, y, step, ntimes, naliroot, no2);
      double diff{std::fabs(1.0 * (no2 - naliroot) / ntimes)};
      if (no2 < naliroot || (diff > 0.02 && no2 > ntimes / 2.0 && naliroot > ntimes / 2.0)) {
        same = false;
      }
      if (!same) {
        std::cout << "diff(%)=" << diff * 100.0 << " for x=" << x << " and y=" << y << "\n";
        std::cout << "o2=" << no2 << " and aliroot=" << naliroot << " for x=" << x << " and y=" << y << "\n";
        std::cout << "detElemId=" << detElemId << "\n";
        std::cout << "isBendingPlane=" << isBendingPlane << "\n";
        std::ostringstream filename;
        filename << "bug-" << o2seg.id() << "-" << (isBendingPlane ? "B" : "NB") << "-" << ndiff << ".html";
        ++ndiff;
        o2::mch::svg::writeContours(contours, filename.str().c_str(), x, y);
      }
    }
  }
  return same;
}

BOOST_DATA_TEST_CASE(HasPadByPositionIsTheSameForAliRootAndO2, boost::unit_test::data::make(
  {100, 300, 500, 501, 502, 503, 504, 600, 601, 602, 700, 701, 702, 703, 704, 705, 706, 902, 903, 904, 905}), detElemId)
{
  double step{1}; // cm
  BOOST_TEST(checkHasPadByPosition(mseg, detElemId, true, step, 100));
  BOOST_TEST(checkHasPadByPosition(mseg, detElemId, false, step, 100));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
