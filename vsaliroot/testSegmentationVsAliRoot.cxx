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

#include "AliMpDDLStore.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpManuIterator.h"
#include "AliMpMotif.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <utility>
#include "genSegmentationFactory.h"

using namespace o2::mch::mapping;

struct MAPPING
{
    MAPPING()
    {
      if (!ddlStore) {
        AliMpDataProcessor mp;
        AliMpDataMap *dataMap = mp.CreateDataMap("data");
        AliMpDataStreams dataStreams(dataMap);
        ddlStore = AliMpDDLStore::ReadData(dataStreams);
        mseg = AliMpSegmentation::Instance();
      }
    }

    static AliMpDDLStore *ddlStore;
    static AliMpSegmentation *mseg;
};

AliMpDDLStore *MAPPING::ddlStore{nullptr};
AliMpSegmentation *MAPPING::mseg{nullptr};

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentation, MAPPING)

bool sameHasPadByPosition(const AliMpVSegmentation &alseg, const SegmentationInterface &seg, double x, double y)
{
  bool aliroot = alseg.PadByPosition(x, y).IsValid();
  bool o2 = seg.hasPadByPosition(x,y);
  if (o2!=aliroot) {
    std::cout << "o2=" << o2 << " and aliroot=" << aliroot << " for x=" << x << " and y=" << y << "\n";
  }
  return aliroot == o2;
}

void writeEnvelop(std::string filename, o2::mch::contour::Contour<double>& contour)
{
  std::ofstream out(filename);
  int scale = 5;
  out << "<html><body>\n";
  auto box = getBBox(contour);
  out << "<svg height=\"" << scale * box.height() << "\" width=\"" << scale * box.width() << "\">\n";
  for (auto i = 0; i < contour.size(); ++i) {
    out << "<polygon points=\"";
    auto vertices = contour[i].getVertices();
    for (auto j = 0; j < vertices.size(); ++j) {
      auto v = vertices[j];
      v.x -= box.xmin();
      v.y -= box.ymin();
      out << scale * v.x << "," << scale * v.y << ' ';
    }
    out << "\" style=\"fill:none;stroke:black;stroke-width:1\"/>\n";
  }
  out << "</svg>\n";
  out << "</body></html>\n";

}

bool checkHasPadByPosition(AliMpSegmentation *mseg, int detElemId, int type, int ntimes)
{
  auto al = mseg->GetMpSegmentation(detElemId, AliMp::kCath0);
  auto o2 = getSegmentation(type, true);
  // here should get a bunch of (x,y) within the bounding box of the contour of segmentation
  // instead of just one
  auto env = o2->getEnvelop();
  auto bbox = getBBox(env);
  std::cout << "envelop=" << env << "\n";
  std::ostringstream filename;
  filename << "toto-" << detElemId << "-" << type << ".html";
  writeEnvelop(filename.str(),env);
  std::cout << "bbox=" << bbox << "\n";
  bool same{true};
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> disx(bbox.xmin(), bbox.xmax());
  std::uniform_real_distribution<> disy(bbox.ymin(), bbox.ymax());
  for (int i = 0; i < ntimes && same; ++i) {
    double x = disx(gen);
    double y = disy(gen);
    same = sameHasPadByPosition(*al, *o2, x,y);
  }
  return same;
}

BOOST_AUTO_TEST_CASE(One)
{
  int ntimes{100};
  BOOST_TEST(checkHasPadByPosition(mseg, 505, 6,ntimes));
  BOOST_TEST(checkHasPadByPosition(mseg, 614, 6,ntimes));
}

void writeEnvelop(int type, bool bending)
{
  std::stringstream filename;
  filename << "envelop-" << type << "-" << bending << ".html";
  auto seg = getSegmentation(type, bending);
  std::cout << seg->nofDualSampas() << '\n';
  auto contour = seg->getEnvelop();
  writeEnvelop(filename.str(),contour);
}

//BOOST_AUTO_TEST_CASE(Envelop)
//{
//  for (auto i = 19; i >= 0; --i ) {
//    std::cout << "Envelop " << i << " true " << '\n';
//    writeEnvelop(i, true);
//    std::cout << "Envelop " << i << " false " << '\n';
//    writeEnvelop(i, false);
//  }
//  BOOST_TEST(false);
//}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
