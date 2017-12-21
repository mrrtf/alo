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

#include "segmentation1.h"
#include "writer.h"
#include "motifPosition.h"
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <set>
#include <map>

using rapidjson::Value;

namespace jsonmap {
namespace codegen {
namespace impl2 {

void reportIndicesRange(const Value &motiftypes);

void reportNonRegularMotifs(const Value &segmentations, const Value &motiftypes, const Value &padsizes);

void reportNonRegularMotifs(const std::vector<MotifPosition> &motifpositions);

void generateCodeForSegmentations(const Value &segmentations, const Value &motiftypes,
                                  const Value &padsizes,
                                  const Value &detection_elements)
{
  //reportNonRegularMotifs(segmentations, motiftypes, padsizes);
  //reportIndicesRange(motiftypes);
}

void reportNonRegularMotifs(const std::vector<MotifPosition> &motifpositions,
                            std::set<int> &regulars, std::map<int, int> &nonreg, std::set<int> &all)
{
  for (auto &mp: motifpositions) {
    all.insert(mp.mMotifTypeId);
    if (mp.hasTwoPadSizes()) {
      nonreg[mp.mMotifTypeId]++;
    } else {
      regulars.insert(mp.mMotifTypeId);
    }
  }
}

void reportNonRegularMotifs(const Value &segmentations, const Value &motiftypes, const Value &padsizes)
{
  std::map<int, int> nonreg;
  std::set<int> regulars;
  std::set<int> all;
  for (auto index = 0; index < segmentations.GetArray().Size(); ++index) {
    for (auto isBendingPlane: {true, false}) {
      std::vector<MotifPosition> motifpositions = getMotifPositions(index, isBendingPlane, segmentations, motiftypes,
                                                                    padsizes);
      reportNonRegularMotifs(motifpositions, regulars, nonreg, all);
    }
  }

  for (auto p: nonreg) {
    std::cout << p.first << "->" << p.second;
    auto found = std::find(regulars.begin(), regulars.end(), p.first) != regulars.end();
    if (found) {
      std::cout << " also in regular";
    }
    std::cout << "\n";
  }

  std::cout << all.size() << " motifs : " << nonreg.size() << " non regulars and " << regulars.size()
            << " regulars motifs\n";

}

void reportIndicesRange(const Value &motiftypes)
{
  int ixmax{0};
  int iymax{0};
  int maxproduct{0};
  const auto &mts = motiftypes.GetArray();
  for (auto i = 0; i < mts.Size(); ++i) {
    const Value &pads = mts[i]["pads"];
    int x{0};
    int y{0};
    for (const auto &p: pads.GetArray()) {
      int ix = p["ix"].GetInt();
      int iy = p["iy"].GetInt();
      ixmax = std::max(ixmax, ix);
      iymax = std::max(iymax, iy);
      maxproduct = std::max(maxproduct, ix * iy);
      x = std::max(x, ix);
      y = std::max(y, iy);
    }
  }

  std::cout << "(ix,iy)max = " << ixmax << "," << iymax << " ix*iy max = " << maxproduct << "\n";
}

}
}
}

