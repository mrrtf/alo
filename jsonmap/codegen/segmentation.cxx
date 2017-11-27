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
    MotifPosition(int f, int m, double padsizex, double padsizey, double x, double y) :
      mFECId(f), mMotifTypeId(m), mPadSizeX(padsizex), mPadSizeY(padsizey), mPositionX(x), mPositionY(y)
    {

    }

    MotifPosition(int f, int m, double padsizex, double padsizey, double x, double y,
                  double x2, double y2, std::vector<int> list) :
      mFECId(f), mMotifTypeId(m), mPadSizeX(padsizex), mPadSizeY(padsizey), mPositionX(x), mPositionY(y),
      mSecondPadSizeX(x2), mSecondPadSizeY(y2), mPadNumbers(list)
    {

    }

    int FECId() const
    { return mFECId; }

    void secondPadSize(double x2_, double y2_,
                       std::vector<int> padnumbers_)
    {
      mSecondPadSizeX = x2_;
      mSecondPadSizeY = y2_;
      mPadNumbers = padnumbers_;
    }

    bool hasTwoPadSizes() const
    { return !mPadNumbers.empty(); }

    friend std::ostream &operator<<(std::ostream &os, const MotifPosition &position);

  private:
    int mFECId;
    int mMotifTypeId;
    double mPadSizeX;
    double mPadSizeY;
    double mPositionX;
    double mPositionY;
    double mSecondPadSizeX;
    double mSecondPadSizeY;
    std::vector<int> mPadNumbers;
};

std::ostream &operator<<(std::ostream &os, const MotifPosition &position)
{
  os << "{" << position.mFECId << "," << position.mMotifTypeId << "," << position.mPadSizeX << ","
     << position.mPadSizeY << "," << std::setprecision(10) << position.mPositionX << "," << position.mPositionY;
  if (position.hasTwoPadSizes()) {
    os << "," << position.mSecondPadSizeX << "," << position.mSecondPadSizeY << ", {";
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
      static_cast<double>(psArray[padSizeId]["x"].GetDouble()),
      static_cast<double>(psArray[padSizeId]["y"].GetDouble()),
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
        psArray[secondPadSizeId]["x"].GetDouble(),
        psArray[secondPadSizeId]["y"].GetDouble(),
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
       << "," << (index ? "MotifPosition":"MotifPositionTwoPadSizes")
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

std::pair<std::string, std::string>
generateCodeForSegmentationInterface()
{
  std::ostringstream decl;
  std::ostringstream impl;

  decl << R"(
#include "contour.h"
)";

  decl << mappingNamespaceBegin();

  decl << R"(
class SegmentationInterface {
  public:
    virtual bool isBendingPlane() const = 0;
    virtual int getId() const = 0;
    virtual int nofDualSampas() const = 0;
    virtual int nofPads() const = 0;
    virtual bool hasPadByPosition(double x, double y) const = 0;
    virtual bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const = 0;
    virtual o2::mch::contour::Contour<double> getEnvelop() const = 0;
    virtual std::vector<o2::mch::contour::Contour<double>> getSampaContours() const = 0;
    virtual o2::mch::contour::Contour<double> getSampaPads(int dualSampaId) const = 0;
};
)";

  decl << mappingNamespaceEnd();
  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
}

std::string
generateCodeForSegmentationCommon()
{
  std::ostringstream decl;

  decl << R"(
#include <array>
#include "genMotifType.h"
#include <tuple>
#include "genSegmentationInterface.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <ostream>
#include "contourCreator.h"
#include "motifPosition.h"
#include "motifPositionTwoPadSizes.h"
#include "pad.h"
)";

  decl << mappingNamespaceBegin();

  decl << R"***(
template<int SEGID, bool BENDINGPLANE, int NFEC, int (*berg2channel)(int), typename MOTIFPOSITION>
class SegmentationImpl0 : public SegmentationInterface
{
  public:
    SegmentationImpl0(const MotifTypeArray& motifTypes)
    { throw std::out_of_range("Invalid segmentation initialization. Only specific specializations are allowed."); }

