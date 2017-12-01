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
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include "genMotifType.h"
#include <array>
#include <limits>

using namespace o2::mch::mapping;

struct MT
{
    MotifType mt{arrayOfMotifTypes.back()};
};

std::array<std::pair<int, int>, 47> Non64Motifs = {
  std::make_pair<int, int>(4, 48),
  std::make_pair<int, int>(5, 63),
  std::make_pair<int, int>(7, 46),
  std::make_pair<int, int>(8, 32),
  std::make_pair<int, int>(16, 48),
  std::make_pair<int, int>(17, 56),
  std::make_pair<int, int>(18, 40),
  std::make_pair<int, int>(19, 40),
  std::make_pair<int, int>(31, 48),
  std::make_pair<int, int>(32, 48),
  std::make_pair<int, int>(41, 62),
  std::make_pair<int, int>(42, 48),
  std::make_pair<int, int>(47, 62),
  std::make_pair<int, int>(48, 62),
  std::make_pair<int, int>(49, 49),
  std::make_pair<int, int>(50, 39),
  std::make_pair<int, int>(51, 32),
  std::make_pair<int, int>(52, 50),
  std::make_pair<int, int>(55, 57),
  std::make_pair<int, int>(56, 40),
  std::make_pair<int, int>(58, 58),
  std::make_pair<int, int>(61, 60),
  std::make_pair<int, int>(76, 52),
  std::make_pair<int, int>(77, 56),
  std::make_pair<int, int>(78, 55),
  std::make_pair<int, int>(79, 60),
  std::make_pair<int, int>(80, 62),
  std::make_pair<int, int>(81, 60),
  std::make_pair<int, int>(82, 59),
  std::make_pair<int, int>(83, 46),
  std::make_pair<int, int>(85, 31),
  std::make_pair<int, int>(86, 57),
  std::make_pair<int, int>(87, 60),
  std::make_pair<int, int>(88, 63),
  std::make_pair<int, int>(89, 57),
  std::make_pair<int, int>(92, 58),
  std::make_pair<int, int>(93, 54),
  std::make_pair<int, int>(95, 48),
  std::make_pair<int, int>(96, 54),
  std::make_pair<int, int>(97, 60),
  std::make_pair<int, int>(99, 60),
  std::make_pair<int, int>(158, 48),
  std::make_pair<int, int>(183, 56),
  std::make_pair<int, int>(184, 56),
  std::make_pair<int, int>(185, 56),
  std::make_pair<int, int>(186, 56),
  std::make_pair<int, int>(201, 48)
};

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_AUTO_TEST_SUITE(motiftype)

BOOST_AUTO_TEST_CASE(NumberOfMotifTypes)
{
  BOOST_CHECK_EQUAL(arrayOfMotifTypes.size(), 210);
}

BOOST_AUTO_TEST_CASE(NumberOfNon64Motifs)
{
  BOOST_CHECK_EQUAL(Non64Motifs.size(), 47);
}

BOOST_AUTO_TEST_CASE(NumberOfPads)
{
  for (auto i = 0; i < arrayOfMotifTypes.size(); ++i) {
    const MotifType &mt = arrayOfMotifTypes[i];
    auto f = std::find_if(Non64Motifs.begin(), Non64Motifs.end(),
                          [&](const std::pair<int, int> &p) { return p.first == i; });
    if (f == Non64Motifs.end()) {
      BOOST_CHECK_EQUAL(mt.getNofPads(), 64);
    } else {
      BOOST_CHECK_EQUAL(mt.getNofPads(), f->second);
    }
  }
}

//BOOST_DATA_TEST_CASE(NofPadsInXAndYAreCompatibleWithIxAndIyRanges, boost::unit_test::data::xrange(0,1), i)

