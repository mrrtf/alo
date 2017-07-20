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

#include "genSegmentationFactory.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <array>
#include "boost/format.hpp"

using namespace o2::mch::mapping;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(segmentation)

BOOST_AUTO_TEST_CASE(SegmentationIdMustBeBetween0and20)
{
  BOOST_CHECK_THROW(getSegmentation(-1,true), std::out_of_range);
  BOOST_CHECK_THROW(getSegmentation(21,true), std::out_of_range);
  BOOST_CHECK_NO_THROW(getSegmentation(2,true) );
}

BOOST_AUTO_TEST_CASE(NofBendingPads)
{
  // we explicitly don't make a loop
  // we prefer this solution to more clearly document the number of pads per segmentation
  BOOST_CHECK_EQUAL(getSegmentation(0, true)->nofPads(),14392);
  BOOST_CHECK_EQUAL(getSegmentation(1, true)->nofPads(),13947);
  BOOST_CHECK_EQUAL(getSegmentation(2, true)->nofPads(),2928);
  BOOST_CHECK_EQUAL(getSegmentation(3, true)->nofPads(),3568);
  BOOST_CHECK_EQUAL(getSegmentation(4, true)->nofPads(),3120);
  BOOST_CHECK_EQUAL(getSegmentation(5, true)->nofPads(),1920);
  BOOST_CHECK_EQUAL(getSegmentation(6, true)->nofPads(),1280);
  BOOST_CHECK_EQUAL(getSegmentation(7, true)->nofPads(),3008);
  BOOST_CHECK_EQUAL(getSegmentation(8, true)->nofPads(),3648);
  BOOST_CHECK_EQUAL(getSegmentation(9, true)->nofPads(),3200);
  BOOST_CHECK_EQUAL(getSegmentation(10, true)->nofPads(),3200);
  BOOST_CHECK_EQUAL(getSegmentation(11, true)->nofPads(),4096);
  BOOST_CHECK_EQUAL(getSegmentation(12, true)->nofPads(),4160);
  BOOST_CHECK_EQUAL(getSegmentation(13, true)->nofPads(),2560);
  BOOST_CHECK_EQUAL(getSegmentation(14, true)->nofPads(),1920);
  BOOST_CHECK_EQUAL(getSegmentation(15, true)->nofPads(),960);
  BOOST_CHECK_EQUAL(getSegmentation(16, true)->nofPads(),640);
  BOOST_CHECK_EQUAL(getSegmentation(17, true)->nofPads(),4480);
  BOOST_CHECK_EQUAL(getSegmentation(18, true)->nofPads(),2880);
  BOOST_CHECK_EQUAL(getSegmentation(19, true)->nofPads(),2240);
  BOOST_CHECK_EQUAL(getSegmentation(20, true)->nofPads(),1280);
}


BOOST_AUTO_TEST_CASE(NofNonBendingPads)
{
  BOOST_CHECK_EQUAL(getSegmentation(0, false)->nofPads(),14280);
  BOOST_CHECK_EQUAL(getSegmentation(1, false)->nofPads(),13986);
  BOOST_CHECK_EQUAL(getSegmentation(2, false)->nofPads(),2048);
  BOOST_CHECK_EQUAL(getSegmentation(3, false)->nofPads(),2496);
  BOOST_CHECK_EQUAL(getSegmentation(4, false)->nofPads(),2176);
  BOOST_CHECK_EQUAL(getSegmentation(5, false)->nofPads(),1344);
  BOOST_CHECK_EQUAL(getSegmentation(6, false)->nofPads(),896);
  BOOST_CHECK_EQUAL(getSegmentation(7, false)->nofPads(),2112);
  BOOST_CHECK_EQUAL(getSegmentation(8, false)->nofPads(),2560);
  BOOST_CHECK_EQUAL(getSegmentation(9, false)->nofPads(),2240);
  BOOST_CHECK_EQUAL(getSegmentation(10, false)->nofPads(),2240);
  BOOST_CHECK_EQUAL(getSegmentation(11, false)->nofPads(),2880);
  BOOST_CHECK_EQUAL(getSegmentation(12, false)->nofPads(),2912);
  BOOST_CHECK_EQUAL(getSegmentation(13, false)->nofPads(),1792);
  BOOST_CHECK_EQUAL(getSegmentation(14, false)->nofPads(),1344);
  BOOST_CHECK_EQUAL(getSegmentation(15, false)->nofPads(),672);
  BOOST_CHECK_EQUAL(getSegmentation(16, false)->nofPads(),448);
  BOOST_CHECK_EQUAL(getSegmentation(17, false)->nofPads(),3136);
  BOOST_CHECK_EQUAL(getSegmentation(18, false)->nofPads(),2016);
  BOOST_CHECK_EQUAL(getSegmentation(19, false)->nofPads(),1568);
  BOOST_CHECK_EQUAL(getSegmentation(20, false)->nofPads(),896);
}

