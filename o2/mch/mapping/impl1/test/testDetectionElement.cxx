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

#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <array>
#include <vector>
#include "segmentationFactory.h"

using namespace o2::mch::mapping::impl1;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(detection_element)

BOOST_AUTO_TEST_CASE(GetDetElemIdFromDetElemIndexShouldThrowForInvalidIndex)
{
  BOOST_CHECK_THROW(getDetElemIdFromDetElemIndex(-1), std::runtime_error);
  BOOST_CHECK_THROW(getDetElemIdFromDetElemIndex(156), std::runtime_error);
  BOOST_CHECK_NO_THROW(getDetElemIdFromDetElemIndex(15));
}

BOOST_DATA_TEST_CASE(SegTypeByDetElemIndex, boost::unit_test::data::xrange(0, 156), deIndex)
{
  std::array<int, 156> segtype{
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 3, 4, 5, 6, 6, 5, 4, 3, 7, 8, 9, 5, 6,
    6, 5, 9, 8, 7, 8, 9, 5, 6, 6, 5, 9, 8, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15,
    16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15,
    14, 13, 12, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17,
    11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11
  };
  BOOST_TEST(getSegTypeIndexFromDetElemIndex(deIndex) == segtype[deIndex]);
}

BOOST_DATA_TEST_CASE(CircularTest, boost::unit_test::data::xrange(0, 156), deIndex)
{
  int deId = getDetElemIdFromDetElemIndex(deIndex);
  BOOST_TEST(deIndex == getDetElemIndexFromDetElemId(deId));
}

BOOST_AUTO_TEST_CASE(SegmentationByDetElemIdThrowsIfDEIndexIsIncorrect)
{
  BOOST_CHECK_THROW(getSegmentationByDetElemIndex(-1, true), std::runtime_error);
  BOOST_CHECK_THROW(getSegmentationByDetElemIndex(156, true), std::runtime_error);
  BOOST_CHECK_NO_THROW(getSegmentationByDetElemIndex(16, true));
}

BOOST_AUTO_TEST_CASE(GetOneDetElemIdPerSegmentation)
{

  std::vector<int>
    firstDetElemIdOfEachSegmentationType{
    100, 300, 500, 501, 502, 503, 504, 600, 601, 602, 700, 701,
    702, 703, 704, 705, 706, 902, 903, 904, 905
  };

  auto deids = getOneDetElemIdPerSegmentation();
  BOOST_TEST(deids == firstDetElemIdOfEachSegmentationType);
}

BOOST_AUTO_TEST_CASE(GetSegmentationByTypeThrowsIfTypeIsNotBetween0and20)
{
  BOOST_CHECK_THROW(getSegmentationByType(-1, true), std::out_of_range);
  BOOST_CHECK_THROW(getSegmentationByType(21, true), std::out_of_range);
}

BOOST_DATA_TEST_CASE(GetSegmentationByTypeMustNotThrowIfTypeIsBetween0and20, boost::unit_test::data::xrange(0, 21), segtype)
{
  for (auto bending: {true,false}) {
    BOOST_CHECK_NO_THROW(getSegmentationByType(segtype, bending));
  }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()