BOOST_DATA_TEST_CASE(NofPadsInXAndYAreCompatibleWithIxAndIyRanges, boost::unit_test::data::make({58}),i)
{
  const MotifType &mt = arrayOfMotifTypes[i];
  int npads{0};
  int ixmin{std::numeric_limits<int>::max()};
  int iymin{std::numeric_limits<int>::max()};
  int ixmax{0};
  int iymax{0};
  for (int ix = 0; ix < mt.getNofPadsX(); ++ix) {
    for (int iy = 0; iy < mt.getNofPadsY(); ++iy) {
      int index = mt.padIdByIndices(ix, iy);
      if (index >= 0) {
        std::cout << npads << " ix=" << ix << " iy=" << iy << " index=" << index << " berg="
                  << mt.getBerg(index) << "\n";
        ixmax = std::max(ix, ixmax);
        iymax = std::max(iy, iymax);
        ixmin = std::min(ix, ixmin);
        iymin = std::min(iy, iymin);
        ++npads;
      }
    }
  }
  if (npads != mt.getNofPads()) {
    std::cout << "i=" << i << "\n";
  }
  BOOST_CHECK_EQUAL(npads, mt.getNofPads());
  int nx{ixmax - ixmin + 1};
  int ny{iymax - iymin + 1};
  BOOST_CHECK_EQUAL(nx, mt.getNofPadsX());
  BOOST_CHECK_EQUAL(ny, mt.getNofPadsY());
}

BOOST_FIXTURE_TEST_SUITE(PadBy, MT)

BOOST_AUTO_TEST_CASE(PadIdByValidBerg)
{
  BOOST_CHECK_EQUAL(mt.padIdByBerg(83), 63);
  BOOST_CHECK_EQUAL(mt.padIdByBerg(33), 0);
}

BOOST_AUTO_TEST_CASE(HasPadByBerg)
{
  BOOST_CHECK_EQUAL(mt.hasPadByBerg(83), true);
  BOOST_CHECK_EQUAL(mt.hasPadByBerg(33), true);
  BOOST_CHECK_EQUAL(mt.hasPadByBerg(100), false);
  BOOST_CHECK_EQUAL(mt.hasPadByBerg(-1), false);
}

BOOST_AUTO_TEST_CASE(HasPadByIndices)
{
  BOOST_CHECK_EQUAL(mt.hasPadByIndices(2, 7), true);
  BOOST_CHECK_EQUAL(mt.hasPadByIndices(0, 24), true);
  BOOST_CHECK_EQUAL(mt.hasPadByIndices(2, 24), false);
  BOOST_CHECK_EQUAL(mt.hasPadByIndices(-1, -1), false);
}

BOOST_AUTO_TEST_CASE(PadIdByInvalidBerg)
{
  BOOST_CHECK_EQUAL(mt.padIdByBerg(-1), -1);
  BOOST_CHECK_EQUAL(mt.padIdByBerg(100), -1);
}

BOOST_AUTO_TEST_CASE(PadIdByValidIndices)
{
  BOOST_CHECK_EQUAL(mt.padIdByIndices(2, 7), 63);
  BOOST_CHECK_EQUAL(mt.padIdByIndices(0, 24), 0);
}

BOOST_AUTO_TEST_CASE(PadIdByInvalidIndices)
{
  BOOST_CHECK_EQUAL(mt.padIdByIndices(2, 24), -1);
  BOOST_CHECK_EQUAL(mt.padIdByIndices(-1, -1), -1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(IntegerRangesAreShort)
{
  int maxBerg{0};
  int maxIx{0};
  int maxIy{0};
  for (auto i = 0; i < arrayOfMotifTypes.size(); ++i) {
    const MotifType &mt = arrayOfMotifTypes[i];
    for (auto p = 0; p < mt.getNofPads(); ++p) {
      maxBerg = std::max(maxBerg, static_cast<int>(mt.getBerg(p)));
      maxIx = std::max(maxIx, static_cast<int>(mt.getIx(p)));
      maxIy = std::max(maxIy, static_cast<int>(mt.getIy(p)));
    }
  }
  BOOST_CHECK_EQUAL(maxBerg, 83);
  BOOST_CHECK_EQUAL(maxIx, 27);
  BOOST_CHECK_EQUAL(maxIy, 63);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
