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


#include "segmentation.h"
#include "codeWriter.h"
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>

using rapidjson::Value;

struct MotifPosition
{
  public:
    MotifPosition(int f, int m, int p, double x, double y) :
      mFECId(f), mMotifTypeId(m), mPadSizeId(p), mPositionX(x), mPositionY(y)
    {

    }

    MotifPosition(int f, int m, int p1, double x, double y,
                  int p2, std::vector<int> list) :
      mFECId(f), mMotifTypeId(m), mPadSizeId(p1), mPositionX(x), mPositionY(y),
      mSecondPadSizeId(p2), mPadNumbers(list)
    {

    }

    int FECId() const
    { return mFECId; }

    void secondPadSize(int p2,
                       std::vector<int> padnumbers_)
    {
      mSecondPadSizeId = p2;
      mPadNumbers = padnumbers_;
    }

    bool hasTwoPadSizes() const
    { return !mPadNumbers.empty(); }

    friend std::ostream &operator<<(std::ostream &os, const MotifPosition &position);

  private:
    int mFECId;
    int mMotifTypeId;
    int mPadSizeId;
    double mPositionX;
    double mPositionY;
    int mSecondPadSizeId;
    std::vector<int> mPadNumbers;
};

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
getMotifPositionArray(int index, const Value &segmentations, bool isBendingPlane)
{
  const auto &seg = segmentations.GetArray()[index];
  const Value &plane = isBendingPlane ? seg["bending"] : seg["non-bending"];
  return plane["motifpositions"].GetArray();
}


std::vector<MotifPosition>
getMotifPositions(int index, bool bending, const Value &segmentations, const Value &motiftypes,
                  const Value &padsizes)
{
  std::vector<MotifPosition> motifpositions;

  auto mpArray = getMotifPositionArray(index, segmentations, bending);
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

  // sort the motifpositions by fecId => is that needed at all as we have a fecid <-> index array ?
  std::sort(motifpositions.begin(), motifpositions.end(), [](const MotifPosition &mp1,
                                                             const MotifPosition &mp2) {
    return mp1.FECId() < mp2.FECId();
  });

  return motifpositions;
}

std::string bergToChannelFunctionName(int index)
{
  if (index < 2) {
    return "berg80ToManu";
  }
  return "berg100ToManu";
}

std::string generateCodeForOneSegmentation(int index, bool isBending,
                                           const Value &segmentations,
                                           const Value &motiftypes,
                                           const Value &padsizes)
{
  std::vector<MotifPosition> motifpositions = getMotifPositions(index, isBending, segmentations, motiftypes, padsizes);

  std::ostringstream code;

  std::string bendingString = (isBending ? "true" : "false");
  code << "template<>\n";
  code << "SegmentationImpl0<" << index << "," << bendingString
       << "," << motifpositions.size() << "," << bergToChannelFunctionName(index)
       << "," << (index ? "MotifPosition" : "MotifPositionTwoPadSizes")
       << ">::SegmentationImpl0(const MotifTypeArray& motifTypes)"
       << " : mId(" << index << "),mIsBendingPlane(" << bendingString << "), "
       << "mNofPads{0},"
       << "mMotifPositions{ {\n";

  int i{0};
  for (const auto &mp: motifpositions) {
    code << "  " << mp;
    if (i++ < motifpositions.size() - 1) { code << ",\n"; }
  }
  code << "}}\n";

  code << "{";

  code << "  populatePads(motifTypes);\n";
  code << "  createContours(motifTypes);\n";

  code << "}\n";

  return code.str();
}

std::string
generateCodeForSegmentationType(int index, const Value &segmentations, const Value &motiftypes,
                                const Value &padsizes)
{
  std::string code;
  for (auto bending: {true, false}) {
    code += generateCodeForOneSegmentation(index, bending, segmentations, motiftypes, padsizes);
  }
  return code;
}

