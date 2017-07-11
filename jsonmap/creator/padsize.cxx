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
#include <vector>
#include <utility>
#include "seg.h"
#include <set>
#include "de.h"
#include "AliMpVPadIterator.h"
#include "AliMpVSegmentation.h"
#include <iostream>

std::vector<std::pair<float,float>> get_padsizes(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg) {

  std::vector<int> deids = get_deids(ddlStore);
  std::vector<AliMpVSegmentation*> segs = get_segs(mseg,deids,AliMp::kBendingPlane);
  std::vector<AliMpVSegmentation*> nb = get_segs(mseg,deids,AliMp::kNonBendingPlane);
  segs.insert(segs.end(),nb.begin(),nb.end());

  //FIXME: should not really use set with floats, or should define what comparison
  //means for those really...
  int n = 0;
  std::set<float> padSizeX, padSizeY;
  std::set<std::pair<float,float>> padSize;
  float scale = 2;

  for ( const auto& s : segs ){
    std::unique_ptr<AliMpVPadIterator> padIterator(s->CreateIterator());
    padIterator->First();
    do {
      AliMpPad pad = padIterator->CurrentItem();
      auto x = static_cast<float>(scale*pad.GetDimensionX());
      auto y = static_cast<float>(scale*pad.GetDimensionY());
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

  std::vector<std::pair<float,float>> padsizes;

  padsizes.insert(padsizes.end(),padSize.begin(),padSize.end());

  return padsizes;
}
