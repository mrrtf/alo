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


#include "padsize.h"

#include "AliMpVPadIterator.h"
#include "AliMpVSegmentation.h"
#include "de.h"
#include "seg.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <cmath>

std::set<PadSize> get_padsizes(const AliMpVSegmentation& seg)
{
  std::set<PadSize> padsizes;
  std::unique_ptr<AliMpVPadIterator> padIterator(seg.CreateIterator());
  const double scale = 2.0;
  padIterator->First();
  do {
    AliMpPad pad = padIterator->CurrentItem();
    auto x = scale*pad.GetDimensionX();
    auto y = scale*pad.GetDimensionY();
    if (x < 0) {
      seg.Print();
      pad.Print();
    }
    padsizes.insert(PadSize{x,y});
    padIterator->Next();
  }
  while (!padIterator->IsDone());

  return padsizes;
}

std::vector<PadSize> get_padsizes(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg) {

  // return an array of pad sizes (pairs of size in x-direction, size in y-direction)
  std::vector<int> deids = get_deids(ddlStore);
  std::vector<AliMpVSegmentation*> segs = get_segs(mseg,deids,AliMp::kBendingPlane);
  std::vector<AliMpVSegmentation*> nb = get_segs(mseg,deids,AliMp::kNonBendingPlane);
  segs.insert(segs.end(),nb.begin(),nb.end());

  std::vector<PadSize> padsizes;
  std::set<PadSize> padSizeSet;

  for ( const auto& s : segs ) {
    auto ps = get_padsizes(*s);
    padSizeSet.insert(ps.begin(),ps.end());
  }

  padsizes.insert(padsizes.end(),padSizeSet.begin(),padSizeSet.end());

  // sort the pad sizes per pad area
  std::sort(std::begin(padsizes),std::end(padsizes));

  return padsizes;
}

bool operator<(const PadSize &lhs, const PadSize &rhs)
{
  auto lhsArea = lhs.x*lhs.y;
  auto rhsArea = rhs.x*rhs.y;

  if ( lhsArea < rhsArea ) {
    return true;
  }
  if ( rhsArea < lhsArea ) {
    return false;
  }
  return lhs.y < rhs.y;
}

bool operator>(const PadSize &lhs, const PadSize &rhs)
{
  return rhs < lhs;
}

bool operator<=(const PadSize &lhs, const PadSize &rhs)
{
  return !(rhs < lhs);
}

bool operator>=(const PadSize &lhs, const PadSize &rhs)
{
  return !(lhs < rhs);
}

bool operator==(const PadSize &lhs, const PadSize &rhs)
{
  return std::fabs(lhs.x - rhs.x) < 1E-4 && std::fabs(lhs.y- rhs.y) < 1E-4;
}

bool operator!=(const PadSize &lhs, const PadSize &rhs)
{
  return !(rhs == lhs);
}
