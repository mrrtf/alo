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

#include "segmentation.h"
#include "segmentationContours.h"
#include "svgContour.h"
#include <iostream>

using namespace o2::mch::svg;

int main()
{
  o2::mch::mapping::forOneDetectionElementOfEachSegmentationType([](int detElemId) {
    for (auto isBendingPlane : {true, false}) {
      o2::mch::mapping::Segmentation seg{detElemId, isBendingPlane};
      std::ostringstream filename;
      filename << "segmentation-contour-" << detElemId << "-" << (isBendingPlane ? "B" : "NB") << ".html";
      std::cout << "computing " << filename.str() << "\n";
      auto contours = o2::mch::mapping::getSampaContours(detElemId,isBendingPlane);
      writeContours(contours, filename.str().c_str(), 0.0, 0.0);
    }
  });

  return 0;
}