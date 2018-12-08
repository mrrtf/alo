// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche


#include "goImplementation.h"

#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <array>
#include "padGroupType.h"
#include "motifPosition.h"
#include "padSize.h"
#include "padGroup.h"
#include "segmentation2.h"
#include <algorithm>

using namespace rapidjson;

namespace jsonmap {
namespace codegen {
namespace go {

void generateDetElemId2SegType(const rapidjson::Value& segmentations,
                                      const rapidjson::Value& detection_elements)
{
  std::ostringstream code;

  code << R"verbatim(package mapping

import (
    "fmt"
)

func detElemID2SegType(deid DEID) (int, error) {
    m := map[DEID]int{
)verbatim";

  for (int ide = 0; ide < detection_elements.GetArray().Size(); ++ide) {
    const auto &de = detection_elements.GetArray()[ide];
    for (int i = 0; i < segmentations.Size(); ++i) {
      const auto &seg = segmentations.GetArray()[i];
      if (!strcmp(seg["segtype"].GetString(), de["segtype"].GetString())) {
        code << "        " << de["id"].GetInt() << ": " << std::setw(2) << i << ",\n";
        break;
      }
    }
  }

code << R"verbatim(   }
    segType, ok := m[deid]
    if ok {
        return segType, nil
    }
    return -1, fmt.Errorf("detElemId %d is unknown to me", deid)
}
)verbatim";

  std::ofstream out("detelemid2segtype.go");

  out << code.str();

}

template <typename T>
void output(std::ostream& code, T& value)
{
  code << value;
}

template <typename T>
void output(std::ostream& code, std::vector<T>& v)
{
  code << "{";
  for (auto i = 0; i < v.size(); i++) {
    output(code, v[i]);
  }
  code << "}";
}

template <typename T>
void outputSlice(std::ostream& code, std::vector<T>& v, std::string sliceType);

template<>
void output(std::ostream &code, impl2::PadGroupType &pgt)
{
  code << "/* " << pgt.originalMotifTypeIdString << " */ ";
  code << "NewPadGroupType(" << pgt.nofPadsX() << "," << pgt.nofPadsY() << ",";
  auto indices = pgt.fastIndex();
  outputSlice(code,indices,"int");
  code << ")";
}

template <typename T>
void outputSlice(std::ostream& code, std::vector<T>& v, std::string sliceType)
{
  bool compact = (sliceType=="int");

  if (!compact) {
    code << std::string(12,' ');
  }
  code << "[]" << sliceType << "{";
  if (!compact) {
    code << "\n" << std::string(16, ' ');
  }
  for (auto i = 0; i < v.size(); i++) {
    output(code, v[i]);
    code << ",";
    if (!compact) {
      code << "\n";
      code << std::string(16,' ');
    }
  }
  code << "}";
}


std::string getCodeForSegmentationCtor(const std::vector<MotifPosition> &motifPositions,
                                       const std::vector<impl2::PadGroupType> &allPadGroupTypes,
                                       const std::vector<PadSize> &allPadSizes,
                                       const std::map<int, int> &manu2berg)
{
  // get the padgroups corresponding to the motif positions
  std::vector<impl2::PadGroup> padGroups = getPadGroups(motifPositions, allPadGroupTypes, manu2berg);

  // get only the padgrouptypes present in original padgroups
  std::vector<impl2::PadGroupType> pgts = getPadGroupTypes(padGroups, allPadGroupTypes);

  // get only the padsizes present in original padgroups
  auto padsizes{getPadSizes(padGroups, allPadSizes)};

  std::vector<impl2::PadGroup> remappedGroups = remap(padGroups, pgts, padsizes);

  std::stringstream code;

  code << "\n";
  outputSlice(code, remappedGroups,"padGroup");
  code << ",\n";
  outputSlice(code, pgts,"padGroupType");
  code << ",\n";
  outputSlice(code, padsizes,"padSize");

  return code.str();
}

void
generateCodeForSegmentationCreator(int segType, std::string codeForBendingCtor, std::string codeForNonBendingCtor)
{
  std::stringstream code;

  std::string creatorName{"createSegType" + std::to_string(segType)};
  code << "package impl4\n\n";

  code << "import \"github.com/aphecetche/pigiron/mapping\"\n";

  code << "type " << creatorName << " struct{}\n\n";

  code << "func (seg " << creatorName << ") Build(isBendingPlane bool, deid mapping.DEID) mapping.CathodeSegmentation {\n";

  code << "    if isBendingPlane { \n";
  code << "       return newCathodeSegmentation(deid," << segType << ", true," << codeForBendingCtor << ")\n";
  code << "    }\n";
  code << "    return newCathodeSegmentation(deid," << segType << ", false," << codeForNonBendingCtor << ")\n";
  code << "}\n\n";

  code << "func init() {\n";
  code << "    mapping.RegisterCathodeSegmentationBuilder(" << segType << ", " << creatorName << "{})\n";
  code << "}\n";

  std::ofstream out(creatorName + ".go");
  out << code.str();
}

bool checkPadGroupType(const impl2::PadGroupType& pgt) {

  // check that each pad group type has only each pad id once ...

  auto t = pgt.channelId;
  std::sort(t.begin(),t.end());
  auto last = std::unique(t.begin(),t.end());
  t.erase(last,t.end());
  std::vector<int> a;
  std::copy_if(t.begin(),t.end(),std::back_inserter(a),[](int i) { return i>=0; });
  if ( a.size() != pgt.ix.size() ) {
    std::cout << a.size() << " != " << pgt.ix.size() << " " << pgt.originalMotifTypeIdString;
    return false;
  }
  return true;
}

void generateCodeForSegmentations(const Value &segmentations, const Value &motiftypes,
                                   const Value &jsonPadSizes,
                                   const Value &detection_elements,
                                   const Value &bergs)
{
  // generate code for the segmentation creators
  auto padGroupTypes = impl2::getPadGroupTypes(motiftypes, bergs);

  int n{0};
  for (auto& pgt: padGroupTypes) {
    if (!checkPadGroupType(pgt)) {
      std::cout << " | " << n++ << "\n";
    }
  }

  auto padSizes{jsonmap::codegen::getPadSizes(jsonPadSizes)};

  for (int segType = 0; segType < 21; ++segType) {
    std::map<int, int> manu2berg = impl2::getManu2Berg(bergs, segType < 2);
    std::array<std::string, 2> code;
    for (auto i = 0; i < 2; ++i) {
      bool isBendingPlane = (i == 0);
      auto mp = getMotifPositions(segType, isBendingPlane, segmentations, motiftypes, jsonPadSizes);
      code[i] = getCodeForSegmentationCtor(mp, padGroupTypes, padSizes, manu2berg);
    }
    generateCodeForSegmentationCreator(segType, code[0], code[1]);
  }
}


} // namespace go
} // namespace codegen
} // namespace jsonmap
