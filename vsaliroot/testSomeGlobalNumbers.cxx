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
#include "AliMpDetElement.h"
#include "AliMpManuIterator.h"
#include "AliMpMotif.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPad.h"
#include "AliMpSegmentation.h"
#include "AliMpVPadIterator.h"
#include "AliMpVSegmentation.h"
#include "contour.h"
#include "contourCreator.h"
#include "svgContour.h"
#include <TArrayD.h>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

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

    static std::vector<int> detElemIds;
};

AliMpDDLStore *MAPPING::ddlStore{nullptr};
AliMpSegmentation *MAPPING::mseg{nullptr};
std::vector<int> MAPPING::detElemIds
  {
    100, 300, 500, 501, 502, 503, 504, 600, 601, 602, 700, 701, 702, 703, 704, 705, 706, 902, 903, 904, 905
  };

BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentation_global_numbers, MAPPING)

BOOST_AUTO_TEST_CASE(CountNumberOfXYPositions)
{
  bool verbose{false};
  std::set<float> posx;
  std::set<float> posy;

  for (auto detElemId : detElemIds) {
    AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);

    std::set<float> deposx;
    std::set<float> deposy;

    if (verbose) {
      std::cout << "DE " << detElemId;
    }
    for (auto isBendingPlane : {true, false}) {

      auto al = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
        isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

      std::unique_ptr<AliMpVPadIterator> it{al->CreateIterator()};

      it->First();

      // yes, set of floats might be a bad idea, but it seems to
      // do the trick for the task at hand.
      std::set<float> planeposx;
      std::set<float> planeposy;

      while (!it->IsDone()) {
        AliMpPad pad = it->CurrentItem();
        planeposx.insert(static_cast<float>(pad.GetPositionX()));
        planeposy.insert(static_cast<float>(pad.GetPositionY()));
        deposx.insert(static_cast<float>(pad.GetPositionX()));
        deposy.insert(static_cast<float>(pad.GetPositionY()));
        posx.insert(static_cast<float>(pad.GetPositionX()));
        posy.insert(static_cast<float>(pad.GetPositionY()));
        it->Next();
      }

      if (verbose) {
        std::cout << boost::format("%2c has %3d x pos and %3d y pos ")
                     % (isBendingPlane ? "B" : "NB") % planeposx.size() % planeposy.size();
      }
    }
    if (verbose) {
      std::cout << boost::format("-> overvall %3d x pos and %3d y pos\n") % deposx.size() % deposy.size();
    }
  }

  if (verbose) {
    std::cout << boost::format("All in all we have %3d x pos and %3d y pos\n") % posx.size() % posy.size();
    std::copy(posx.begin(), posx.end(), std::ostream_iterator<float>(std::cout, ","));
    std::cout << "\n";
    std::copy(posy.begin(), posy.end(), std::ostream_iterator<float>(std::cout, ","));
    std::cout << "\n";
  }
  BOOST_CHECK(posx.size() == 879);
  BOOST_CHECK(posy.size() == 724);
}

void print(const char *msg, const std::set<float> &s)
{
  std::cout << msg << " = " << s.size() << " values : ";
  std::copy(s.begin(), s.end(), std::ostream_iterator<float>(std::cout, " , "));
  std::cout << "\n";
}

int multiple(const std::set<float> &s)
{

  // check if elements of the vector are multiple of the first one
  // return the max scale factor (or zero if not multiple)

  std::vector<float> v{s.begin(), s.end()};

  float scale{1.0};

  if (v.size() > 1 ) {
    for (auto i = 1; i < v.size() && scale > 0.0; ++i) {
      scale = v[i]/v[0];
      if ( std::floor(scale) != scale ) {
        std::cout << v[i] << " not a multiple of " << v[0] << "\n";
        scale = 0.0;
      }
    }
  }
  if (scale==0.0) {
    print("no multiple for ",s);
  }
  return static_cast<int>(scale);
}

