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

#include "segmentationImpl2.h"
#include "segmentationContours.h"

using namespace o2::mch::mapping::impl2;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(segmentation_impl2)

BOOST_AUTO_TEST_CASE(IntersectForFullyIncludedBox)
{
  std::unique_ptr<Segmentation> seg{createSegmentation(500, true)};

  std::cout << seg->bbox() << "\n";

  Segmentation::BBox included{-50.0-30.0,-10.0,50.0-30.0,10.0};
  //Segmentation::BBox included{-50.0,-10.0,50.0,10.0};

  std::cout << intersect(seg->bbox(),included) << "\n";
  std::vector<int> pads = seg->intersect(included);

  for (auto p: pads) {
    std::cout << p << " | ";
  }
  std::cout << "\n";
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
