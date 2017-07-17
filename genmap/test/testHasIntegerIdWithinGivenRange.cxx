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

#include "HasIntegerIdWithinGivenRange.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>

using namespace o2::mch::mapping;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(hasintegeridwithingivenrange)

using RID = HasIntegerIdWithinGivenRange<0,1>;

BOOST_AUTO_TEST_CASE(ThrowsExceptionWhenHasIntegerIdWithinGivenRangeIdNotValid)
{
  BOOST_CHECK_THROW(RID s(-1), std::out_of_range);
  BOOST_CHECK_THROW(RID s(RID::GetMaxValidId() + 1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(DoesNotThrowExceptionWhenHasIntegerIdWithinGivenRangeIdIsValid)
{
  BOOST_CHECK_NO_THROW(RID s(0));
}

BOOST_DATA_TEST_CASE(ReturnsTrueIfHasIntegerIdWithinGivenRangeIdIsValid, bdata::xrange(1+RID::GetMaxValidId()),segId) {
    BOOST_CHECK(RID::IsValidId(segId));
}

BOOST_AUTO_TEST_CASE(HasIntegerIdWithinGivenRangeRetainsItsId) {
  RID seg(1);
  BOOST_CHECK(seg.GetId()==1);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
