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

std::array<float,18> padsizex{0.6300,0.6300,0.6300,0.6300,0.7143,0.7143,0.7143,0.75,0.75,0.75,0.84,1.26,1.5,2.5,2.52,3.0,5.0,10.0};

std::array<float,18> padsizey{0.42,0.84,1.68,3.36,2.5,5.0,10.0,0.5,1.0,2.0,0.42,0.42,0.5,0.5,0.42,0.5,0.5,0.5};

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(padsize)

BOOST_AUTO_TEST_CASE(NumberOfPadSizes) {
  BOOST_TEST_CHECK(arrayOfPadSizes.size()==18);
}

BOOST_AUTO_TEST_CASE(ExpectedPadSizeX) {
  auto ps = arrayOfPadSizes;
  for (auto i = 0; i < ps.size(); ++i ) {
    BOOST_CHECK_CLOSE(ps[i].first,padsizex[i],0.01);
  }
}

BOOST_AUTO_TEST_CASE(ExpectedPadSizeY) {
  auto ps = arrayOfPadSizes;
  for (auto i = 0; i < ps.size(); ++i ) {
    BOOST_CHECK_CLOSE(ps[i].second,padsizey[i],0.01);
  }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

