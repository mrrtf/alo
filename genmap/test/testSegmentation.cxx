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

#include "Segmentation.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <array>

using namespace o2::mch::mapping;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(segmentation)

std::array<int, 21> bpads = {
  14392, 13947, 2928, 3568, 3120, 1920, 1280, 3008, 3648, 3200, 3200, 4096, 4160, 2560, 1920, 960, 640, 4480, 2880,
  2240, 1280
};

std::array<int, 21> nbpads = {
  14280, 13986, 2048, 2496, 2176, 1344, 896, 2112, 2560, 2240, 2240, 2880, 2912, 1792, 1344, 672, 448, 3136, 2016,
  1568, 896
};

std::array<int,21> bfecs = {};
std::array<int,21> nbfecs = {};

BOOST_AUTO_TEST_CASE(SegmentationIdMustBeBetween0and20) {
  BOOST_CHECK_THROW(Segmentation s(-1,Segmentation::kBendingPlane),std::out_of_range);
  BOOST_CHECK_THROW(Segmentation s(21,Segmentation::kBendingPlane),std::out_of_range);
}

BOOST_TEST_DECORATOR(* boost::unit_test::disabled())
BOOST_DATA_TEST_CASE(NofFEC, bdata::xrange(21), segid ) {
  Segmentation seg(segid,Segmentation::kBendingPlane);
  BOOST_TEST_CHECK(seg.NofDualSampas()==bfecs[segid]);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
