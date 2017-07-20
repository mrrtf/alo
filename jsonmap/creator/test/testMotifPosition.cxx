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
#include <boost/test/data/test_case.hpp>
#include <AliMpMotifPosition.h>
#include <motifPosition.h>
#include "segnumbers.h"
#include "mapping.h"
#include <iostream>
#include <seg.h>
#include "motifPosition.h"
#include "AliMpPlaneType.h"
#include "TArrayI.h"
#include "AliMpVSegmentation.h"
#include "boost/format.hpp"

namespace bdata = boost::unit_test::data;
struct MPS
{
    Mapping m;
};

std::array<int, 21> motifPositionPerSegmentation{
  451, 443, 78, 95, 83, 51, 34, 80, 97, 85, 86, 110, 111, 69, 52, 27, 18, 120, 78, 61, 36
};
std::array<int, 21> motifPositionPerSegmentationB{
  226, 221, 46, 56, 49, 30, 20, 47, 57, 50, 50, 64, 65, 40, 30, 15, 10, 70, 45, 35, 20
};
std::array<int, 21> motifPositionPerSegmentationNB{
  225, 222, 32, 39, 34, 21, 14, 33, 40, 35, 36, 46, 46, 29, 22, 12, 8, 50, 33, 26, 16
};

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, MPS)

BOOST_AUTO_TEST_SUITE(motifposition)

BOOST_DATA_TEST_CASE(NumberOfMotifPositions, (bdata::make(segnames)), segname)
{
  std::vector<AliMpMotifPosition*> motifPositions = get_motifpositions(m.ddlStore(), m.mseg(), segname);

  auto i = std::distance(segnames.begin(), std::find(segnames.begin(), segnames.end(), segname));

  BOOST_TEST_CHECK(motifPositions.size() == motifPositionPerSegmentation[i]);
}

BOOST_DATA_TEST_CASE(NumberOfMotifPositionsInBendingPlane, (bdata::make(segnames)), segname)
{
  AliMpVSegmentation* segmentation = get_seg(m.ddlStore(), m.mseg(), segname, AliMp::kBendingPlane);

  std::vector<AliMpMotifPosition*> motifPositions = get_motifpositions(*segmentation);

  auto i = std::distance(segnames.begin(), std::find(segnames.begin(), segnames.end(), segname));

  BOOST_TEST_CHECK(motifPositions.size() == motifPositionPerSegmentationB[i]);
}

BOOST_DATA_TEST_CASE(NumberOfMotifPositionsInNonBendingPlane, (bdata::make(segnames)), segname)
{
  AliMpVSegmentation* segmentation = get_seg(m.ddlStore(), m.mseg(), segname, AliMp::kNonBendingPlane);

  std::vector<AliMpMotifPosition*> motifPositions = get_motifpositions(*segmentation);

  auto i = std::distance(segnames.begin(), std::find(segnames.begin(), segnames.end(), segname));

  BOOST_TEST_CHECK(motifPositions.size() == motifPositionPerSegmentationNB[i]);
}

/*
BOOST_AUTO_TEST_CASE(MotifPositionDetails)
{
  AliMpVSegmentation* segmentation = get_seg(m.ddlStore(), m.mseg(), "st1", AliMp::kBendingPlane);
  TArrayI manuId;
  segmentation->GetAllElectronicCardIDs(manuId);
  for (int i = 0; i < manuId.GetSize(); ++i) {
    AliMpMotifPosition* mp = segmentation->MotifPosition(manuId[i]);
    std::cout << boost::format("MP %3d (xmin,ymin)=(%7.2f,%7.2f) (xmax,ymax)=(%7.2f,%7.2f) ManuId %4d") % i %
                 (mp->GetPositionX() - mp->GetDimensionX()) %
                 (mp->GetPositionY() - mp->GetDimensionY()) % (mp->GetPositionX() + mp->GetDimensionX()) %
                 (mp->GetPositionY() + mp->GetDimensionY()) % manuId[i] << std::endl;
  }
  AliMpPad pad = segmentation->PadByPosition(40,30);
  pad.Print();
  BOOST_CHECK(true);
}
*/

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

