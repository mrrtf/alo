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
#include "mappingFixture.h"
#include "segmentationContours.h"
#include "segmentationSVGWriter.h"
#include "svgWriter.h"
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
#include <segmentation.h>
#include <set>
#include <utility>
#include <vector>

using namespace o2::mch::mapping;
using namespace o2::mch::contour;

using Area=std::pair<bool, AliMpArea>;

std::ostream &operator<<(ostream &os, const Area &a)
{
  os << a.first << " | " << a.second;
  return os;
}

class area_dataset
{
  public:

    enum
    {
        arity = 1
    };

    area_dataset(std::vector<AliMpArea> areas) : mAreas{areas}
    {}

    std::vector<AliMpArea> mAreas;

    boost::unit_test::data::size_t size() const
    { return mAreas.size(); }

    using iterator=std::vector<AliMpArea>::const_iterator;

    // iterator
    iterator begin() const
    { return std::begin(mAreas); }
};

namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {
// registering area_dataset as a proper dataset
template<>
struct is_dataset<area_dataset> : ::boost::mpl::true_
{
};
}
}
}
}

SVGWriter writeSegmentation(const Segmentation &o2Seg, const BBox<double> &box,
                            const BBox<double> &area)
{
  SVGWriter w(box);

  w.addStyle(svgSegmentationDefaultStyle());

  w.addStyle(R"(
.area {
  fill:yellow;
  stroke-width:0.075px;
  stroke: red;
  opacity: 0.4;
}
}
)");

  svgSegmentation(o2Seg, w, true, true, true, false);

  w.svgGroupStart("area");
  w.box(area.xmin(), area.ymin(), area.width(), area.height());
  w.svgGroupEnd();

  return w;
}


BOOST_AUTO_TEST_SUITE(o2_mch_mapping)
BOOST_FIXTURE_TEST_SUITE(segmentationvsaliroot, MAPPING)

AliMpArea shrink(const AliMpArea &area, double offset = 1E-5)
{
  return {
    area.GetPositionX(), area.GetPositionY(),
    area.GetDimensionX() - offset,
    area.GetDimensionY() - offset,
  };
}

void dumpToFile(const std::string &fileName, const Segmentation &o2Seg, const AliMpArea &area)
{
  std::ofstream out(fileName);
  auto bbox = getBBox(o2Seg);
  BBox<double> barea{
    area.GetPositionX() - area.GetDimensionX(),
    area.GetPositionY() - area.GetDimensionY(),
    area.GetPositionX() + area.GetDimensionX(),
    area.GetPositionY() + area.GetDimensionY()
  };
  SVGWriter w = writeSegmentation(o2Seg, bbox, barea);
  std::vector<std::pair<double, double>> pads;
  o2Seg.forEachPadInArea(barea.xmin(), barea.ymin(), barea.xmax(), barea.ymax(), [&o2Seg, &pads](int paduid) {
    pads.push_back(std::make_pair(o2Seg.padPositionX(paduid), o2Seg.padPositionY(paduid)));
  });
  w.svgGroupStart("testpoints");
  w.points(pads);
  w.svgGroupEnd();
  w.writeHTML(out);
}


/// Compare pads within a given area
///
/// area is given relative to center for all detection elements
///
bool comparePadsInArea(AliMpSegmentation *mseg, int detElemId, bool isBendingPlane,
                       AliMpArea area)
{

  const AliMpVSegmentation *alSeg = alirootSegmentation(mseg, detElemId, isBendingPlane);

  if (!area.IsValid()) {
    if (detElemId < 500) {
      area = AliMpArea(alSeg->GetPositionX() + alSeg->GetDimensionX(),
                       alSeg->GetPositionY() + alSeg->GetDimensionY(),
                       alSeg->GetDimensionX(),
                       alSeg->GetDimensionY());
    } else {
      area = AliMpArea(alSeg->GetPositionX() - alSeg->GetDimensionX(),
                       alSeg->GetPositionY() - alSeg->GetDimensionY(),
                       alSeg->GetDimensionX(),
                       alSeg->GetDimensionY());
    }
  }

  // in order to put aside precision differences between aliroot and o2, we
  // shrink a bit the area. Kind of cheating, but the point here is not
  // to reproduce aliroot 100%
  area = shrink(area);

  std::unique_ptr<AliMpVPadIterator> it{alSeg->CreateIterator(area)};
  it->First();

  std::vector<AliMpPad> alPads;

  while (!it->IsDone()) {
    AliMpPad pad = it->CurrentItem();
    alPads.push_back(pad);
    it->Next();
  }

  o2::mch::mapping::Segmentation o2Seg(detElemId, isBendingPlane);

  std::vector<int> o2Pads;

  o2Seg.forEachPadInArea(area.LeftBorder(), area.DownBorder(),
                         area.RightBorder(), area.UpBorder(), [&o2Pads](int paduid) {
      o2Pads.push_back(paduid);
    });

  if (1==1) { // alPads.size() != o2Pads.size()) {
    static int n{0};
    std::cout << "+++ alPads.size()= " << alPads.size() << " o2Pads.size()= "
              << o2Pads.size() << "\n";
    dumpToFile(
      "bug-pia-" + std::to_string(detElemId) + "-" + (isBendingPlane ? "B" : "NB") + "-" + std::to_string(n++) +
      ".html", o2Seg, area);
  }

  return alPads.size() == o2Pads.size();
}

BOOST_DATA_TEST_CASE(SamePadsInAreaSt12,
                     boost::unit_test::data::make({
                                                    /*AliMpArea{0.0, 0.0, 0.0, 0.0}*/
                                                    AliMpArea{40.0, 40.0, 10.0, 10.0},
                                                    AliMpArea{60, 60, 20, 20},
                                                    AliMpArea{10,10,7,7}
                                                  }) *
                     boost::unit_test::data::make(100) *
                     boost::unit_test::data::make({true,false}),
                     area, detElemId, isBendingPlane)
{
  BOOST_TEST(comparePadsInArea(mseg, detElemId, isBendingPlane, area));
}

BOOST_DATA_TEST_CASE(SamePadsInAreaSt345,
                     boost::unit_test::data::make({
                                                    /*AliMpArea{0.0, 0.0, 0.0, 0.0},*/
                                                    /*AliMpArea{2.3, -5.5, 3.0, 20.0},*/
                                                    /*AliMpArea{2.3, 15.25, 3.5, 2.4}*/
//                                                     AliMpArea{0.0,0.0,50.0,10.0}, /* fully contained */
//                                                    AliMpArea{-30.0,0.0,50.0,10.0}, /* overflow left */
//                                                    AliMpArea{0.0,-15.0,50.0,10.0}, /* overflow top */
//                                                    AliMpArea{0.0,15.0,50.0,10.0}, /* overflow bottom */
//                                                    AliMpArea{-30.0,15.0,50.0,10.0}, /* overflow bottom-left */
//                                                    AliMpArea{-30.0,-15.0,50.0,10.0}, /* overflow top-left */
                                                    AliMpArea{30.0,0.0,50.0,10.0}, /* overflow right */
                                                    //AliMpArea{30.0,-15.0,50.0,10.0}, /* overflow top-right */
                                                    //AliMpArea{30.0,15.0,50.0,10.0} /* overflow bottom-right */


                                                  }) *
                       /*boost::unit_test::data::make(MAPPING::st345DetElemIds) * */
                     boost::unit_test::data::make({500}) *
                     boost::unit_test::data::make({true}),
                     area, detElemId, isBendingPlane)
{
  BOOST_TEST(comparePadsInArea(mseg, detElemId, isBendingPlane, area));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
