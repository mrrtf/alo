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
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "segmentation.h"

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(cstyle_segmentation)

BOOST_AUTO_TEST_CASE(CreateSegmentationWithInvalidVersionMustThrow)
{
  BOOST_CHECK_THROW(o2::mch::mapping::Segmentation seg(100,true),std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
