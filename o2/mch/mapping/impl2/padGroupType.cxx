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


#include "padGroupType.h"
#include "boost/format.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

namespace {
int extent(const std::vector<int> &v)
{
  auto result = std::minmax_element(begin(v), end(v));
  return 1 + *result.second - *result.first;
}
}

std::vector<int> validIndices(const std::vector<int>& ids)
{
  std::vector<int> v;
  for (auto i = 0; i < ids.size(); i++) {
    if (ids[i] >= 0) {
      v.push_back(i);
    }
  }
  return v;
}

PadGroupType::PadGroupType(int nofPadsX, int nofPadsY, std::vector<int> ids)
  :
  mFastId{std::move(ids)},
  mFastIndices{validIndices(mFastId)},
  mNofPads{static_cast<int>(std::count_if(begin(mFastId), end(mFastId), [](int i) { return i >= 0; }))},
  mNofPadsX{nofPadsX},
  mNofPadsY{nofPadsY}
{
}

int PadGroupType::id(int index) const
{
  if (index >= 0 && index < mFastId.size()) {
    return mFastId[index];
  }
  return -1;
}

bool PadGroupType::hasPadById(int id) const
{
  return id != -1 && std::find(begin(mFastId), end(mFastId), id) != end(mFastId);
}

void dump(std::ostream &os, std::string msg, const std::vector<int> &v)
{
  os << boost::format("%4s ") % msg;
  for (auto value: v) {
    os << boost::format("%2d ") % value;
  }
  os << "\n";
}

std::ostream &operator<<(std::ostream &os, const PadGroupType &pgt)
{
  os << "n=" << pgt.getNofPads() << " nx=" << pgt.getNofPadsX() << " ny=" << pgt.getNofPadsY() << "\n";
  dump(os, "index", pgt.mFastId);
  return os;
}

std::vector<o2::mch::contour::Polygon<double>> computePads(const PadGroupType &pgt, double padSizeX, double padSizeY)
{
  constexpr double EXTRAPADSIZE = 1E-4; // artificially increase size of pads by a smidge (1 micron) to avoid gaps
  // between pads

  std::vector<o2::mch::contour::Polygon<double>> pads;

  for (int ix = 0; ix < pgt.getNofPadsX(); ++ix) {
    for (int iy = 0; iy < pgt.getNofPadsY(); ++iy) {
      if (pgt.id(ix, iy) >= 0) {

        double xmin = ix * padSizeX;
        double ymin = iy * padSizeY;
        double xmax = xmin + padSizeX;
        double ymax = ymin + padSizeY;

        // grow a bit to avoid gaps
        xmin -= EXTRAPADSIZE;
        ymin -= EXTRAPADSIZE;
        xmax += EXTRAPADSIZE;
        ymax += EXTRAPADSIZE;

        pads.emplace_back(o2::mch::contour::Polygon<double>({{xmin, ymin},
                                                             {xmax, ymin},
                                                             {xmax, ymax},
                                                             {xmin, ymax},
                                                             {xmin, ymin}})
        );
      }
    }
  }
  return pads;
}

}
}
}

}
