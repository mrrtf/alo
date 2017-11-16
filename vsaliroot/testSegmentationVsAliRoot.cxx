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
#include "AliMpDetElement.h"
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
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <genDESegmentationFactory.h>
#include <genDetectionElement.h>

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
  bool aliroot = alseg.PadByPosition(x, y, false).IsValid();
  bool o2 = seg.hasPadByPosition(x, y);
  if (o2 != aliroot) {
    std::cout << "o2=" << o2 << " and aliroot=" << aliroot << " for x=" << x << " and y=" << y << "\n";
  }
  return aliroot == o2;
}

void writeContour(std::ofstream &out, int scale, o2::mch::contour::Contour<double> &contour,
                  o2::mch::contour::BBox<double>& box, bool highlight=false)
{
  for (auto i = 0; i < contour.size(); ++i) {
    out << "<polygon points=\"";
    auto vertices = contour[i].getVertices();
    for (auto j = 0; j < vertices.size(); ++j) {
      auto v = vertices[j];
      v.x -= box.xmin();
      v.y -= box.ymin();
      out << scale * v.x << "," << scale * v.y << ' ';
    }
    if (!highlight) {
      out << "\" style=\"fill:none;stroke:black;stroke-width:1\"/>\n";
    }
    else
    {
      out << "\" style=\"fill:none;stroke:red;stroke-width:1\"/>\n";
    }
  }
}

void writeSVGHeader(std::ofstream& out, o2::mch::contour::BBox<double>& box, int scale) {
  out << "<svg height=\"" << scale * box.height()+100 << "\" width=\"" << scale * box.width()+100 << "\">\n";
}

void writeContour(std::string filename, o2::mch::contour::Contour<double> &contour)
{
  std::ofstream out(filename);
  int scale = 5;
  out << "<html><body>\n";
  auto box = getBBox(contour);
  writeSVGHeader(out,box,scale);
  writeContour(out, scale, contour, box);
  out << "</svg>\n";
  out << "</body></html>\n";
}

std::pair<const AliMpVSegmentation *, SegmentationInterface *>
getSegmentations(AliMpSegmentation *mseg, int detElemId, int type, bool isBendingPlane)
{
  AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);

  auto al = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
    isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

  auto o2 = getSegmentation(type, isBendingPlane);

  return {al, o2.get()};
}

bool checkHasPadByPosition(AliMpSegmentation *mseg, int detElemId, int type, bool isBendingPlane, int ntimes)
{
  auto pair = getSegmentations(mseg, detElemId, type, isBendingPlane);
  auto al = pair.first;
  auto o2 = pair.second;

  auto env = o2->getEnvelop();
  auto bbox = getBBox(env);

  std::ostringstream filename;
  filename << "toto-" << detElemId << "-" << type << ".html";
  writeContour(filename.str(), env);

  bool same{true};
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> disx(bbox.xmin(), bbox.xmax());
  std::uniform_real_distribution<> disy(bbox.ymin(), bbox.ymax());
  for (int i = 0; i < ntimes && same; ++i) {
    double x = disx(gen);
    double y = disy(gen);
    same = sameHasPadByPosition(*al, *o2, x, y);
  }
  if (!same) {
    std::cout << "detElemId=" << detElemId << "\n";
    std::cout << "type=" << type << "\n";
    std::cout << "isBendingPlane=" << isBendingPlane << "\n";
    std::cout << "envelop=" << env << "\n";
    std::cout << "bbox=" << bbox << "\n";
  }
  return same;
}

BOOST_AUTO_TEST_CASE(One)
{
  int detElemId{101};
  int type{0};
  bool isBendingPlane{1};
  //double x{90.1511};
  double x{89.04};
  double y{28.4681};
  auto pair = getSegmentations(mseg, detElemId, type, isBendingPlane);
  BOOST_CHECK(sameHasPadByPosition(*(pair.first), *(pair.second), x, y));

  auto o2 = getSegmentation(0, true);
  auto contours = o2->getSampaContours();
  //std::vector<o2::mch::contour::Contour<double>> contours;// = o2->getSampaContours();

  std::ofstream out("bug.html");
  int scale = 5;
  out << "<html><body>\n";
  auto env = o2->getEnvelop();
  auto box = getBBox(env);
  std::cout << "bbox=" << box << std::endl;
  writeSVGHeader(out,box,scale);
  writeContour(out, scale, env, box);
  for (auto &&c: contours) {
    writeContour(out,scale,c,box);
    if (c.contains(x, y)) {
      writeContour(out,scale,c,box,true);
    }
  }

  out << "<circle cx=\"" << scale*x << "\" cy=\"" << scale*y << "\" r=\"5\"\n"
    "style=\"fill:none;stroke:black;stroke-width:1px;\"/>";

  out << "</svg>\n";
  out << "</body></html>\n";


  std::cout << pair.first->GetPositionX() <<  " " << pair.first->GetPositionY() << "\n";
  std::cout << pair.first->GetDimensionX()*2 <<  " " << pair.first->GetDimensionY()*2 << "\n";
  BOOST_CHECK(o2->hasPadByPosition(x, y) == true);
}

//BOOST_DATA_TEST_CASE(HasPadByPositionIsTheSameForAliRootAndO2,boost::unit_test::data::make({0,8,16,17,18,19,20,34,35,36,52,53,54,55,56,57,58,106,107,108,109}),deIndex)
//BOOST_DATA_TEST_CASE(HasPadByPositionIsTheSameForAliRootAndO2,boost::unit_test::data::make({16,17,18,19,20,34,35,36,52,53,54,55,56,57,58,106,107,108,109}),deIndex)
BOOST_DATA_TEST_CASE(HasPadByPositionIsTheSameForAliRootAndO2, boost::unit_test::data::xrange(0, 16), deIndex)
{
  int ntimes{100};
  int deId = o2::mch::mapping::getDetElemIdFromDetElemIndex(deIndex);
  int segTypeIndex = o2::mch::mapping::getSegTypeIndexFromDetElemIndex(deIndex);
  BOOST_TEST_REQUIRE(checkHasPadByPosition(mseg, deId, segTypeIndex, true, ntimes));
  BOOST_TEST_REQUIRE(checkHasPadByPosition(mseg, deId, segTypeIndex, false, ntimes));
}

void writeEnvelop(int type, bool bending)
{
  std::stringstream filename;
  filename << "envelop-" << type << "-" << bending << ".html";
  auto seg = getSegmentation(type, bending);
  std::cout << seg->nofDualSampas() << '\n';
  auto contour = seg->getEnvelop();
  writeContour(filename.str(), contour);
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
