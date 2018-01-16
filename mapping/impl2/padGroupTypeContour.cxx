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

#include "padGroupTypeContour.h"

#include "polygon.h"
#include "contourCreator.h"
#include "padGroupType.h"
#include "boost/format.hpp"

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

std::vector<o2::mch::contour::Polygon<int>> computePads(const PadGroupType &pgt)
{
  std::vector<o2::mch::contour::Polygon<int>> pads;

  for (int ix = 0; ix < pgt.getNofPadsX(); ++ix) {
    for (int iy = 0; iy < pgt.getNofPadsY(); ++iy) {
      if (pgt.padIdByIndices(ix, iy) >= 0) {
        pads.push_back({{ix,     iy},
                        {ix + 1, iy},
                        {ix + 1, iy + 1},
                        {ix,     iy + 1},
                        {ix,     iy},
                       });
      }
    }
  }
  return pads;
}

o2::mch::contour::Polygon<int> computeContour(const PadGroupType &pgt)
{
  std::vector<o2::mch::contour::Polygon<int>> pads{computePads(pgt)};

  o2::mch::contour::Contour<int> contour = o2::mch::contour::createContour(pads);
  if (contour.size() != 1) {
    std::cout << "OUPS. Contour.size()=" << contour.size() << "\n";
    throw std::runtime_error("contour size should be 1 and is " + std::to_string(contour.size()));
  }
  return contour[0];
}


std::vector<o2::mch::contour::Polygon<int>> computeContours(const std::vector<PadGroupType> &padGroupTypes)
{
  std::vector<o2::mch::contour::Polygon<int>> contours;
  for (auto &pgt : padGroupTypes) {
    contours.push_back(computeContour(pgt));
  }
  return contours;
}


}}}}
