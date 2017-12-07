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

#include "segmentationInterface.h"
#include <array>

using namespace o2::mch::mapping;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(detection_element)

BOOST_AUTO_TEST_CASE(GetDetElemIdFromDetElemIndexShouldThrowForInvalidIndex)
{
  BOOST_CHECK_THROW(getDetElemIdFromDetElemIndex(-1),std::out_of_range);
  BOOST_CHECK_THROW(getDetElemIdFromDetElemIndex(156),std::out_of_range);
  BOOST_CHECK_NO_THROW(getDetElemIdFromDetElemIndex(15));
}

BOOST_DATA_TEST_CASE(CircularTest,boost::unit_test::data::xrange(0,156),deIndex)
{
  int deId = getDetElemIdFromDetElemIndex(deIndex);
  BOOST_TEST(deIndex==getDetElemIndexFromDetElemId(deId));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()