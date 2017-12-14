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

#include "motifPosition.h"
#include "codeWriter.h"
#include "rapidjson/document.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <boost/format.hpp>

using namespace rapidjson;

std::ostream &operator<<(std::ostream &os, const MotifPosition &position)
{
  os << "{" << position.mFECId << "," << position.mMotifTypeId << ","
     << position.mPadSizeId << "," << std::setprecision(10) << position.mPositionX << "," << position.mPositionY;
  if (position.hasTwoPadSizes()) {
    os << "," << position.mSecondPadSizeId << ", {";
    for (auto i = 0; i < position.mPadNumbers.size(); ++i) {
      os << position.mPadNumbers[i];
      if (i < position.mPadNumbers.size() - 1) {
        os << ",";
      }
    }
    os << "}";
  }
  os << "}";
  return os;
}


auto
getMotifPositionArray(int segtype, const Value &segmentations, bool isBendingPlane)
{
  const auto &seg = segmentations.GetArray()[segtype];
  const Value &plane = isBendingPlane ? seg["bending"] : seg["non-bending"];
  return plane["motifpositions"].GetArray();
}


std::vector<MotifPosition>
getMotifPositions(int segtype, bool bending, const Value &segmentations, const Value &motiftypes,
                  const Value &padsizes)
{
  std::vector<MotifPosition> motifpositions;

  auto mpArray = getMotifPositionArray(segtype, segmentations, bending);
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

    MotifPosition pos{
      mp["fec"].GetInt(),
      mp["motiftype"].GetInt(),
      padSizeId,
      static_cast<double>(mp["x"].GetDouble()),
      static_cast<double>(mp["y"].GetDouble())
    };

    if (secondPadSizeId >= 0) {
      std::vector<int> padnumbers;
      auto pn = mp["padsize"].GetArray()[2]["pads"].GetArray();
      for (int i = 0; i < pn.Size(); ++i) {
        padnumbers.push_back(pn[i].GetInt());
      }
      pos.secondPadSize(
        secondPadSizeId,
        padnumbers
      );
    }

    motifpositions.push_back(pos);
  }

  // sort the motifpositions by fecId => is that needed at all as we have a fecid <-> segtype array ?
  std::sort(motifpositions.begin(), motifpositions.end(), [](const MotifPosition &mp1,
                                                             const MotifPosition &mp2) {
    return mp1.FECId() < mp2.FECId();
  });

  return motifpositions;
}


std::pair<std::string,std::string> generateCodeForBerg2Manu(const Value &bergs, int id)
{
  std::ostringstream decl;
  std::ostringstream header;
  std::ostringstream impl;

  int npins = bergs[id]["id"].GetInt();

  decl << "int berg" << npins << "ToManu(int berg)";

  header << decl.str() << ";\n";

  impl << decl.str() << "{\n";

  int i{0};
  for (const auto &b: bergs[id]["pins"].GetArray()) {
    std::string smanu = b["manu"].GetString();
    if (isdigit(smanu[0])) {
      impl << "  if (berg==" << b["id"].GetInt() << ") return " << atoi(smanu.c_str()) << ";\n";
    }
    ++i;
  }

  impl << R"(
  std::ostringstream msg;
  msg << "berg number " << berg << " is invalid";
  throw std::out_of_range(msg.str());
}
)";

  return {header.str(),impl.str()};
}

std::pair<std::string,std::string> generateCodeForGetMotifPosition(int segtype, bool isBending,
                                            const std::vector<MotifPosition> &motifPositions)
{
  std::ostringstream decl;
  std::ostringstream header;
  std::ostringstream impl;

  std::string mpClassName{(segtype ? "MotifPosition" : "MotifPositionTwoPadSizes")};

  decl << "template<>\n";
  decl << "std::array<" << mpClassName << "," << motifPositions.size()
       << "> getMotifPositions<"
       << segtype << ","
       << (isBending ? "true" : "false")
       << ">()";

  header << decl.str() << ";\n";

  impl << decl.str() << "{\n return {\n";

  int i{0};
  for (const auto &mp: motifPositions) {
    impl << "    " << mpClassName << " " << mp;
    if (i++ < motifPositions.size() - 1) { impl << ",\n"; }
  }
  impl << "  };\n}\n";

  return { header.str(), impl.str()};
}

std::string generateCodeForMotifPositionTrait(int segtype, bool bending, int size)
{
  std::ostringstream decl;
  decl << "template<> struct MotifPositionTrait<" << segtype
       << "," << bending << "> : MotifPositionBaseTrait<" << size << "," <<
       (segtype ? "MotifPosition" : "MotifPositionTwoPadSizes") << "," <<
       (segtype < 2 ? "berg80ToManu" : "berg100ToManu") << "> {};\n";
  return decl.str();
}

void generateCodeForMotifPositions(const Value &segmentations,
                                   const Value &motiftypes,
                                   const Value &padsizes,
                                   const Value &bergs)
{
  std::ostringstream decl;
  std::ostringstream impl;

  impl << mappingNamespaceBegin();
  impl << generateInclude({"sstream"});

  decl << generateInclude({"array"});

  for ( auto i : { 0, 1} ) {
    auto p = generateCodeForBerg2Manu(bergs, i);
    decl << p.first;
    impl << p.second;
  }

  // then one trait and one template specialization per segmentation type
  for (int segtype = 0; segtype < segmentations.GetArray().Size(); ++segtype) {
    for (auto bending: {true, false}) {
      std::vector<MotifPosition> motifpositions = getMotifPositions(segtype, bending, segmentations, motiftypes,
                                                                    padsizes);
      decl << generateCodeForMotifPositionTrait(segtype, bending, motifpositions.size());
      auto code = generateCodeForGetMotifPosition(segtype, bending, motifpositions);
      decl << code.first;
      impl << code.second;
    }
  }

  impl << mappingNamespaceEnd();

  bool includeGuards{false};
  bool standalone{false};
  outputCode(decl.str(), impl.str(), "genMotifPosition", includeGuards, standalone,"motifPosition.h");
}