    int getId() const override
    { return mId; }

    bool isBendingPlane() const override
    { return mIsBendingPlane; }

    int nofDualSampas() const override
    { return NFEC; }

    int nofPads() const override
    {
      return mNofPads;
    }

    bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const override
    {
      if (dualSampaChannel < 0 || dualSampaChannel > 63) {
        throw std::out_of_range("dualSampaChannel should be between 0 and 63");
      }
      auto it = std::find_if(begin(mMotifPositions), end(mMotifPositions), [&](const MOTIFPOSITION& mp) { return mp.FECId()==dualSampaId; });
      if (it == mMotifPositions.end()) {
        throw std::out_of_range("dualSampaId is not there");
      }
      int index = std::distance(mMotifPositions.begin(), it);
      int padIndex = index * 64 + dualSampaChannel;
      return mPads[padIndex].isValid();
    }

    bool hasPadByPosition(double x, double y) const override {
      for ( auto i = 0; i < mFEContours.size(); ++i ){
        if (mFEContours[i].contains(x,y)) {
          return true;
        }
      }
       return false;
    }

    o2::mch::contour::Contour<double> getEnvelop() const override
    {
      std::vector<o2::mch::contour::Polygon<double>> polygons;
      for (auto i = 0; i < mFEContours.size(); ++i) {
        auto c = mFEContours[i];
        for (auto j = 0; j < c.size(); ++j) {
          polygons.push_back(c[j]);
        }
      }
      return o2::mch::contour::createContour(polygons);
    }

    o2::mch::contour::Contour<double> getSampaPads(int dualSampaIndex) const override
    {
      return mPadContours[dualSampaIndex];
    }

    std::vector<o2::mch::contour::Contour<double>> getSampaContours() const override {
      std::vector<o2::mch::contour::Contour<double>> contours;
      contours.insert(contours.end(),mFEContours.begin(),mFEContours.end());
      return contours;
    }

  private:

    std::vector<Pad> getPads(const MOTIFPOSITION& mp, const MotifTypeArray& motifTypes) {
      std::vector<Pad> motifPads{mp.getPads(motifTypes[mp.motifTypeId()])};
      std::vector<Pad> pads;
      for (auto p: motifPads) {
        pads.push_back(p.translate(mp.positionX(),mp.positionY()));
      }
      return pads;
    }

    void populatePadsForOneMotifPosition(int index, const MOTIFPOSITION& mp, const MotifTypeArray& motifTypes) {
      const MotifType& mt = motifTypes[mp.motifTypeId()];
      auto pads = getPads(mp,motifTypes);
      for (auto i = 0; i < pads.size(); ++i) {
         int fecChannel = berg2channel(mt.getBerg(i));
         int padIndex = index*64 + fecChannel;
         mPads[padIndex] = pads[i];
      }
    }

    void populatePads(const MotifTypeArray& motifTypes) {
      for ( int index = 0; index < mMotifPositions.size(); ++index ) {
        const MOTIFPOSITION& mp = mMotifPositions[index];
        const MotifType& mt = motifTypes[mp.motifTypeId()];
        populatePadsForOneMotifPosition(index,mp,motifTypes);
        mNofPads += mt.getNofPads();
      }
    }

    std::vector<o2::mch::contour::Polygon<double>> padAsPolygons(const std::vector<Pad>& pads)
    {
      std::vector<o2::mch::contour::Polygon<double>> cpads;
      for (auto&& p: pads) {
        cpads.push_back({
                         { p.xBottomLeft, p.yBottomLeft},
                         { p.xTopRight, p.yBottomLeft},
                         { p.xTopRight, p.yTopRight},
                         { p.xBottomLeft, p.yTopRight},
                         { p.xBottomLeft, p.yBottomLeft}
                       });
      }
      return cpads;
    }

