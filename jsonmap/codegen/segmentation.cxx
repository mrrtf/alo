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

using rapidjson::Value;

struct MotifPosition
{
    int fecId;
    int motifTypeId;
    float dx;
    float dy;
    float x;
    float y;
};

std::ostream& operator<<(std::ostream& os, const MotifPosition& position)
{
  os << "{" << position.fecId << "," << position.motifTypeId << "," << position.dx << ","
     << position.dy << "," << position.x << "," << position.y << "}";
  return os;
}


auto
getMotifPositionArray(int index, const Value& segmentations, bool isBendingPlane)
{
  const auto& seg = segmentations.GetArray()[index];
  const Value& plane = isBendingPlane ? seg["bending"] : seg["non-bending"];
  return plane["motifpositions"].GetArray();
}


std::vector<MotifPosition>
getMotifPositions(int index, bool bending, const Value& segmentations, const Value& motiftypes,
                  const Value& padsizes)
{
  std::vector<MotifPosition> motifpositions;

  auto mpArray = getMotifPositionArray(index, segmentations, bending);
  auto mtArray = motiftypes.GetArray();
  auto psArray = padsizes.GetArray();

  for (auto& mp: mpArray) {
    int padSizeId = mp["padsize"].GetInt();
    motifpositions.push_back({
                               mp["fec"].GetInt(),
                               mp["motiftype"].GetInt(),
                               static_cast<float>(psArray[padSizeId]["x"].GetDouble()),
                               static_cast<float>(psArray[padSizeId]["y"].GetDouble()),
                               static_cast<float>(mp["x"].GetDouble()),
                               static_cast<float>(mp["y"].GetDouble())
                             });
  }

  // sort the motifpositions by fecId => is that needed at all as we have a fecid <-> index array ?
  std::sort(motifpositions.begin(), motifpositions.end(), [](const MotifPosition& mp1,
                                                             const MotifPosition& mp2) {
    return mp1.fecId < mp2.fecId;
  });

  return motifpositions;
}

std::string bergToChannelFunctionName(int index)
{
  if (index < 2 )
  {
    return "berg80ToManu";
  }
  return "berg100ToManu";
}

std::string generateCodeForOneSegmentation(int index, bool isBending,
                                           const Value& segmentations,
                                           const Value& motiftypes,
                                           const Value& padsizes)
{
  std::vector<MotifPosition> motifpositions = getMotifPositions(index, isBending, segmentations, motiftypes, padsizes);

  std::ostringstream code;

  std::string bendingString = (isBending ? "true" : "false");
  code << "template<>\n";
  code << "SegmentationImpl0<" << index << "," << bendingString
       << "," << motifpositions.size() << "," << bergToChannelFunctionName(index) << ">::SegmentationImpl0(const MotifTypeArray& motifTypes)"
       << " : mId(" << index << "),mIsBendingPlane(" << bendingString << "), "
       << "mNofPads{0},"
       << "mMotifPositions{ {\n";

  int i{0};
  for (const auto& mp: motifpositions) {
    code << "  " << mp;
    if (i++ < motifpositions.size() - 1) { code << ",\n"; }
  }
  code << "}}\n";

  code << "{";

  code << "  populatePads(motifTypes);\n";

  code << "}\n";

  return code.str();
}

