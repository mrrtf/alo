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

#include "segmentation2.h"
#include "boost/format.hpp"
#include "motifPosition.h"
#include "padGroup.h"
#include "padGroupType.h"
#include "segmentationCommon.h"
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

namespace jsonmap {
namespace codegen {
namespace impl2 {

std::string
padgroups(const std::vector<MotifPosition> &motifPositions, const std::vector<PadGroupType> &padGroupTypes,
          int nonRegIndexLimit, std::string space, const std::map<int, int> &manu2berg)
{
  auto pgs = getPadGroups(motifPositions, padGroupTypes, manu2berg);

  std::stringstream code;

  code << "{\n";

  for (auto i = 0; i < pgs.size(); ++i) {
    code << pgs[i];
    if (i < pgs.size() - 1) {
      code << ",";
    }
    code << "\n";
  }

  code << "   }";

  return code.str();
}

std::string codeForDetElemId2SegType(const Value &segmentations,
                                     const Value &detection_elements)
{
  std::stringstream impl;

  impl << R"(
int detElemId2SegType(int detElemId)
{)";

  impl << generateCodeForDetElemIdArray(detection_elements);
  impl << generateCodeForSegTypeArray(segmentations, detection_elements);

  impl << R"(
  int detElemIndex = std::distance(detElemIndexToDetElemId.begin(),
                                   std::find(detElemIndexToDetElemId.begin(), detElemIndexToDetElemId.end(),
                                             detElemId));
  return (detElemIndex>=0 && detElemIndex<156) ? detElemIndexToSegType[detElemIndex] : -1;
}
  )";
  return impl.str();
}

void generateCodeForDetElemId2SegType(const Value &segmentation, const Value &detection_elements)
{
  bool includeGuards{true};
  bool standalone{true};
  std::stringstream decl, impl;

  decl << mappingNamespaceBegin("impl2");
  decl << "int detElemId2SegType(int detElemId);";
  decl << mappingNamespaceEnd("impl2");

  impl << generateInclude({"array"});
  impl << mappingNamespaceBegin("impl2");
  impl << codeForDetElemId2SegType(segmentation, detection_elements);
  impl << mappingNamespaceEnd("impl2");
  outputCode(decl.str(), impl.str(),
             "genDetElemId2SegType",
             includeGuards, !standalone);

}

std::map<int, int> getManu2Berg(const Value &bergs, bool is80pins)
{
  auto berg2manu = getBerg2Manu(bergs, is80pins);
  std::map<int, int> manu2berg;
  for (auto p: berg2manu) {
    manu2berg[p.second] = p.first;
  }
  return manu2berg;
}

void generateCodeForSegmentations1(const Value &segmentations, const Value &motiftypes,
                                   const Value &padsizes,
                                   const Value &detection_elements,
                                   const Value &bergs)
{
  std::stringstream impl;

  impl << generateInclude(
    {
      "array", "stdexcept", "vector", "segmentation.h", "segmentationImpl2.h", "segmentationCreator.h",
      "genDetElemId2SegType.h"
    });
  impl << mappingNamespaceBegin("impl2");

  impl << R"(

  Segmentation* createSegmentation(int detElemId, bool isBendingPlane)
  {
    int segType = detElemId2SegType(detElemId);
    )";

  int nonRegIndexLimit = motiftypes.GetArray().Size();
  auto padGroupTypes = getPadGroupTypes(motiftypes, bergs);

  for (int segType = 0; segType < 21; ++segType) {

    std::map<int, int> manu2berg = getManu2Berg(bergs, segType < 2);

    for (auto isBendingPlane : {true, false}) {
      std::vector<MotifPosition> motifPositions = getMotifPositions(segType, isBendingPlane, segmentations, motiftypes,
                                                                    padsizes);
      impl << " if (isBendingPlane==" << (isBendingPlane ? "true" : "false") << " && segType==" << segType
           << ") {\n";
      impl << "   return new Segmentation{" << segType << ","
           << (isBendingPlane ? "true" : "false") << "," <<
           padgroups(motifPositions, padGroupTypes, nonRegIndexLimit, std::string(5, ' '), manu2berg) << "};\n";
      impl << " };\n";
    }
  }

  impl
    << "  throw std::runtime_error(std::to_string(segType) + \" is not a valid segtype\");\n";
  impl << "\n}\n";
  impl << mappingNamespaceEnd("impl2");

  bool includeGuards{true};
  bool standalone{true};
  outputCode("", impl.str(), "genSegmentation",
             includeGuards, !standalone);
}

std::string codeForSegmentationCtor()
{
  return "{},{},{}";
}

void
generateCodeForSegmentationCreator(int segType, std::vector<PadGroupType> &padGroupTypes, const Value &segmentations,
                                   const Value &motiftypes,
                                   const Value &padsizes,
                                   const Value &bergs)
{
  std::stringstream impl;

  impl << generateInclude({"segmentationCreator.h"});
  impl << mappingNamespaceBegin("impl2");

  std::string creatorName{"createSegType" + std::to_string(segType)};
  impl << "Segmentation* " << creatorName << "(bool isBendingPlane) {\n";
  int nonRegIndexLimit = motiftypes.GetArray().Size();

  std::map<int, int> manu2berg = getManu2Berg(bergs, segType < 2);

  std::vector<MotifPosition> mpBending = getMotifPositions(segType, true, segmentations, motiftypes,
                                                           padsizes);
  std::vector<MotifPosition> mpNonBending = getMotifPositions(segType, false, segmentations, motiftypes,
                                                              padsizes);

  impl << " if (isBendingPlane) { \n";
  impl << "   return new Segmentation{" << segType << ",true," << codeForSegmentationCtor() << "};\n}\n";
  impl << " return new Segmentation{" << segType << ",true," << codeForSegmentationCtor();
  impl << " };\n}\n";

  auto registerName = "SegmentationCreatorRegisterC" + creatorName.substr(1);
  impl << "class " << registerName << "{ \n";
  impl << "  public:\n  " << registerName << "() { registerSegmentationCreator(" << segType << "," << creatorName
       << "); }\n";
  impl << "} a" << registerName << ";\n";

  impl << mappingNamespaceEnd("impl2");
  outputCode("", impl.

               str(),

             "genSegmentationCreatorForSegType" +
             std::to_string(segType),
             true, true);
}

void generateCodeForSegmentations2(const Value &segmentations, const Value &motiftypes,
                                   const Value &padsizes,
                                   const Value &detection_elements,
                                   const Value &bergs)
{
  std::stringstream impl;

  // first generate code for the "driver" which dispatch to the relevant segmentation creator...

  impl << generateInclude(
    {
      "array", "stdexcept", "vector", "segmentation.h", "segmentationImpl2.h", "segmentationCreator.h",
      "genDetElemId2SegType.h"
    });
  impl << mappingNamespaceBegin("impl2");

  impl << R"(

  Segmentation* createSegmentation2(int detElemId, bool isBendingPlane)
  {
    int segType = detElemId2SegType(detElemId);
    SegmentationCreator creator = getSegmentationCreator(segType);
    if (!creator) {
       return nullptr;
    }
    return creator(isBendingPlane);
   }
 )";

  impl << mappingNamespaceEnd("impl2");

  bool includeGuards{true};
  bool standalone{true};
  outputCode("", impl.str(), "genSegmentation2",
             includeGuards, !standalone);

  // ... then generate code for the segmentation creators themselves
  auto padGroupTypes = getPadGroupTypes(motiftypes, bergs);
  for (int segType = 0; segType < 21; ++segType) {
    generateCodeForSegmentationCreator(segType, padGroupTypes, segmentations, motiftypes, padsizes, bergs);
  }

}

void generateCodeForSegmentations(const Value &segmentations, const Value &motiftypes,
                                  const Value &padsizes,
                                  const Value &detection_elements,
                                  const Value &bergs)
{
  generateCodeForDetElemId2SegType(segmentations, detection_elements);
  generateCodeForSegmentations1(segmentations, motiftypes, padsizes, detection_elements, bergs);
  generateCodeForSegmentations2(segmentations, motiftypes, padsizes, detection_elements, bergs);
}
}
}
}

