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

#include "segmentation.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include "boost/format.hpp"
#include <iostream>

using namespace o2::mch::mapping;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(segmentation)

BOOST_AUTO_TEST_CASE(NumberOfDetectionElementsIs156)
{
  std::vector<int> des;
  forEachDetectionElement([&des](int detElemId) { des.push_back(detElemId); });
  BOOST_CHECK_EQUAL(des.size(), 156);
}

BOOST_AUTO_TEST_CASE(GetSegmentationMustNotThrowIfDetElemIdIsValid)
{
  forOneDetectionElementOfEachSegmentationType([](int detElemId) {
    BOOST_CHECK_NO_THROW(Segmentation(detElemId, true));
    BOOST_CHECK_NO_THROW(Segmentation(detElemId, false));
  });
}

BOOST_AUTO_TEST_CASE(GetSegmentationThrowsIfDetElemIdIsNotValid)
{
  BOOST_CHECK_THROW(Segmentation(-1, true), std::runtime_error);
  BOOST_CHECK_THROW(Segmentation(121, true), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(NofBendingPads)
{
  // we explicitly don't make a loop
  // we prefer this solution to more clearly document the number of pads per DE-type
  // sorted by number of pads.

  BOOST_CHECK_EQUAL(Segmentation(100, true).nofPads(), 14392);
  BOOST_CHECK_EQUAL(Segmentation(300, true).nofPads(), 13947);
  BOOST_CHECK_EQUAL(Segmentation(902, true).nofPads(), 4480);
  BOOST_CHECK_EQUAL(Segmentation(702, true).nofPads(), 4160);
  BOOST_CHECK_EQUAL(Segmentation(701, true).nofPads(), 4096);
  BOOST_CHECK_EQUAL(Segmentation(601, true).nofPads(), 3648);
  BOOST_CHECK_EQUAL(Segmentation(501, true).nofPads(), 3568);
  BOOST_CHECK_EQUAL(Segmentation(602, true).nofPads(), 3200);
  BOOST_CHECK_EQUAL(Segmentation(700, true).nofPads(), 3200);
  BOOST_CHECK_EQUAL(Segmentation(502, true).nofPads(), 3120);
  BOOST_CHECK_EQUAL(Segmentation(600, true).nofPads(), 3008);
  BOOST_CHECK_EQUAL(Segmentation(500, true).nofPads(), 2928);
  BOOST_CHECK_EQUAL(Segmentation(903, true).nofPads(), 2880);
  BOOST_CHECK_EQUAL(Segmentation(703, true).nofPads(), 2560);
  BOOST_CHECK_EQUAL(Segmentation(904, true).nofPads(), 2240);
  BOOST_CHECK_EQUAL(Segmentation(503, true).nofPads(), 1920);
  BOOST_CHECK_EQUAL(Segmentation(704, true).nofPads(), 1920);
  BOOST_CHECK_EQUAL(Segmentation(504, true).nofPads(), 1280);
  BOOST_CHECK_EQUAL(Segmentation(905, true).nofPads(), 1280);
  BOOST_CHECK_EQUAL(Segmentation(705, true).nofPads(), 960);
  BOOST_CHECK_EQUAL(Segmentation(706, true).nofPads(), 640);
}

BOOST_AUTO_TEST_CASE(NofNonBendingPads)
{
  BOOST_CHECK_EQUAL(Segmentation(100, false).nofPads(), 14280);
  BOOST_CHECK_EQUAL(Segmentation(300, false).nofPads(), 13986);
  BOOST_CHECK_EQUAL(Segmentation(902, false).nofPads(), 3136);
  BOOST_CHECK_EQUAL(Segmentation(702, false).nofPads(), 2912);
  BOOST_CHECK_EQUAL(Segmentation(701, false).nofPads(), 2880);
  BOOST_CHECK_EQUAL(Segmentation(601, false).nofPads(), 2560);
  BOOST_CHECK_EQUAL(Segmentation(501, false).nofPads(), 2496);
  BOOST_CHECK_EQUAL(Segmentation(602, false).nofPads(), 2240);
  BOOST_CHECK_EQUAL(Segmentation(700, false).nofPads(), 2240);
  BOOST_CHECK_EQUAL(Segmentation(502, false).nofPads(), 2176);
  BOOST_CHECK_EQUAL(Segmentation(600, false).nofPads(), 2112);
  BOOST_CHECK_EQUAL(Segmentation(500, false).nofPads(), 2048);
  BOOST_CHECK_EQUAL(Segmentation(903, false).nofPads(), 2016);
  BOOST_CHECK_EQUAL(Segmentation(703, false).nofPads(), 1792);
  BOOST_CHECK_EQUAL(Segmentation(904, false).nofPads(), 1568);
  BOOST_CHECK_EQUAL(Segmentation(503, false).nofPads(), 1344);
  BOOST_CHECK_EQUAL(Segmentation(704, false).nofPads(), 1344);
  BOOST_CHECK_EQUAL(Segmentation(504, false).nofPads(), 896);
  BOOST_CHECK_EQUAL(Segmentation(905, false).nofPads(), 896);
  BOOST_CHECK_EQUAL(Segmentation(705, false).nofPads(), 672);
  BOOST_CHECK_EQUAL(Segmentation(706, false).nofPads(), 448);
}

BOOST_AUTO_TEST_CASE(TotalNofBendingFECInSegTypes)
{
  int nb{0};
  int nnb{0};
  forOneDetectionElementOfEachSegmentationType([&](int detElemId) {
    nb += Segmentation(detElemId, true).nofDualSampas();
    nnb += Segmentation(detElemId, false).nofDualSampas();
  });
  BOOST_CHECK_EQUAL(nb, 1246);
  BOOST_CHECK_EQUAL(nnb, 1019);
}

BOOST_AUTO_TEST_CASE(NofBendingFEC)
{
  BOOST_CHECK_EQUAL(Segmentation(100, true).nofDualSampas(), 226);
  BOOST_CHECK_EQUAL(Segmentation(300, true).nofDualSampas(), 221);
  BOOST_CHECK_EQUAL(Segmentation(902, true).nofDualSampas(), 70);
  BOOST_CHECK_EQUAL(Segmentation(702, true).nofDualSampas(), 65);
  BOOST_CHECK_EQUAL(Segmentation(701, true).nofDualSampas(), 64);
  BOOST_CHECK_EQUAL(Segmentation(601, true).nofDualSampas(), 57);
  BOOST_CHECK_EQUAL(Segmentation(501, true).nofDualSampas(), 56);
  BOOST_CHECK_EQUAL(Segmentation(602, true).nofDualSampas(), 50);
  BOOST_CHECK_EQUAL(Segmentation(700, true).nofDualSampas(), 50);
  BOOST_CHECK_EQUAL(Segmentation(502, true).nofDualSampas(), 49);
  BOOST_CHECK_EQUAL(Segmentation(600, true).nofDualSampas(), 47);
  BOOST_CHECK_EQUAL(Segmentation(500, true).nofDualSampas(), 46);
  BOOST_CHECK_EQUAL(Segmentation(903, true).nofDualSampas(), 45);
  BOOST_CHECK_EQUAL(Segmentation(703, true).nofDualSampas(), 40);
  BOOST_CHECK_EQUAL(Segmentation(904, true).nofDualSampas(), 35);
  BOOST_CHECK_EQUAL(Segmentation(503, true).nofDualSampas(), 30);
  BOOST_CHECK_EQUAL(Segmentation(704, true).nofDualSampas(), 30);
  BOOST_CHECK_EQUAL(Segmentation(504, true).nofDualSampas(), 20);
  BOOST_CHECK_EQUAL(Segmentation(905, true).nofDualSampas(), 20);
  BOOST_CHECK_EQUAL(Segmentation(705, true).nofDualSampas(), 15);
  BOOST_CHECK_EQUAL(Segmentation(706, true).nofDualSampas(), 10);
}

BOOST_AUTO_TEST_CASE(NofNonBendingFEC)
{
  BOOST_CHECK_EQUAL(Segmentation(100, false).nofDualSampas(), 225);
  BOOST_CHECK_EQUAL(Segmentation(300, false).nofDualSampas(), 222);
  BOOST_CHECK_EQUAL(Segmentation(902, false).nofDualSampas(), 50);
  BOOST_CHECK_EQUAL(Segmentation(701, false).nofDualSampas(), 46);
  BOOST_CHECK_EQUAL(Segmentation(702, false).nofDualSampas(), 46);
  BOOST_CHECK_EQUAL(Segmentation(601, false).nofDualSampas(), 40);
  BOOST_CHECK_EQUAL(Segmentation(501, false).nofDualSampas(), 39);
  BOOST_CHECK_EQUAL(Segmentation(700, false).nofDualSampas(), 36);
  BOOST_CHECK_EQUAL(Segmentation(602, false).nofDualSampas(), 35);
  BOOST_CHECK_EQUAL(Segmentation(502, false).nofDualSampas(), 34);
  BOOST_CHECK_EQUAL(Segmentation(600, false).nofDualSampas(), 33);
  BOOST_CHECK_EQUAL(Segmentation(903, false).nofDualSampas(), 33);
  BOOST_CHECK_EQUAL(Segmentation(500, false).nofDualSampas(), 32);
  BOOST_CHECK_EQUAL(Segmentation(703, false).nofDualSampas(), 29);
  BOOST_CHECK_EQUAL(Segmentation(904, false).nofDualSampas(), 26);
  BOOST_CHECK_EQUAL(Segmentation(704, false).nofDualSampas(), 22);
  BOOST_CHECK_EQUAL(Segmentation(503, false).nofDualSampas(), 21);
  BOOST_CHECK_EQUAL(Segmentation(905, false).nofDualSampas(), 16);
  BOOST_CHECK_EQUAL(Segmentation(504, false).nofDualSampas(), 14);
  BOOST_CHECK_EQUAL(Segmentation(705, false).nofDualSampas(), 12);
  BOOST_CHECK_EQUAL(Segmentation(706, false).nofDualSampas(), 8);
}

BOOST_AUTO_TEST_CASE(CountPadsInSegmentations)
{
  int n{0};
  forOneDetectionElementOfEachSegmentationType([&n](int detElemId) {
    for (auto plane : {true, false}) {
      Segmentation seg{detElemId, plane};
      n += seg.nofPads();
    }
  });
  BOOST_CHECK_EQUAL(n, 143469);
}

BOOST_AUTO_TEST_CASE(LoopOnSegmentations)
{
  int n{0};
  forOneDetectionElementOfEachSegmentationType([&n](int detElemId) {
    n += 2; // two planes (bending, non-bending)
  });
  BOOST_CHECK_EQUAL(n, 42);
}

BOOST_AUTO_TEST_CASE(DualSampasWithLessThan64Pads)
{
  std::map<int, int> non64;
  forOneDetectionElementOfEachSegmentationType([&non64](int detElemId) {
    for (auto plane : {true, false}) {
      Segmentation seg{detElemId, plane};
      for (int i = 0; i < seg.nofDualSampas(); ++i) {
        int n{0};
        seg.forEachPadInDualSampa(seg.dualSampaId(i), [&n](PadHandle /*ph*/) {
          ++n;
        });
        if (n != 64) {
          non64[n]++;
        }
      }
    }
  });

  BOOST_CHECK_EQUAL(non64[31], 1);
  BOOST_CHECK_EQUAL(non64[32], 2);
  BOOST_CHECK_EQUAL(non64[39], 1);
  BOOST_CHECK_EQUAL(non64[40], 3);
  BOOST_CHECK_EQUAL(non64[46], 2);
  BOOST_CHECK_EQUAL(non64[48], 10);
  BOOST_CHECK_EQUAL(non64[49], 1);
  BOOST_CHECK_EQUAL(non64[50], 1);
  BOOST_CHECK_EQUAL(non64[52], 3);
  BOOST_CHECK_EQUAL(non64[54], 2);
  BOOST_CHECK_EQUAL(non64[55], 3);
  BOOST_CHECK_EQUAL(non64[56], 114);
  BOOST_CHECK_EQUAL(non64[57], 3);
  BOOST_CHECK_EQUAL(non64[58], 2);
  BOOST_CHECK_EQUAL(non64[59], 1);
  BOOST_CHECK_EQUAL(non64[60], 6);
  BOOST_CHECK_EQUAL(non64[62], 4);
  BOOST_CHECK_EQUAL(non64[63], 7);

  int n{0};
  for (auto p: non64) {
    n += p.second;
  }

  BOOST_CHECK_EQUAL(n, 166);
}

struct SEG
{
Segmentation seg{100,true};
};

BOOST_FIXTURE_TEST_SUITE(HasPadBy, SEG)

BOOST_AUTO_TEST_CASE(ThrowsIfDualSampaChannelIsNotBetween0And63)
{
  BOOST_CHECK_THROW(seg.findPadByFEE(102, -1), std::out_of_range);
  BOOST_CHECK_THROW(seg.findPadByFEE(102, 64), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(ReturnsTrueIfPadIsConnected)
{
  BOOST_CHECK_EQUAL(seg.findPadByFEE(102, 3), true);
}

BOOST_AUTO_TEST_CASE(ReturnsFalseIfPadIsNotConnected)
{
  BOOST_CHECK_EQUAL(seg.findPadByFEE(214, 14), false);
}


BOOST_AUTO_TEST_CASE(HasPadByPosition)
{
  BOOST_CHECK_EQUAL(seg.findPadByPosition(40.0, 30.0), true);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
