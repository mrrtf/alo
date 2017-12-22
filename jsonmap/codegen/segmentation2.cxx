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
#include "padGroupType.h"
#include "segmentationCommon.h"
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <set>
#include <map>
#include <tuple>

using rapidjson::Value;

namespace jsonmap {
namespace codegen {
namespace impl2 {

int getPadSizeId(const MotifPosition &mp, const PadGroupType &pgt, int channel,
                const std::map<int, int> &manu2berg)
{
  if (mp.hasTwoPadSizes()) {
    int berg = manu2berg.find(channel)->second;
    auto it = std::find(mp.mPadNumbers.begin(), mp.mPadNumbers.end(), berg);
    if (it != mp.mPadNumbers.end()) {
      return mp.mSecondPadSizeId;
    }
  }
  return mp.mPadSizeId;
}

void getPosition(const MotifPosition &mp, const PadGroupType &pgt, double &x, double &y)
{
  // once again, don't fret about some hard-coded stuff, that's faster sometimes ;-)

  x = mp.mPositionX;
  y = mp.mPositionY;

  using shifts = std::tuple<std::string, int, double, double>;

  std::vector<shifts> special = {{"1BG", 16, 2.52 * 3, 0.0},
                                 {"1NG", 16, 0.0,      1.68 * 3},
                                 {"1NH", 12, 0.0,      1.68 * 3},
                                 {"E14", 7,  1 * 2.5,  10 * 0.5},
                                 {"E15", 4,  1 * 2.5,  5 * 0.5}};

  for (auto &s : special) {
    if (pgt.originalMotifTypeIdString == std::get<0>(s) &&
        pgt.channelId.size() == std::get<1>(s)) {
      x += std::get<2>(s);
      y += std::get<3>(s);
    }
  }
}

std::string
padgroups(const std::vector<MotifPosition> &motifPositions, const std::vector<PadGroupType> &padGroupTypes,
          int nonRegIndexLimit, std::string space, const std::map<int,int>& manu2berg)
{
  std::stringstream code;

  code << "{\n";

  for (auto i = 0; i < motifPositions.size(); ++i) {
    const auto &mp = motifPositions[i];
    int motifTypeId = mp.mMotifTypeId;
    for (auto &pgt : padGroupTypes) {
      if (pgt.originalMotifTypeId == motifTypeId) {
        int padSizeId{getPadSizeId(mp, pgt,pgt.channelId[0],manu2berg)};
        double x, y;
        getPosition(mp, pgt, x, y);
        // shoud update padSizeId, x,y for split pad groups
        if (pgt.id >= nonRegIndexLimit) {
          code << ", ";
        } else {
          code << space;
        }
        code << "{" << mp.mFECId << "," << pgt.id << "," << padSizeId << "," << x << "," << y << "}";
      }
    }
    if (i < motifPositions.size() - 1) {
      code << ",";
    }
    code << "\n";
  }

  code << "   }";

  return code.str();
}

void generateCodeForSegmentations(const Value &segmentations, const Value &motiftypes,
                                  const Value &padsizes,
                                  const Value &detection_elements,
                                  const Value &bergs)
{
  std::stringstream impl;

  impl << generateInclude({"array", "stdexcept", "vector", "segmentation.h"});
  impl << mappingNamespaceBegin("impl2");
  impl << generateCodeForDetElemIdArray(detection_elements);
  impl << generateCodeForSegTypeArray(segmentations, detection_elements);

  impl << R"(
Segmentation* createSegmentation(int detElemId, bool isBendingPlane)
{
  int detElemIndex = std::distance(detElemIndexToDetElemId.begin(),
                                   std::find(detElemIndexToDetElemId.begin(), detElemIndexToDetElemId.end(),
                                             detElemId));
  int segType = detElemIndexToSegType[detElemIndex];

 )";

  int nonRegIndexLimit = motiftypes.GetArray().Size();
  auto padGroupTypes = getPadGroupTypes(motiftypes, bergs);

  for (int segType = 0; segType < 21; ++segType) {

    auto berg2manu = getBerg2Manu(bergs,segType<2 ? 80:100);
    std::map<int,int> manu2berg;
    for ( auto p: berg2manu) {
      manu2berg[p.second]=p.first;
    }

    for (auto isBendingPlane : {true, false}) {
      std::vector<MotifPosition> motifPositions = getMotifPositions(segType, isBendingPlane, segmentations, motiftypes,
                                                                    padsizes);
      impl << " if (isBendingPlane==" << (isBendingPlane ? "true" : "false") << " && segType==" << segType
           << ") {\n";
      impl << "   return new Segmentation{" << segType << ","
           << (isBendingPlane ? "true" : "false") << "," <<
           padgroups(motifPositions, padGroupTypes, nonRegIndexLimit, std::string(5, ' '),manu2berg) << "};\n";
      impl << " };\n";
    }
  }

  impl
    << "  throw std::out_of_range(std::to_string(segType) + \" is not a valid segtype\");\n";
  impl << "\n}\n";
  impl << mappingNamespaceEnd("impl2");

  bool includeGuards{true};
  bool standalone{true};
  outputCode("", impl.str(), "impl2-genSegmentation",
             includeGuards, !standalone);
  // #include "report.h"
  //reportNonRegularMotifs(segmentations, motiftypes, padsizes);
  //reportIndicesRange(motiftypes);
}


}
}
}

