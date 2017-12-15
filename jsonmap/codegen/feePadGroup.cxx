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


#include "feePadGroup.h"
#include <vector>

using rapidjson::Value;

void convert(const Value &mp, const Value &mt)
{

}

//std::vector<FEEPadGroup>
void
getFEEPadGroups(int segtype, bool bending, const Value &segmentations, const Value &motiftypes,
                const Value &padsizes)
{
  const auto &seg = segmentations.GetArray()[segtype];
  const Value &plane = bending ? seg["bending"] : seg["non-bending"];
  auto mpArray = plane["motifpositions"].GetArray();
  auto mtArray = motiftypes.GetArray();
  auto psArray = padsizes.GetArray();

  for (auto &mp: mpArray) {

    int padSizeId{-1};
    int secondPadSizeId{-1};

    if (mp["padsize"].IsInt()) {
      padSizeId = mp["padsize"].GetInt();
    } else {
      assert(mp["padsize"].IsArray());
      padSizeId = mp["padsize"].GetArray()[0].GetInt();
      secondPadSizeId = mp["padsize"].GetArray()[1].GetInt();
    }

//    MotifPosition pos{
//      mp["fec"].GetInt(),
//      mp["motiftype"].GetInt(),
//      padSizeId,
//      static_cast<double>(mp["x"].GetDouble()),
//      static_cast<double>(mp["y"].GetDouble())
//    };

    if (secondPadSizeId >= 0) {
      std::vector<int> padnumbers;
      auto pn = mp["padsize"].GetArray()[2]["pads"].GetArray();
      for (int i = 0; i < pn.Size(); ++i) {
        padnumbers.push_back(pn[i].GetInt());
      }
//      pos.secondPadSize(
//        secondPadSizeId,
//        padnumbers
//      );
    }

    //convert(mp, mtArray[mp["motiftype"].GetInt()], pads);
  }
}

void generateCodeForFEEPadGroups(const Value &segmentations,
                                 const Value &motiftypes,
                                 const Value &padsizes,
                                 const Value &bergs)
{
  for (int segtype = 0; segtype < segmentations.GetArray().Size(); ++segtype) {
    for (auto bending: {true, false}) {

//      std::vector<FEEPadGroup> padGroups = getFEEPadGroups(segtype, bending, segmentations, motiftypes,
//                                                                    padsizes);

    }
  }
}