BOOST_AUTO_TEST_CASE(NofNonBendingFEC)
{
  BOOST_CHECK_EQUAL(getSegmentation(0, false)->nofDualSampas(),225);
  BOOST_CHECK_EQUAL(getSegmentation(1, false)->nofDualSampas(),222);
  BOOST_CHECK_EQUAL(getSegmentation(2, false)->nofDualSampas(),32);
  BOOST_CHECK_EQUAL(getSegmentation(3, false)->nofDualSampas(),39);
  BOOST_CHECK_EQUAL(getSegmentation(4, false)->nofDualSampas(),34);
  BOOST_CHECK_EQUAL(getSegmentation(5, false)->nofDualSampas(),21);
  BOOST_CHECK_EQUAL(getSegmentation(6, false)->nofDualSampas(),14);
  BOOST_CHECK_EQUAL(getSegmentation(7, false)->nofDualSampas(),33);
  BOOST_CHECK_EQUAL(getSegmentation(8, false)->nofDualSampas(),40);
  BOOST_CHECK_EQUAL(getSegmentation(9, false)->nofDualSampas(),35);
  BOOST_CHECK_EQUAL(getSegmentation(10, false)->nofDualSampas(),36);
  BOOST_CHECK_EQUAL(getSegmentation(11, false)->nofDualSampas(),46);
  BOOST_CHECK_EQUAL(getSegmentation(12, false)->nofDualSampas(),46);
  BOOST_CHECK_EQUAL(getSegmentation(13, false)->nofDualSampas(),29);
  BOOST_CHECK_EQUAL(getSegmentation(14, false)->nofDualSampas(),22);
  BOOST_CHECK_EQUAL(getSegmentation(15, false)->nofDualSampas(),12);
  BOOST_CHECK_EQUAL(getSegmentation(16, false)->nofDualSampas(),8);
  BOOST_CHECK_EQUAL(getSegmentation(17, false)->nofDualSampas(),50);
  BOOST_CHECK_EQUAL(getSegmentation(18, false)->nofDualSampas(),33);
  BOOST_CHECK_EQUAL(getSegmentation(19, false)->nofDualSampas(),26);
  BOOST_CHECK_EQUAL(getSegmentation(20, false)->nofDualSampas(),16);
}

BOOST_AUTO_TEST_CASE(NofBendingFEC)
{
  BOOST_CHECK_EQUAL(getSegmentation(0, true)->nofDualSampas(),226);
  BOOST_CHECK_EQUAL(getSegmentation(1, true)->nofDualSampas(),221);
  BOOST_CHECK_EQUAL(getSegmentation(2, true)->nofDualSampas(),46);
  BOOST_CHECK_EQUAL(getSegmentation(3, true)->nofDualSampas(),56);
  BOOST_CHECK_EQUAL(getSegmentation(4, true)->nofDualSampas(),49);
  BOOST_CHECK_EQUAL(getSegmentation(5, true)->nofDualSampas(),30);
  BOOST_CHECK_EQUAL(getSegmentation(6, true)->nofDualSampas(),20);
  BOOST_CHECK_EQUAL(getSegmentation(7, true)->nofDualSampas(),47);
  BOOST_CHECK_EQUAL(getSegmentation(8, true)->nofDualSampas(),57);
  BOOST_CHECK_EQUAL(getSegmentation(9, true)->nofDualSampas(),50);
  BOOST_CHECK_EQUAL(getSegmentation(10, true)->nofDualSampas(),50);
  BOOST_CHECK_EQUAL(getSegmentation(11, true)->nofDualSampas(),64);
  BOOST_CHECK_EQUAL(getSegmentation(12, true)->nofDualSampas(),65);
  BOOST_CHECK_EQUAL(getSegmentation(13, true)->nofDualSampas(),40);
  BOOST_CHECK_EQUAL(getSegmentation(14, true)->nofDualSampas(),30);
  BOOST_CHECK_EQUAL(getSegmentation(15, true)->nofDualSampas(),15);
  BOOST_CHECK_EQUAL(getSegmentation(16, true)->nofDualSampas(),10);
  BOOST_CHECK_EQUAL(getSegmentation(17, true)->nofDualSampas(),70);
  BOOST_CHECK_EQUAL(getSegmentation(18, true)->nofDualSampas(),45);
  BOOST_CHECK_EQUAL(getSegmentation(19, true)->nofDualSampas(),35);
  BOOST_CHECK_EQUAL(getSegmentation(20, true)->nofDualSampas(),20);
}

struct SEG {
    std::unique_ptr<SegmentationInterface> seg{getSegmentation(0,true)};
};

BOOST_FIXTURE_TEST_SUITE(HasPadBy,SEG)

BOOST_AUTO_TEST_CASE(ThrowsIfDualSampaChannelIsNotBetween0And63) {
  BOOST_CHECK_THROW(seg->hasPadByFEE(102,-1),std::out_of_range);
  BOOST_CHECK_THROW(seg->hasPadByFEE(102,64),std::out_of_range);
}

BOOST_AUTO_TEST_CASE(ThrowsIfDualSampaIdIsInvalid) {
  BOOST_CHECK_THROW(seg->hasPadByFEE(1000,0),std::out_of_range);
}

BOOST_AUTO_TEST_CASE(ReturnsTrueIfPadIsConnected) {
  BOOST_CHECK_EQUAL(seg->hasPadByFEE(102,3),true);
}

BOOST_AUTO_TEST_CASE(ReturnsFalseIfPadIsNotConnected) {
  BOOST_CHECK_EQUAL(seg->hasPadByFEE(214,14),false);
}
BOOST_AUTO_TEST_SUITE_END()

//BOOST_AUTO_TEST_CASE(HasPadByPosition) {
//  BOOST_CHECK_EQUAL(getSegmentation(0,true)->hasPadByPosition(40.0,30.0),true);
//}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
