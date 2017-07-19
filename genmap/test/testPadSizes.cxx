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
#include "genPadSize.h"
#include <iostream>
#include <array>

using namespace o2::mch::mapping;

std::array<int,18> padsizex{6300,6300,6300,6300,7143,7143,7143,7500,7500,7500,8400,12600,15000,25000,25200,30000,50000,100000};

std::array<int,18> padsizey{4200,8400,16800,33600,25000,50000,100000,5000,10000,20000,4200,4200,5000,5000,4200,5000,5000,5000};

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(padsize)

BOOST_AUTO_TEST_CASE(NumberOfPadSizes) {
  BOOST_TEST_CHECK(arrayOfPadSizes.size()==18);
}

BOOST_AUTO_TEST_CASE(ExpectedPadSizeX) {
  auto ps = arrayOfPadSizes;
  for (auto i = 0; i < ps.size(); ++i ) {
    BOOST_TEST_CHECK(ps[i].first==padsizex[i]);
  }
}

BOOST_AUTO_TEST_CASE(ExpectedPadSizeY) {
  auto ps = arrayOfPadSizes;
  for (auto i = 0; i < ps.size(); ++i ) {
    BOOST_TEST_CHECK(ps[i].second==padsizey[i]);
  }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

