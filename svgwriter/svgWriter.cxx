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

#include "genSegmentationFactory.h"
#include "svgSegmentationInterface.h"

int main()
{
  for ( int i = 0; i < o2::mch::mapping::getNumberOfSegmentations(); ++i ) {
    for ( auto isBendingPlane : { true, false} ) {
      auto o2 = o2::mch::mapping::getSegmentation(i, isBendingPlane);
      auto bbox = o2::mch::contour::getBBox(o2->getEnvelop());
      std::ostringstream filename;
      filename << "segmentation-contour-" << i << "-" << (isBendingPlane ? "B" : "NB") << ".html";
      o2::mch::svg::writeSegmentationInterface(*o2, filename.str().c_str(), 0, 0);
    }
  }

  return 0;
}