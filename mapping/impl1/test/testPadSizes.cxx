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
#include "padSize.h"
#include <iostream>
#include <array>

std::array<float, 18> padsizex{
  0.63, 0.84, 0.75, 1.26, 0.63, 1.5, 0.75, 2.52, 0.63, 2.5, 0.75, 3, 0.714286, 0.63, 5, 0.714286, 10, 0.714286
};
std::array<float, 18> padsizey{
  0.42, 0.42, 0.5, 0.42, 0.84, 0.5, 1, 0.42, 1.68, 0.5, 2, 0.5, 2.5, 3.36, 0.5, 5, 0.5, 10
};

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(padsize)

BOOST_AUTO_TEST_CASE(ExpectedPadSizeX)
{
  for (auto i = 0; i < padsizex.size(); ++i) {
    BOOST_CHECK_CLOSE(o2::mch::mapping::impl1::padSizeX(i), padsizex[i], 0.01);
  }
}

BOOST_AUTO_TEST_CASE(ExpectedPadSizeY)
{
  for (auto i = 0; i < padsizey.size(); ++i) {
    BOOST_CHECK_CLOSE(o2::mch::mapping::impl1::padSizeY(i), padsizey[i], 0.01);
  }
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

