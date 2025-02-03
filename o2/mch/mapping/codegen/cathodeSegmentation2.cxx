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

#include "cathodeSegmentation2.h"
#include "boost/format.hpp"
#include "padGroup.h"
#include "padSize.h"
#include "cathodeSegmentationCommon.h"
#include "writer.h"
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <tuple>
#include <vector>

using rapidjson::Value;

namespace jsonmap
{
namespace codegen
{
namespace impl2
{

std::string codeForDetElemId2SegType(const Value& catsegs,
                                     const Value& detection_elements)
{
  std::stringstream impl;

  impl << R"(
int detElemId2SegType(int detElemId)
{)";

  impl << generateCodeForDetElemIdArray(detection_elements);
  impl << generateCodeForCatSegTypeArray(catsegs, detection_elements);

  impl << R"(
  int detElemIndex =
    std::distance(detElemIndexToDetElemId.begin(),
                  std::find(detElemIndexToDetElemId.begin(),
                            detElemIndexToDetElemId.end(), detElemId));
  return (detElemIndex>=0 && detElemIndex<156) 
           ? detElemIndexToSegType[detElemIndex] 
           : -1;
}
  )";
  return impl.str();
}

void generateCodeForDetElemId2SegType(const std::string& ns, const Value& segmentation, const Value& detection_elements)
{
  bool includeGuards{ true };
  bool standalone{ true };
  std::stringstream decl, impl;

  decl << mappingNamespaceBegin(ns);
  decl << "int detElemId2SegType(int detElemId);";
  decl << mappingNamespaceEnd(ns);

  impl << generateInclude({ "array", "algorithm" });
  impl << mappingNamespaceBegin(ns);
  impl << codeForDetElemId2SegType(segmentation, detection_elements);
  impl << mappingNamespaceEnd(ns);
  outputCode(decl.str(), impl.str(),
             "DetElemId2SegType",
             includeGuards, !standalone);
}

std::map<int, int> getManu2Berg(const Value& bergs, bool is80pins)
{
  auto berg2manu = getBerg2Manu(bergs, is80pins);
  std::map<int, int> manu2berg;
  for (auto p : berg2manu) {
    manu2berg[p.second] = p.first;
  }
  return manu2berg;
}

template <typename T>
void output(std::ostream& code, T& value)
{
  code << value;
}

template <>
void output(std::ostream& code, PadGroupType& pgt)
{

  code << "/* " << pgt.originalMotifTypeIdString << " */ ";
  code << "{" << pgt.nofPadsX() << "," << pgt.nofPadsY() << ",";
  auto indices = pgt.fastIndex();
  code << "{";
  for (auto i = 0; i < indices.size(); i++) {
    code << indices[i];
    if (i < indices.size() - 1) {
      code << ",";
    }
  }
  code << "}";
  code << "}";
}

template <typename T>
void output(std::ostream& code, std::vector<T>& v)
{
  code << "{";
  for (auto i = 0; i < v.size(); i++) {
    output(code, v[i]);
    if (i < v.size() - 1) {
      code << ",";
    }
    code << "\n";
  }
  code << "}";
}

std::string getCodeForCathodeSegmentationCtor(const std::vector<MotifPosition>& motifPositions,
                                              const std::vector<PadGroupType>& allPadGroupTypes,
                                              const std::vector<PadSize>& allPadSizes,
                                              const std::map<int, int>& manu2berg)
{
  // get the padgroups corresponding to the motif positions
  std::vector<PadGroup> padGroups = getPadGroups(motifPositions, allPadGroupTypes, manu2berg);

  // get only the padgrouptypes present in original padgroups
  std::vector<PadGroupType> pgts = getPadGroupTypes(padGroups, allPadGroupTypes);

  // get only the padsizes present in original padgroups
  auto padsizes{ getPadSizes(padGroups, allPadSizes) };

  std::vector<PadGroup> remappedGroups = remap(padGroups, pgts, padsizes);

  std::stringstream code;

  code << "\n/* PG */\n";
  output(code, remappedGroups);
  code << ",\n /* PGT */\n";
  output(code, pgts);
  code << ",\n /* PS */\n";
  output(code, padsizes);

  return code.str();
}

void generateCodeForCathodeSegmentationCreator(const std::string& ns, int segType, std::string codeForBendingCtor, std::string codeForNonBendingCtor)
{
  std::stringstream impl;

  impl << generateInclude({ "CathodeSegmentationCreator.h" });
  impl << mappingNamespaceBegin(ns);

  std::string creatorName{ "createSegType" + std::to_string(segType) };
  impl << "CathodeSegmentation* " << creatorName << "(bool isBendingPlane) {\n";

  impl << "  if (isBendingPlane) { \n";
  impl << "    return new CathodeSegmentation{\n" << segType << ",\ntrue," << codeForBendingCtor << "};\n}\n";
  impl << "  else {\n ";
  impl << "   return new CathodeSegmentation{\n" << segType << ",\nfalse," << codeForNonBendingCtor << "};\n}\n";
  impl << "}\n";

  auto registerName = "CathodeSegmentationCreatorRegisterC" + creatorName.substr(1);
  impl << "class " << registerName << "{ \n";
  impl << "  public:\n  " << registerName << "()\n {\n registerCathodeSegmentationCreator(" << segType << "," << creatorName
       << ");\n }\n";
  impl << "} a" << registerName << ";\n";

  impl << mappingNamespaceEnd(ns);
  outputCode("", impl.str(), "CathodeSegmentationCreatorForSegType" + std::to_string(segType), true, true);
}

bool checkPadGroupType(const PadGroupType& pgt)
{

  // check that each pad group type has only each pad id once ...

  auto t = pgt.channelId;
  std::sort(t.begin(), t.end());
  auto last = std::unique(t.begin(), t.end());
  t.erase(last, t.end());
  std::vector<int> a;
  std::copy_if(t.begin(), t.end(), std::back_inserter(a), [](int i) { return i >= 0; });
  if (a.size() != pgt.ix.size()) {
    std::cout << a.size() << " != " << pgt.ix.size() << " " << pgt.originalMotifTypeIdString;
    return false;
  }
  return true;
}

void generateCodeForCathodeSegmentations2(const std::string& ns, const Value& catsegs, const Value& motiftypes,
                                          const Value& jsonPadSizes,
                                          const Value& detection_elements,
                                          const Value& bergs)
{
  // generate code for the segmentation creators
  auto padGroupTypes = getPadGroupTypes(motiftypes, bergs);

  int n{ 0 };
  for (auto& pgt : padGroupTypes) {
    if (!checkPadGroupType(pgt)) {
      std::cout << " | " << n++ << "\n";
    }
  }

  auto padSizes{ jsonmap::codegen::getPadSizes(jsonPadSizes) };

  for (int segType = 0; segType < 21; ++segType) {
    std::map<int, int> manu2berg = getManu2Berg(bergs, segType < 2);
    std::array<std::string, 2> code;
    for (auto i = 0; i < 2; ++i) {
      bool isBendingPlane = (i == 0);
      auto mp = getMotifPositions(segType, isBendingPlane, catsegs, motiftypes, jsonPadSizes);
      code[i] = getCodeForCathodeSegmentationCtor(mp, padGroupTypes, padSizes, manu2berg);
    }
    generateCodeForCathodeSegmentationCreator(ns, segType, code[0], code[1]);
  }
}

void generateCodeForCathodeSegmentations(const std::string& ns,
                                         const Value& catsegs, const Value& motiftypes,
                                         const Value& padsizes,
                                         const Value& detection_elements,
                                         const Value& bergs)
{
  generateCodeForDetElemId2SegType(ns, catsegs, detection_elements);
  generateCodeForCathodeSegmentations2(ns, catsegs, motiftypes, padsizes, detection_elements, bergs);
}
} // namespace impl2
} // namespace codegen
} // namespace jsonmap

