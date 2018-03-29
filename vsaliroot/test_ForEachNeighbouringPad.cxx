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

#include "AliMpPad.h"
#include "AliMpVPadIterator.h"
#include "AliMpVSegmentation.h"
#include "MappingFixture.h"
#include "MCHContour/SVGWriter.h"
#include "MCHMappingInterface/Segmentation.h"
#include "MCHMappingSegContour/SegmentationContours.h"
#include "MCHMappingSegContour/SegmentationSVGWriter.h"
#include <AliMpArea.h>
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

using namespace o2::mch::mapping;
using namespace o2::mch::contour;

extern SVGWriter writeSegmentation(const Segmentation &o2Seg, const BBox<double> &box,
                                   const BBox<double> &area);

void dumpToFile(const std::string &fileName, const Segmentation &o2Seg, int paduid)
{
  std::ofstream out(fileName);
  auto bbox = getBBox(o2Seg);
  BBox<double> barea{
    o2Seg.padPositionX(paduid) - o2Seg.padSizeX(paduid) / 2.0,
    o2Seg.padPositionY(paduid) - o2Seg.padSizeY(paduid) / 2.0,
    o2Seg.padPositionX(paduid) + o2Seg.padSizeX(paduid) / 2.0,
    o2Seg.padPositionY(paduid) + o2Seg.padSizeY(paduid) / 2.0
  };

  std::vector<std::pair<double, double>> neighbours;

  o2Seg.forEachNeighbouringPad(paduid, [&neighbours, &o2Seg](int nuid) {
    neighbours.push_back(std::make_pair(o2Seg.padPositionX(nuid), o2Seg.padPositionY(nuid)));
  });

  SVGWriter w = writeSegmentation(o2Seg, bbox, barea);
  w.svgGroupStart("testpoints");
  w.points(neighbours);
  w.svgGroupEnd();
  w.writeHTML(out);
}

bool compareNeighbours(const AliMpVSegmentation &alSeg, const AliMpPad &pad, const Segmentation &o2Seg, int paduid)
{

  TObjArray alNeighbours;
  alSeg.GetNeighbours(pad, alNeighbours);

  std::vector<int> o2Neighbours;
  o2Seg.forEachNeighbouringPad(paduid, [&o2Neighbours](int nuid) {
    o2Neighbours.push_back(nuid);
  });

  return (o2Neighbours.size() == alNeighbours.GetEntries());
}

bool compareNeighbouringPads(AliMpSegmentation *mseg, int detElemId, bool isBendingPlane)
{
  const AliMpVSegmentation *alSeg = alirootSegmentation(mseg, detElemId, isBendingPlane);
  std::unique_ptr<AliMpVPadIterator> it{alSeg->CreateIterator()};
  Segmentation o2seg(detElemId, isBendingPlane);

  it->First();

  int naliroot{0};
  int no2{0};
  while (!it->IsDone()) {
    AliMpPad pad = it->CurrentItem();
    ++naliroot;
    int paduid = o2seg.findPadByFEE(pad.GetManuId(), pad.GetManuChannel());
    if (o2seg.isValid(paduid) && o2seg.padDualSampaId(paduid) == pad.GetManuId() &&
        o2seg.padDualSampaChannel(paduid) == pad.GetManuChannel()) {
      ++no2;
      if (!compareNeighbours(*alSeg, pad, o2seg, paduid)) {
        dumpToFile("bug-pnp-" + std::to_string(detElemId) + "-" + (isBendingPlane ? "B" : "NB") + ".html", o2seg,
                   paduid);
        return false;
      }
    }
    it->Next();
  }
  return no2 == naliroot;
}

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentationvsaliroot, MAPPING)

BOOST_DATA_TEST_CASE(SameNeighbouringPads,
                     boost::unit_test::data::make(MAPPING::detElemIds) *
                     boost::unit_test::data::make({true,false}),
                     detElemId, isBendingPlane)
{
  BOOST_TEST(compareNeighbouringPads(mseg, detElemId, isBendingPlane));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
