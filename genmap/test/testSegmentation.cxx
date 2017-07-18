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

#include "genSegmentation.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <array>

using namespace o2::mch::mapping;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(segmentation)

BOOST_AUTO_TEST_CASE(SegmentationIdMustBeBetween0and20)
{
  using WRONG1 = Segmentation<-1, true>;
  using WRONG2 = Segmentation<21, true>;
  using OK = Segmentation<2, true>;
  BOOST_CHECK_THROW(WRONG1 s, std::out_of_range);
  BOOST_CHECK_THROW(WRONG2 s, std::out_of_range);
  BOOST_CHECK_NO_THROW(OK s);
}

template<int N, bool isBendingPlane>
void CheckNofFEC(int b)
{
  Segmentation<N, isBendingPlane> seg;
  BOOST_CHECK_EQUAL(seg.NofDualSampas(), b);
}

template<int N, bool isBendingPlane>
void CheckNofPads(int b)
{
  Segmentation<N, isBendingPlane> seg;
  BOOST_CHECK_EQUAL(seg.NofPads(), b);
}

BOOST_AUTO_TEST_CASE(NofBendingPads)
{
  // we explicitely don't make a loop
  // (that would be possible using some array of SegmentationInterface)
  // as a simpler way to document the number of pads per segmentation
  CheckNofPads<0, true>(14392);
  CheckNofPads<1, true>(13947);
  CheckNofPads<2, true>(2928);
  CheckNofPads<3, true>(3568);
  CheckNofPads<4, true>(3120);
  CheckNofPads<5, true>(1920);
  CheckNofPads<6, true>(1280);
  CheckNofPads<7, true>(3008);
  CheckNofPads<8, true>(3648);
  CheckNofPads<9, true>(3200);
  CheckNofPads<10, true>(3200);
  CheckNofPads<11, true>(4096);
  CheckNofPads<12, true>(4160);
  CheckNofPads<13, true>(2560);
  CheckNofPads<14, true>(1920);
  CheckNofPads<15, true>(960);
  CheckNofPads<16, true>(640);
  CheckNofPads<17, true>(4480);
  CheckNofPads<18, true>(2880);
  CheckNofPads<19, true>(2240);
  CheckNofPads<20, true>(1280);
}

BOOST_AUTO_TEST_CASE(NofNonBendingPads)
{
  CheckNofPads<0, false>(14280);
  CheckNofPads<1, false>(13986);
  CheckNofPads<2, false>(2048);
  CheckNofPads<3, false>(2496);
  CheckNofPads<4, false>(2176);
  CheckNofPads<5, false>(1344);
  CheckNofPads<6, false>(896);
  CheckNofPads<7, false>(2112);
  CheckNofPads<8, false>(2560);
  CheckNofPads<9, false>(2240);
  CheckNofPads<10, false>(2240);
  CheckNofPads<11, false>(2880);
  CheckNofPads<12, false>(2912);
  CheckNofPads<13, false>(1792);
  CheckNofPads<14, false>(1344);
  CheckNofPads<15, false>(672);
  CheckNofPads<16, false>(448);
  CheckNofPads<17, false>(3136);
  CheckNofPads<18, false>(2016);
  CheckNofPads<19, false>(1568);
  CheckNofPads<20, false>(896);
}

BOOST_AUTO_TEST_CASE(NofNonBendingFEC)
{
  CheckNofFEC<0, false>(225);
  CheckNofFEC<1, false>(222);
  CheckNofFEC<2, false>(32);
  CheckNofFEC<3, false>(39);
  CheckNofFEC<4, false>(34);
  CheckNofFEC<5, false>(21);
  CheckNofFEC<6, false>(14);
  CheckNofFEC<7, false>(33);
  CheckNofFEC<8, false>(40);
  CheckNofFEC<9, false>(35);
  CheckNofFEC<10, false>(36);
  CheckNofFEC<11, false>(46);
  CheckNofFEC<12, false>(46);
  CheckNofFEC<13, false>(29);
  CheckNofFEC<14, false>(22);
  CheckNofFEC<15, false>(12);
  CheckNofFEC<16, false>(8);
  CheckNofFEC<17, false>(50);
  CheckNofFEC<18, false>(33);
  CheckNofFEC<19, false>(26);
  CheckNofFEC<20, false>(16);
}

BOOST_AUTO_TEST_CASE(NofBendingFEC)
{
  CheckNofFEC<0, true>(226);
  CheckNofFEC<1, true>(221);
  CheckNofFEC<2, true>(46);
  CheckNofFEC<3, true>(56);
  CheckNofFEC<4, true>(49);
  CheckNofFEC<5, true>(30);
  CheckNofFEC<6, true>(20);
  CheckNofFEC<7, true>(47);
  CheckNofFEC<8, true>(57);
  CheckNofFEC<9, true>(50);
  CheckNofFEC<10, true>(50);
  CheckNofFEC<11, true>(64);
  CheckNofFEC<12, true>(65);
  CheckNofFEC<13, true>(40);
  CheckNofFEC<14, true>(30);
  CheckNofFEC<15, true>(15);
  CheckNofFEC<16, true>(10);
  CheckNofFEC<17, true>(70);
  CheckNofFEC<18, true>(45);
  CheckNofFEC<19, true>(35);
  CheckNofFEC<20, true>(20);
}

BOOST_AUTO_TEST_CASE(MotifTypeIdFitsIntoAUint8)
{
  BOOST_CHECK_EQUAL(gMotifTypeIdMax, 209);
}

BOOST_AUTO_TEST_CASE(PadSizeIdFitsIntoAUint8)
{
  BOOST_CHECK_EQUAL(gPadSizeIdMax, 17);
}

BOOST_AUTO_TEST_CASE(FecIdFitsIntoAShort) {
  BOOST_CHECK_EQUAL(gFecIdMax,1361);
}

/*
BOOST_AUTO_TEST_CASE(FindByPosition) {
  Segmentation<0,true> seg;
  int padid = seg.FindPadIdByPosition(12.0,12.0);
  BOOST_CHECK_EQUAL(padid,2);
}
*/

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
