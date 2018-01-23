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

using namespace o2::mch::mapping::impl2;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(segmentation_impl2)

BOOST_AUTO_TEST_CASE(FractionOfRectangles)
{
  int npolygons{0};
  int nrectangles{0};
  o2::mch::mapping::forOneDetElemPerSegmentationType([&](int detElemId){

    int np{0};
    int nr{0};
    for (auto isBendingPlane : {true, false}) {
      auto contours = getSampaContours(detElemId, isBendingPlane);
      for (auto &c: contours) {
        for (auto i = 0; i < c.size(); ++i) {
          ++npolygons;
          ++np;
          const auto &p = c[i];
          if (p.size() == 5) {
            ++nrectangles;
            ++nr;
          }
        }
      }
    }

    std::cout << boost::format("DE %4d nr/np = %7.2f%%\n") % detElemId % (nr*100.0/np);
  });
  std::cout << "npolygons=" << npolygons << " nrectangles=" << nrectangles << "\n";
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