std::string generateCodeForSegTypeArray(const Value &segmentations, const Value &detection_elements)
{
  std::ostringstream impl;

  impl << "namespace {";

  impl << "\n  std::array<int," << detection_elements.Size() << "> segtype{";

  for (int ide = 0; ide < detection_elements.GetArray().Size(); ++ide) {
    const auto &de = detection_elements.GetArray()[ide];
    for (int i = 0; i < segmentations.Size(); ++i) {
      const auto &seg = segmentations.GetArray()[i];
      if (!strcmp(seg["segtype"].GetString(), de["segtype"].GetString())) {
        impl << i;
        if (ide < detection_elements.GetArray().Size() - 1) { impl << ","; }
        break;
      }
    }
  }

  impl << "};\n}";

  return impl.str();
}

std::string generateCodeForDetectionElements(const rapidjson::Value &detection_elements)
{
  assert(detection_elements.IsArray());
  std::vector<int> deids;
  for (const auto &de: detection_elements.GetArray()) {
    assert(de.IsObject());
    assert(de["id"].IsInt());
    deids.push_back(de["id"].GetInt());
  }

  std::ostringstream impl;

  std::sort(deids.begin(), deids.end());

  impl << "std::vector<int> getDetElemIds() { \n";
  impl << "return {\n";
  for (auto i = 0; i < deids.size(); ++i) {
    impl << deids[i];
    if (i < deids.size() - 1) { impl << ","; }
  }
  impl << "};\n}\n";
  return impl.str();
}

std::string generateCodeForSegmentationFactory(const Value &segmentations, const Value &detection_elements)
{
  std::ostringstream impl;

  impl << generateInclude({"array", "iterator", "stdexcept", "vector"});

  impl << mappingNamespaceBegin();

  impl << generateCodeForDetectionElements(detection_elements);

  impl << generateCodeForSegTypeArray(segmentations, detection_elements);

  impl << R"(
  std::unique_ptr<SegmentationInterface> getSegmentationByType(int type, bool isBendingPlane) {
)";

  for (int i = 0; i < 21; ++i) {
    for (auto b : std::array<bool, 2>{true, false}) {
      auto n = getMotifPositionArray(i, segmentations, b).Size();
      impl << "    if (isBendingPlane==" << (b ? "true" : "false") << " && type==" << i << ") {\n";
      impl << "      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<" << i << ","
           << (b ? "true" : "false") << "," << n << "," << bergToChannelFunctionName(i) << "," <<
           (i ? "MotifPosition" : "MotifPositionTwoPadSizes")
           << ">{arrayOfMotifTypes}};\n";
      impl << "    };\n";
    }
  }

  impl
    << "  return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<-1,true,0,nullptr,MotifPosition>{arrayOfMotifTypes}};\n";
  impl << "}\n";
  impl << mappingNamespaceEnd();

  return impl.str();
}

std::string generateCodeForBerg2Manu(const Value &bergs, int id)
{
  std::ostringstream code;

  int npins = bergs[id]["id"].GetInt();

  code << "int berg" << npins << "ToManu(int berg) {\n";

  int i{0};
  for (const auto &b: bergs[id]["pins"].GetArray()) {
    std::string smanu = b["manu"].GetString();
    if (isdigit(smanu[0])) {
      code << "  if (berg==" << b["id"].GetInt() << ") return " << atoi(smanu.c_str()) << ";\n";
    }
    ++i;
  }

  code << R"(
  std::ostringstream msg;
  msg << "berg number " << berg << " is invalid";
  throw std::out_of_range(msg.str());
}
  )";

  return code.str();
}

void generateCodeForSegmentations(const Value &segmentations, const Value &motiftypes,
                                  const Value &padsizes,
                                  const Value &bergs,
                                  const Value &detection_elements)
{
  std::string decl;
  decl += "\n";
  decl += generateCodeForBerg2Manu(bergs, 0);
  decl += generateCodeForBerg2Manu(bergs, 1);
  decl += "\n";
  for (int i = 0; i < segmentations.GetArray().Size(); ++i) {
    decl += generateCodeForSegmentationType(i, segmentations, motiftypes, padsizes);
  }
  bool includeGuards{true};
  bool standalone{true};
  outputCode(decl, "", "genSegmentationImpl0", !includeGuards, standalone);
  outputCode("", generateCodeForSegmentationFactory(segmentations, detection_elements), "genSegmentationFactory",
             includeGuards, !standalone);
}

