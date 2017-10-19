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
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "bbox.h"

using namespace o2::mch::contour;

BOOST_AUTO_TEST_SUITE(o2_mch_contour)

BOOST_AUTO_TEST_SUITE(bbox)

BOOST_AUTO_TEST_CASE(BBoxMustBeCreatedValid)
{
  BOOST_CHECK_THROW(BBox<int>(2,0,2,0),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(CheckBBoxBoundaries)
{
  BBox<double> test{ -15.0, 5.0, -10.0, 20.0 };
  BOOST_TEST(test.xmin()==-15.0);
  BOOST_TEST(test.xmax()==5.0);
  BOOST_TEST(test.ymin()==-10.0);
  BOOST_TEST(test.ymax()==20.0);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

