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

#define BOOST_TEST_MODULE mch mapping test
#include <boost/test/included/unit_test.hpp>

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

namespace bdata = boost::unit_test::data;

struct Mapping
{
    Mapping()
    {
      ddlStore();
    }

    AliMpDDLStore* ddlStore()
    {
      static AliMpDDLStore* ddlStore = nullptr;
      static AliMpSegmentation* mseg = nullptr;
      if (!ddlStore) { readMapping(ddlStore, mseg); }
      return ddlStore;
    }

    AliMpSegmentation* mseg()
    {
      static AliMpSegmentation* mseg = AliMpSegmentation::Instance();
      return mseg;
    }

    std::vector<int> deids()
    {
      static std::vector<int> v = get_deids(ddlStore());
      return v;
    }

    std::vector<AliMpVSegmentation*> b_segs()
    {
      static std::vector<AliMpVSegmentation*> v = get_segs(mseg(), deids(), AliMp::kBendingPlane);
      return v;
    };

    std::vector<AliMpVSegmentation*> nb_segs()
    {
      static std::vector<AliMpVSegmentation*> v = get_segs(mseg(), deids(), AliMp::kNonBendingPlane);
      return v;
    };
};

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, Mapping)

BOOST_AUTO_TEST_CASE(loadMapping)
{
  BOOST_TEST_REQUIRE(ddlStore());
  BOOST_TEST_REQUIRE(mseg());
  BOOST_TEST_REQUIRE(deids().size() == 156);
}

namespace {

std::vector<std::string> segnames{
  "st1",
  "st2",
  "122000SR1",
  "112200SR2",
  "122200S",
  "222000N",
  "220000N",
  "122000NR1",
  "112200NR2",
  "122200N",
  "122330N",
  "112233NR3",
  "112230N",
  "222330N",
  "223300N",
  "333000N",
  "330000N",
  "112233N",
  "222333N",
  "223330N",
  "333300N"
};

std::array<int, 21> bpads = {
  14392, 13947, 2928, 3568, 3120, 1920, 1280, 3008, 3648, 3200, 3200, 4096, 4160, 2560, 1920, 960, 640, 4480, 2880,
  2240, 1280
};

std::array<int, 21> nbpads = {
  14280, 13986, 2048, 2496, 2176, 1344, 896, 2112, 2560, 2240, 2240, 2880, 2912, 1792, 1344, 672, 448, 3136, 2016,
  1568, 896
};

AliMpVSegmentation* getSegmentation(std::string segname, AliMp::PlaneType plane)
{
  Mapping m;

  const std::vector<AliMpVSegmentation*>& segs = (plane == AliMp::kBendingPlane ? m.b_segs() : m.nb_segs());

  auto ix = std::find_if(segs.begin(), segs.end(),
                         [&](AliMpVSegmentation* seg) { return get_segtype(*seg) == segname; });

  if (ix != segs.end()) {
    return *ix;
  }
  return nullptr;
}

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

BOOST_AUTO_TEST_CASE(segmentationOrder)
{
  for (auto i = 0; i < b_segs().size(); ++i) {
    BOOST_TEST_CHECK((get_segtype(*(b_segs()[i])) == segnames[i]));
  }
  for (auto i = 0; i < nb_segs().size(); ++i) {
    BOOST_TEST_CHECK((get_segtype(*(nb_segs()[i])) == segnames[i]));
  }
}

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

BOOST_AUTO_TEST_CASE(testSt2Manu1098) {

  AliMpVSegmentation* seg = getSegmentation("st2",AliMp::kNonBendingPlane);

  AliMpPad pad = seg->PadByPosition(14.75,24.25);
  BOOST_TEST_CHECK(pad.GetManuId()==1122);

  pad = seg->PadByPosition(14.75,24.75);
  BOOST_TEST_CHECK(pad.GetManuId()==1122);

  pad = seg->PadByPosition(14.75,24.25-0.5);
  BOOST_TEST_CHECK(pad.GetManuId()==1098);
}

BOOST_DATA_TEST_CASE(testPadUniqueness, (bdata::make({"st2"}) * bdata::make({AliMp::kNonBendingPlane})), segname, plane)
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