    void createContours(const MotifTypeArray& motifTypes)
    {
      for (int index = 0; index < mMotifPositions.size(); ++index) {
        auto pads = padAsPolygons(getPads(mMotifPositions[index], motifTypes));
        o2::mch::contour::Contour<double> contour;
        for (auto& p: pads) {
          contour.addPolygon(p);
        }
        mPadContours[index] = contour;
        mFEContours[index] = o2::mch::contour::createContour(pads);
      }
    }

    int mId;
    bool mIsBendingPlane;
    int mNofPads;
    std::array<Pad,NFEC*64> mPads;
    std::array<MOTIFPOSITION,NFEC> mMotifPositions;
    std::array<o2::mch::contour::Contour<double>,NFEC> mPadContours;
    std::array<o2::mch::contour::Contour<double>,NFEC> mFEContours;
};
)***";
  return decl.str();
}

std::string
generateCodeForSegmentationType(int index, const Value &segmentations, const Value &motiftypes,
                                const Value &padsizes)
{
  std::string code = generateCodeForOneSegmentation(index, true, segmentations, motiftypes, padsizes);
  code += generateCodeForOneSegmentation(index, false, segmentations, motiftypes, padsizes);
  return code;
}

std::pair<std::string, std::string> generateCodeForSegmentationFactory(const Value &segmentations)
{
  std::ostringstream decl;
  std::ostringstream impl;

  decl << R"(#include "genSegmentationInterface.h"
#include "genMotifType.h"
#include <memory>
)";

  decl << mappingNamespaceBegin();

  decl << R"(
int getNumberOfSegmentations();
std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane);
  )";
  decl << mappingNamespaceEnd();

  impl << R"(#include "genSegmentationImpl0.h"
)";
  impl << mappingNamespaceBegin();

  impl << R"(
  int getNumberOfSegmentations() {return 21;}
  std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane) {
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

  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
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
                                  const Value &bergs)
{
  std::pair<std::string, std::string> code = generateCodeForSegmentationInterface();
  outputCode(code.first, code.second, "genSegmentationInterface");

  std::string decl = generateCodeForSegmentationCommon();
  decl += "\n";
  decl += generateCodeForBerg2Manu(bergs, 0);
  decl += generateCodeForBerg2Manu(bergs, 1);
  decl += "\n";
  for (int i = 0; i < segmentations.GetArray().Size(); ++i) {
    decl += generateCodeForSegmentationType(i, segmentations, motiftypes, padsizes);
  }
  decl += mappingNamespaceEnd();
  outputCode(decl, "", "genSegmentationImpl0");

  code = generateCodeForSegmentationFactory(segmentations);
  outputCode(code.first, code.second, "genSegmentationFactory");
}

void generateCodeForDESegmentationFactory(const Value &segmentations, const Value &detection_elements)
{
  std::ostringstream decl;
  std::ostringstream impl;

  decl << R"(#include "genSegmentationInterface.h"
#include <memory>
)";

  decl << mappingNamespaceBegin();

  decl << R"(

int getSegTypeIndexFromDetElemIndex(int deIndex);

std::unique_ptr<SegmentationInterface> getDESegmentation(int deIndex, bool isBendingPlane);

  )";
  decl << mappingNamespaceEnd();

  impl << R"(
#include "genSegmentationFactory.h"
#include <array>
)";
  impl << mappingNamespaceBegin();

  impl << R"(
int getSegTypeIndexFromDetElemIndex(int deIndex) {
)";

  impl << "\n  static std::array<int," << detection_elements.Size() << "> segtype{";

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

  impl << "};\n";

  impl << R"(
    if (deIndex >= segtype.size()) throw std::out_of_range("deIndex is incorrect");
    return segtype[deIndex];
  }

  std::unique_ptr<SegmentationInterface> getDESegmentation(int deIndex, bool isBendingPlane) {
      return getSegmentation(getSegTypeIndexFromDetElemIndex(deIndex),isBendingPlane);
  }
)";

  impl << mappingNamespaceEnd();

  outputCode(decl.str(), impl.str(), "genDESegmentationFactory");
}