std::pair<std::string, std::string>
generateCodeForSegmentationInterface()
{
  std::ostringstream decl;
  std::ostringstream impl;

  decl << mappingNamespaceBegin();

  decl << R"(
class SegmentationInterface {
  public:
    virtual bool isBendingPlane() const = 0;
    virtual int getId() const = 0;
    virtual int nofDualSampas() const = 0;
    virtual int nofPads() const = 0;
    virtual bool hasPadByPosition(float x, float y) const = 0;
    virtual bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const = 0;
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
#include "boost/format.hpp"
)";

  decl << mappingNamespaceBegin();

  decl << R"***(
template<int SEGID, bool BENDINGPLANE, int NFEC, int (*berg2channel)(int)>
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
      auto it = std::find_if(begin(mMotifPositions), end(mMotifPositions), [&](const MotifPosition& mp) { return mp.fecId==dualSampaId; });
      if (it == mMotifPositions.end()) {
        throw std::out_of_range("dualSampaId is not there");
      }
      int index = std::distance(mMotifPositions.begin(), it);
      int padIndex = index * 64 + dualSampaChannel;
      return mPads[padIndex].isValid();
    }

    bool hasPadByPosition(float x, float y) const override {
       return false;
    }

  private:

    struct Pad
    {
        Pad(float x1 = 0, float y1 = 0, float x2 = 0, float y2 = 0) :
          xBottomLeft{x1}, yBottomLeft{y1},
          xTopRight{x2}, yTopRight{y2}
        {}

        bool isValid() const
        { return (xTopRight-xBottomLeft) > 0.1; }

        friend std::ostream& operator<<(std::ostream& os, const Pad& pad)
        {
          if (pad.isValid()) {
            os << boost::format("(%7.2f,%7.2f)->(%7.2f,%7.2f)") % pad.xBottomLeft % pad.yBottomLeft %
                                                                  pad.xTopRight % pad.yTopRight;
          }
        else
        {
          os << " ( not existing pad )";
        }
        return os;
        }

        float xBottomLeft;
        float yBottomLeft;
        float xTopRight;
        float yTopRight;
    };

    struct MotifPosition
    {
      MotifPosition(int f=0, int m=0, float dx_=0, float dy_=0, float x_=0, float y_=0) :
        fecId(f), motifTypeId(m), dx(dx_),dy(dy_), x(x_), y(y_) {}
      int fecId;
      int motifTypeId;
      float dx;
      float dy;
      float x;
      float y;
    };

    void populatePadsForOneMotifPosition(int index, const MotifPosition& mp, const MotifTypeArray& motifTypes) {
      const MotifType& mt = motifTypes[mp.motifTypeId];
      for ( int i = 0; i < mt.getNofPads(); ++i ) {
         int fecChannel = berg2channel(mt.getBerg(i));
         float padx = mp.x + mt.getIx(i) * mp.dx;
         float pady = mp.y + mt.getIy(i) * mp.dy;
         int padIndex = index*64 + fecChannel;
         mPads[padIndex] = { padx-mp.dx, pady-mp.dy, padx+mp.dx, pady+mp.dy };
      }
    }

    void populatePads(const MotifTypeArray& motifTypes) {
      for ( int index = 0; index < mMotifPositions.size(); ++index ) {
        const MotifPosition& mp = mMotifPositions[index];
        const MotifType& mt = motifTypes[mp.motifTypeId];
        populatePadsForOneMotifPosition(index,mp,motifTypes);
        mNofPads += mt.getNofPads();
      }
    }

    int mId;
    bool mIsBendingPlane;
    int mNofPads;
    std::array<Pad,NFEC*64> mPads;
    std::array<MotifPosition,NFEC> mMotifPositions;
};
)***";
  return decl.str();
}

std::string
generateCodeForSegmentationType(int index, const Value& segmentations, const Value& motiftypes,
                                const Value& padsizes)
{
  std::string code = generateCodeForOneSegmentation(index, true, segmentations, motiftypes, padsizes);
  code += generateCodeForOneSegmentation(index, false, segmentations, motiftypes, padsizes);
  return code;
}

std::pair<std::string, std::string> generateCodeForSegmentationFactory(const Value& segmentations)
{
  std::ostringstream decl;
  std::ostringstream impl;

  decl << R"(#include "genSegmentationInterface.h"
#include "genMotifType.h"
#include <memory>
)";

  decl << mappingNamespaceBegin();

  decl << R"(

std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane);

  )";
  decl << mappingNamespaceEnd();

  impl << R"(#include "genSegmentationImpl0.h"
)";
  impl << mappingNamespaceBegin();

  impl << R"(
  std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane) {
)";

  for (int i = 0; i < 21; ++i) {
    for (auto b : std::array<bool, 2>{true, false}) {
      auto n = getMotifPositionArray(i, segmentations, b).Size();
      impl << "    if (isBendingPlane==" << (b ? "true" : "false") << " && type==" << i << ") {\n";
      impl << "      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<" << i << ","
           << (b ? "true" : "false") << "," << n << "," << bergToChannelFunctionName(i) << ">{arrayOfMotifTypes}};\n";
      impl << "    };\n";
    }
  }

  impl << "  return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<-1,true,0,nullptr>{arrayOfMotifTypes}};\n";
  impl << "}\n";

  impl << mappingNamespaceEnd();

  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
}

std::string generateCodeForBerg2Manu(const Value& bergs, int id)
{
  std::ostringstream code;

  int npins = bergs[id]["id"].GetInt();

  code << "int berg" << npins << "ToManu(int berg) {\n";

  int i{0};
  for (const auto& b: bergs[id]["pins"].GetArray()) {
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

void generateCodeForSegmentations(const Value& segmentations, const Value& motiftypes,
                                  const Value& padsizes,
                                  const Value& bergs)
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

