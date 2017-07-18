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

#include "AliMpArea.h"
#include "AliMpDDLStore.h"
#include "AliMpSegmentation.h"
#include "AliMpVPadIterator.h"
#include "AliMpVSegmentation.h"
#include "AliMpVSegmentation.h"
#include "de.h"
#include "readmapping.h"
#include "seg.h"
#include <array>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include "mapping.h"
#include "segnumbers.h"

namespace bdata = boost::unit_test::data;

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, Mapping)

BOOST_AUTO_TEST_SUITE(pads)

namespace {

int countPadsInSegmentation(const AliMpVSegmentation& seg)
{
  std::unique_ptr<AliMpVPadIterator> it{seg.CreateIterator()};

  it->First();

  int n = 0;

  while (!it->IsDone()) {
    it->Next();
    ++n;
  }

  return n;
}
} // anonymous namespace


BOOST_DATA_TEST_CASE(countBendingPads, (bdata::make(segnames) ^ bdata::make(bpads)), segname, npads)
{
  AliMpVSegmentation* seg = getSegmentation(segname, AliMp::kBendingPlane);

  int n = countPadsInSegmentation(*seg);

  BOOST_TEST_CHECK((n == npads), " n=" << n << " expected=" << npads);
  BOOST_TEST_CHECK((n == seg->NofPads()));
}

BOOST_DATA_TEST_CASE(countNonBendingPads, (bdata::make(segnames) ^ bdata::make(nbpads)), segname, npads)
{
  AliMpVSegmentation* seg = getSegmentation(segname, AliMp::kNonBendingPlane);

  int n = countPadsInSegmentation(*seg);

  BOOST_TEST_CHECK((n == npads), " n=" << n << " expected=" << npads);
  BOOST_TEST_CHECK((n == seg->NofPads()));
}

BOOST_AUTO_TEST_CASE(countAllPads)
{

  int b{0};
  int nb{0};

  for (const auto& segname : segnames) {
    AliMpVSegmentation* bseg = getSegmentation(segname, AliMp::kBendingPlane);
    b += countPadsInSegmentation(*bseg);
    AliMpVSegmentation* nbseg = getSegmentation(segname, AliMp::kNonBendingPlane);
    nb += countPadsInSegmentation(*nbseg);
  }
  BOOST_TEST_CHECK((b == 79427), "b=" << b << " expected 79427");
  BOOST_TEST_CHECK((nb == 64042), "nb=" << nb << " expected 64042");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
