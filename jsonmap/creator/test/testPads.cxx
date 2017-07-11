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

BOOST_TEST_DECORATOR(* boost::unit_test::disabled())
BOOST_AUTO_TEST_CASE(testSt2Manu1098) {

  AliMpVSegmentation* seg = getSegmentation("st2",AliMp::kNonBendingPlane);

  AliMpPad pad = seg->PadByPosition(14.75,24.25);
  BOOST_TEST_CHECK(pad.GetManuId()==1122);

  pad = seg->PadByPosition(14.75,24.75);
  BOOST_TEST_CHECK(pad.GetManuId()==1122);

  pad = seg->PadByPosition(14.75,24.25-0.5);
  BOOST_TEST_CHECK(pad.GetManuId()==1098);
}

BOOST_TEST_DECORATOR(* boost::unit_test::disabled())
BOOST_DATA_TEST_CASE(testPadUniqueness , (bdata::make({"st2"}) * bdata::make({AliMp::kNonBendingPlane})), segname, plane)
{
  AliMpVSegmentation* seg = getSegmentation(segname, plane);
  AliMpArea area(16 + 1, 19.5 + 0.75, 12,12);
  std::unique_ptr<AliMpVPadIterator> it{seg->CreateIterator(area)};
  it->First();

  std::vector<AliMpPad> pads;

  while (!it->IsDone()) {
    it->Next();
    if (!it->IsDone()) {
      AliMpPad p = it->CurrentItem();
      pads.push_back(p);
    }
  }

  BOOST_TEST_CHECK((pads[0] == pads[0]));

  const float cmtomicron = 1E4;

  int n = 0;
  for (const auto& p : pads ) {
    if (p.GetManuId() == 1122 ) {
      n++;
    }
  }

  BOOST_TEST_CHECK((n==60),"n=" << n << " expected 60");

  for (auto i = 0; i < pads.size(); ++i) {
    for (auto j = i + 1; j < pads.size(); ++j) {
      double dx = pads[i].GetPositionX() - pads[j].GetPositionX();
      double dy = pads[i].GetPositionY() - pads[j].GetPositionY();
      double distance = sqrt(dx * dx + dy * dy);
      BOOST_TEST_CHECK(distance > 0.1);
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