BOOST_AUTO_TEST_CASE(PadSizesAreMultipleOfEachOther)
{
  std::set<float> padSizeX;
  std::set<float> padSizeY;
  int maxmx{0};
  int maxmy{0};

  for (auto detElemId : detElemIds) {

    for (auto isBendingPlane : {true, false}) {

      AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);

      auto al = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
        isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

      std::unique_ptr<AliMpVPadIterator> it{al->CreateIterator()};

      it->First();

      // yes, set of floats might be a bad idea, but it seems to
      // do the trick for the task at hand.
      std::set<float> planePadSizeX;
      std::set<float> planePadSizeY;

      while (!it->IsDone()) {
        AliMpPad pad = it->CurrentItem();
        planePadSizeX.insert(static_cast<float>(2.0 * pad.GetDimensionX()));
        planePadSizeY.insert(static_cast<float>(2.0 * pad.GetDimensionY()));
        padSizeX.insert(static_cast<float>(2.0 * pad.GetDimensionX()));
        padSizeY.insert(static_cast<float>(2.0 * pad.GetDimensionY()));
        it->Next();
      }

      int mx = multiple(planePadSizeX);
      int my = multiple(planePadSizeY);

      maxmx = std::max(mx, maxmx);
      maxmy = std::max(my, maxmy);

      if (detElemId==100)
      {
        // DE 100 BendingPlane is the one allowed exception with one pad size which is not a multiple
        // of the first one. But that's of no big consequence as this pad size (0.84cm in x)
        // is for a special at the end of a row (so strictly speaking it is not needed
        // to compute the bottom-left of pad positions)
        if ( isBendingPlane ) {
          BOOST_CHECK(mx==0);
        }
        else {
          BOOST_CHECK(mx>0);
        }
      } else
      {
       BOOST_CHECK(mx > 0);
      }
      BOOST_CHECK(my > 0);
    }
  }

  BOOST_CHECK(maxmx==4);
  BOOST_CHECK(maxmy==8);
  BOOST_CHECK(padSizeX.size()==11);
  BOOST_CHECK(padSizeY.size()==10);
}

BOOST_AUTO_TEST_CASE(PadIntegerIndicesRanges)
{
  int pixmax{0};
  int piymax{0};

  for (auto detElemId : detElemIds) {

    for (auto isBendingPlane : {true,false}) {

      float x{0.0};
      float y{0.0};
      AliMpDetElement *detElement = AliMpDDLStore::Instance()->GetDetElement(detElemId);

      auto al = mseg->GetMpSegmentation(detElemId, detElement->GetCathodType(
        isBendingPlane ? AliMp::kBendingPlane : AliMp::kNonBendingPlane));

      std::unique_ptr<AliMpVPadIterator> it{al->CreateIterator()};

      it->First();

      // yes, set of floats might be a bad idea, but it seems to
      // do the trick for the task at hand.
      std::set<float> planePadSizeX;
      std::set<float> planePadSizeY;

      double offsetx{0.0};
      double offsety{0.0};

      // why do I have to correct this here by hand ???
      if ( detElemId == 100 && isBendingPlane == false ) {
        offsetx = -0.315;
        offsety = 0.21;
      }
      if (detElemId == 300 && isBendingPlane == true ) {
        offsetx = -1.0;
        offsety = -0.75;
      }
      if (detElemId == 300 && isBendingPlane == false ) {
        offsetx = -0.625;
        offsety = -0.5;
      }

      while (!it->IsDone()) {
        AliMpPad pad = it->CurrentItem();
        float xleft = pad.GetPositionX() - pad.GetDimensionX() + al->GetPositionX() - offsetx;
        float ybottom = pad.GetPositionY() - pad.GetDimensionY() + al->GetPositionY() - offsety;

        planePadSizeX.insert(static_cast<float>(2.0 * pad.GetDimensionX()));
        planePadSizeY.insert(static_cast<float>(2.0 * pad.GetDimensionY()));

        if ( xleft < -0.01 || ybottom < -0.01 ) {
          std::cout << xleft << " " << ybottom << " ";
          pad.Print();
        }
        x = std::max(x,xleft);
        y = std::max(y,ybottom);
        it->Next();
      }

      float refPadSizeX = *(begin(planePadSizeX));
      float refPadSizeY = *(begin(planePadSizeY));

      float ix = x/refPadSizeX;
      float iy = y/refPadSizeY;

      BOOST_CHECK(std::floor(ix)-ix<1E-3);
      BOOST_CHECK(std::floor(iy)-iy<1E-3);

      int pix = static_cast<int>(std::floor(ix));
      int piy = static_cast<int>(std::floor(iy));

      pixmax = std::max(pix,pixmax);
      piymax = std::max(piy,piymax);

      //std::cout << boost::format("DE %4d B %c xmax %7.2f ymax %7.2f padsize min x %7.2f min y %7.2f pix %4d piy %4d\n") % detElemId % isBendingPlane % x % y % refPadSizeX % refPadSizeY % pix % piy;
    }
  }

  BOOST_CHECK_EQUAL(pixmax,335);
  BOOST_CHECK_EQUAL(piymax,235);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
