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

PadGroupType::PadGroupType(int nofPadsX, int nofPadsY, const std::vector<int> &ids)
  :
  mFastId{ids},
  mNofPads{static_cast<int>(std::count_if(begin(mFastId), end(mFastId), [](int i) { return i >= 0; }))},
  mNofPadsX{nofPadsX},
  mNofPadsY{nofPadsY}
{

}

int PadGroupType::getIndex(int ix, int iy) const
{
  return ix + iy * mNofPadsX;
}

int PadGroupType::padIdByIndices(int ix, int iy) const
{
  int index = getIndex(ix, iy);
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
  for (auto i = 0; i < v.size(); ++i) {
    os << boost::format("%2d ") % v[i];
  }
  os << "\n";
}

std::ostream &operator<<(std::ostream &os, const PadGroupType &pgt)
{
  os << "n=" << pgt.getNofPads() << " nx=" << pgt.getNofPadsX() << " ny=" << pgt.getNofPadsY() << "\n";
  dump(os, "index", pgt.mFastId);
  return os;
}

}
}
}

}
