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

#define BOOST_TEST_MODULE o2 mch geometry test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "interval.h"

using namespace o2::mch::geometry;

BOOST_AUTO_TEST_SUITE(o2_mch_geometry)

BOOST_AUTO_TEST_SUITE(interval)

BOOST_AUTO_TEST_CASE(IntervalCtorThrowsIfBeginIsAfterEnd)
{
  BOOST_CHECK_THROW(Interval a(24, 3), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(IntervalCtorThrowsIfBeginEqualsEnd)
{
  BOOST_CHECK_THROW(Interval a(24, 24), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(IntervalIsFullyContainedInInterval)
{
  Interval i{1, 5};
  BOOST_CHECK_EQUAL(Interval(0, 4).isFullyContainedIn(i), false);
  BOOST_CHECK_EQUAL(Interval(1, 2).isFullyContainedIn(i), true);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


