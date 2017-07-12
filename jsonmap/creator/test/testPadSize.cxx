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
#include "mapping.h"
#include "padsize.h"

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, Mapping)

BOOST_AUTO_TEST_SUITE(padsize)

BOOST_AUTO_TEST_CASE(NumberOfDifferentPadSizes)
{
  auto ps = get_padsizes(ddlStore(),mseg());
  BOOST_TEST_CHECK(ps.size() == 18);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()