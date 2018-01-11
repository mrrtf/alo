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

PadGroupType::PadGroupType(const std::vector<int> &ids, const std::vector<int> &ix, const std::vector<int> &iy)
  : mId(ids), mIx(ix), mIy(iy)
{
  if (mId.size() != mIx.size() || mId.size() != mIy.size() || mIx.size() != mIy.size()) {
    throw std::out_of_range("input vectors should be the same size");
  }

  mNofPadsX = extent(mIx);
  int ixmax = std::distance(begin(mIx), std::max(begin(mIx), end(mIx)));
  int iymax = std::distance(begin(mIy), std::max(begin(mIy), end(mIy)));

  // generate a quick-index ix + iy*mNofPadsX -> index in other arrays
  mIndex.resize(getIndex(ixmax, iymax), -1);
  for (auto i = 0; i < mIx.size(); ++i) {
    mIndex[getIndex(mIx[i], mIy[i])] = i;
  }
}

int PadGroupType::getIndex(int ix, int iy) const
{
  return ix + iy * mNofPadsX;
}

int PadGroupType::getNofPadsY() const
{
  return extent(mIy);
}


int PadGroupType::padIdByIndices(int ix, int iy) const
{
  return mId[getIndex(ix, iy)];
}

bool PadGroupType::hasPadById(int id) const
{
  return std::find(begin(mId),end(mId),id) != end(mId);
}

void dump(std::ostream& os, std::string msg, const std::vector<int>& v)
{
  os << boost::format("%4s ") % msg;
  for (auto i = 0; i < v.size(); ++i )
  {
    os << boost::format("%2d ") % v[i];
  }
  os << "\n";
}

std::ostream& operator<<(std::ostream &os, const PadGroupType &pgt)
{
  os << "n=" << pgt.getNofPads() << " nx=" << pgt.getNofPadsX() << " ny=" << pgt.getNofPadsY() << "\n";

  dump(os,"id",pgt.mId);
  dump(os,"ix",pgt.mIx);
  dump(os,"iy",pgt.mIy);

  return os;
}

}
}
}

}
