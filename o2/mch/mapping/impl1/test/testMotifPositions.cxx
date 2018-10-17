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
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include "boost/format.hpp"
#include <iostream>
#include "motifPosition.h"
#include "contour.h"
#include "motifPositionContours.h"

using namespace o2::mch::mapping::impl1;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(motif_positions)

BOOST_AUTO_TEST_CASE(BergFunctionFromMotifPositionTrait)
{
  BOOST_CHECK_EQUAL((MotifPositionTrait<0, false>::func) (53), 43);
  BOOST_CHECK_EQUAL((MotifPositionTrait<16, true>::func) (53), 49);
}

BOOST_AUTO_TEST_CASE(NofNonBendingFEC)
{
  BOOST_CHECK_EQUAL((getMotifPositions<0, false>().size()), 225);
  BOOST_CHECK_EQUAL((getMotifPositions<1, false>().size()), 222);
  BOOST_CHECK_EQUAL((getMotifPositions<2, false>().size()), 32);
  BOOST_CHECK_EQUAL((getMotifPositions<3, false>().size()), 39);
  BOOST_CHECK_EQUAL((getMotifPositions<4, false>().size()), 34);
  BOOST_CHECK_EQUAL((getMotifPositions<5, false>().size()), 21);
  BOOST_CHECK_EQUAL((getMotifPositions<6, false>().size()), 14);
  BOOST_CHECK_EQUAL((getMotifPositions<7, false>().size()), 33);
  BOOST_CHECK_EQUAL((getMotifPositions<8, false>().size()), 40);
  BOOST_CHECK_EQUAL((getMotifPositions<9, false>().size()), 35);
  BOOST_CHECK_EQUAL((getMotifPositions<10, false>().size()), 36);
  BOOST_CHECK_EQUAL((getMotifPositions<11, false>().size()), 46);
  BOOST_CHECK_EQUAL((getMotifPositions<12, false>().size()), 46);
  BOOST_CHECK_EQUAL((getMotifPositions<13, false>().size()), 29);
  BOOST_CHECK_EQUAL((getMotifPositions<14, false>().size()), 22);
  BOOST_CHECK_EQUAL((getMotifPositions<15, false>().size()), 12);
  BOOST_CHECK_EQUAL((getMotifPositions<16, false>().size()), 8);
  BOOST_CHECK_EQUAL((getMotifPositions<17, false>().size()), 50);
  BOOST_CHECK_EQUAL((getMotifPositions<18, false>().size()), 33);
  BOOST_CHECK_EQUAL((getMotifPositions<19, false>().size()), 26);
  BOOST_CHECK_EQUAL((getMotifPositions<20, false>().size()), 16);
}

BOOST_AUTO_TEST_CASE(NofBendingFEC)
{
  BOOST_CHECK_EQUAL((getMotifPositions<0, true>().size()), 226);
  BOOST_CHECK_EQUAL((getMotifPositions<1, true>().size()), 221);
  BOOST_CHECK_EQUAL((getMotifPositions<2, true>().size()), 46);
  BOOST_CHECK_EQUAL((getMotifPositions<3, true>().size()), 56);
  BOOST_CHECK_EQUAL((getMotifPositions<4, true>().size()), 49);
  BOOST_CHECK_EQUAL((getMotifPositions<5, true>().size()), 30);
  BOOST_CHECK_EQUAL((getMotifPositions<6, true>().size()), 20);
  BOOST_CHECK_EQUAL((getMotifPositions<7, true>().size()), 47);
  BOOST_CHECK_EQUAL((getMotifPositions<8, true>().size()), 57);
  BOOST_CHECK_EQUAL((getMotifPositions<9, true>().size()), 50);
  BOOST_CHECK_EQUAL((getMotifPositions<10, true>().size()), 50);
  BOOST_CHECK_EQUAL((getMotifPositions<11, true>().size()), 64);
  BOOST_CHECK_EQUAL((getMotifPositions<12, true>().size()), 65);
  BOOST_CHECK_EQUAL((getMotifPositions<13, true>().size()), 40);
  BOOST_CHECK_EQUAL((getMotifPositions<14, true>().size()), 30);
  BOOST_CHECK_EQUAL((getMotifPositions<15, true>().size()), 15);
  BOOST_CHECK_EQUAL((getMotifPositions<16, true>().size()), 10);
  BOOST_CHECK_EQUAL((getMotifPositions<17, true>().size()), 70);
  BOOST_CHECK_EQUAL((getMotifPositions<18, true>().size()), 45);
  BOOST_CHECK_EQUAL((getMotifPositions<19, true>().size()), 35);
  BOOST_CHECK_EQUAL((getMotifPositions<20, true>().size()), 20);
}

template<int SEGTYPE, bool BENDING>
int numberOfMps()
{
  int n{0};
  auto mps = getMotifPositions<SEGTYPE, BENDING>();
  for (auto mp :mps) {
    auto contour = createMotifPositionContour(mp, arrayOfMotifTypes[mp.motifTypeId()]);
    if (contour.size() > 1) {
      ++n;
    }
  }
  return n;
};

BOOST_AUTO_TEST_CASE(NofBendingMotifPositionsWhereContourIsMoreThanOnePolygon)
{
  BOOST_CHECK_EQUAL((numberOfMps<0, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<1, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<2, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<3, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<4, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<5, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<6, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<7, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<8, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<9, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<10, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<11, true>()), 2);
  BOOST_CHECK_EQUAL((numberOfMps<12, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<13, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<14, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<15, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<16, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<17, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<18, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<10, true>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<20, true>()), 0);

}

BOOST_AUTO_TEST_CASE(NofNonBendingMotifPositionsWhereContourIsMoreThanOnePolygon)
{
  BOOST_CHECK_EQUAL((numberOfMps<0, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<1, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<2, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<3, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<4, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<5, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<6, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<7, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<8, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<9, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<10, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<11, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<12, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<13, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<14, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<15, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<16, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<17, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<18, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<10, false>()), 0);
  BOOST_CHECK_EQUAL((numberOfMps<20, false>()), 0);

}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
