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

/// get the list of pad sizes

#include "seg.h"
#include <vector>
#include "AliMpPlaneType.h"
#include "AliMpVSegmentation.h"
#include "AliMpVPadIterator.h"
#include "AliMpDDLStore.h"
#include "de.h"
#include <iostream>
#include <set>
#include "readmapping.h"

int main()
{
  AliMpDDLStore *ddlStore;
  AliMpSegmentation *mseg;
  readMapping(ddlStore,mseg);

  std::vector<int> deids = get_deids(ddlStore);
  std::vector<AliMpVSegmentation*> segs = get_segs(mseg,deids,AliMp::kBendingPlane);
  std::vector<AliMpVSegmentation*> nb = get_segs(mseg,deids,AliMp::kNonBendingPlane);
  segs.insert(segs.end(),nb.begin(),nb.end());

  int n = 0;
  using SIZEUNIT=float;
  std::set<SIZEUNIT> padSizeX, padSizeY;
  std::set<std::pair<SIZEUNIT,SIZEUNIT>> padSize;
  SIZEUNIT scale = 2;

  for ( const auto& s : segs ){
    std::unique_ptr<AliMpVPadIterator> padIterator(s->CreateIterator());
    padIterator->First();
    do {
      AliMpPad pad = padIterator->CurrentItem();
      auto x = static_cast<SIZEUNIT>(scale*pad.GetDimensionX());
      auto y = static_cast<SIZEUNIT>(scale*pad.GetDimensionY());
      padSizeX.insert(x);
      padSizeY.insert(y);
      if (x < 0) {
        s->Print();
        pad.Print();
      }
      padSize.insert(std::make_pair(x, y));
      ++n;
      padIterator->Next();
    }
    while (!padIterator->IsDone());
  }

  std::cout << "n=" << n << std::endl;
  std::cout << "number of x sizes = " << padSizeX.size() << " : " << std::endl;
  for (const auto& x: padSizeX) {
    std::cout << x << " ";
  }
  std::cout << std::endl << "number of y sizes = " << padSizeY.size() << " : " << std::endl;
  for (const auto& y: padSizeY) {
    std::cout << y << " ";
  }
  std::cout << std::endl << "number of x,y sizes = " << padSize.size() << " : " << std::endl;
  for (const auto& v: padSize) {
    std::cout << "(" << v.first << "," << v.second << ") " << std::endl;
  }
  std::cout << std::endl;
  std::cout << sizeof(double) << std::endl;
  std::cout << sizeof(float) << std::endl;
  std::cout << sizeof(int) << std::endl;
  std::cout << sizeof(int16_t) << std::endl;
  return 0;
}
