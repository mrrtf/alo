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

namespace o2 {
namespace mch {
namespace mapping {

std::vector<std::pair<double, double>> generateTestPoints(int n, double xmin, double ymin, double xmax, double ymax, int extent)
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::vector<std::pair<double, double>> testPoints;

  if (extent == 0) {
    testPoints.resize(n);
    std::uniform_real_distribution<double> distX{xmin,xmax};
    std::uniform_real_distribution<double> distY{ymin,ymax};
    std::generate(testPoints.begin(), testPoints.end(),
                  [&distX, &distY, &mt] { return std::make_pair<double, double>(distX(mt), distY(mt)); });

  } else {
    std::normal_distribution<double> dist(0.0, extent * 1.0);
    while (testPoints.size() < n) {
      double x = dist(mt);
      double y = dist(mt);
      if (x >= xmin && x <= xmax &&
          y >= ymin && y <= ymax) {
        testPoints.push_back({x, y});
      }
    }
  }
  return testPoints;
}

}
}
}
