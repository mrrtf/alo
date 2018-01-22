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


#include "generateTestPoints.h"
#include <random>
#include "segmentation.h"
#include "segmentationContours.h"
#include "contourCreator.h"

using o2::mch::contour::getBBox;
using o2::mch::contour::getEnvelop;

namespace o2 {
namespace mch {
namespace mapping {


std::vector<std::pair<double, double>> generateTestPoints(int n, int detElemId, int extent)
{
  Segmentation seg{detElemId,true};
  auto bbox = getBBox(getEnvelop(getSampaContours(seg)));

  double offset{0.0};

  std::random_device rd;
  std::mt19937 mt(rd());
  std::vector<std::pair<double, double>> testPoints;

  if (extent == 0) {

    testPoints.resize(n);

    std::uniform_real_distribution<double> distX{bbox.xmin() - offset, bbox.xmax() + offset};
    std::uniform_real_distribution<double> distY{bbox.ymin() - offset, bbox.ymax() + offset};

    std::generate(testPoints.begin(), testPoints.end(),
                  [&distX, &distY, &mt] { return std::make_pair<double, double>(distX(mt), distY(mt)); });

  } else {
    std::normal_distribution<double> dist(0.0, extent * 1.0);
    while (testPoints.size() < n) {
      double x = dist(mt);
      double y = dist(mt);
      if (x >= bbox.xmin() - offset && x <= bbox.xmax() + offset &&
          y >= bbox.ymin() - offset && y <= bbox.ymax() + offset) {
        testPoints.push_back({x, y});
      }
    }
  }
  return testPoints;
}
}}}
