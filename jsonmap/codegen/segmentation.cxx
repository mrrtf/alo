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

void generateCodeForMotifPosition(std::ostringstream& code, const rapidjson::Value& mp)
{
  code << "MotifPosition{ " << mp["x"].GetDouble() << "," << mp["y"].GetDouble() << ",";
  code << mp["motiftype"].GetInt() << "," << mp["padsize"].GetInt() << ",";
  code << mp["fec"].GetInt() << "}";
}

void generateCodeForOneSegmentation(int index, bool isBending, std::ostringstream& code,
                                    const rapidjson::Value& seg)
{
  assert(seg.IsObject());
  const rapidjson::Value& plane = isBending ? seg["bending"] : seg["non-bending"];
  assert(plane.IsObject());

  code << "template<>\n";
  code << "Segmentation<" << index << "," << (isBending ? "true" : "false") << ">::Segmentation() : mId(" << index;
  code << "), mIsBendingPlane(" << (isBending ? "true" : "false") << "), mMP{";

  auto ab = plane["motifpositions"].GetArray();
  code << "  ";
  int i{0};
  for (auto& mp: ab) {
    generateCodeForMotifPosition(code, mp);
    if (++i < ab.Size()) { code << ", "; }
    code << "\n  ";
  }
  code << "}\n";
  code << "{ init(); }\n";
}

void generateMotifPositionCode(std::ostringstream& code)
{
  code << R"(struct MotifPosition
{
    float x;
    float y;
    int motifTypeId;
    int padSizeId;
    int fecId;
};
int gMotifTypeIdMax{0};
int gPadSizeIdMax{0};
int gFecIdMax{0};
)";
};

std::pair<std::string, std::string>
generateCodeForSegmentations(const rapidjson::Value& segmentations, const rapidjson::Value& motiftypes,
                             const rapidjson::Value& padsizes)
{
  assert(motiftypes.IsArray());
  assert(padsizes.IsArray());
  assert(segmentations.IsArray());

  std::ostringstream decl;
  std::ostringstream impl;

  decl << R"(#include <vector>
#include "genMotifType.h"
)";

  decl << mappingNamespaceBegin();

  generateMotifPositionCode(decl);

  decl << R"(
class SegmentationInterface {
  public:
    virtual bool IsBendingPlane() const = 0;
    virtual int GetId() const = 0;
    virtual int NofDualSampas() const = 0;
    virtual int NofPads() const = 0;
};

template<int N, bool isBendingPlane>
class Segmentation : public SegmentationInterface
{
  public:
    Segmentation()
    { throw std::out_of_range("invalid segmentation initialization"); }

    int GetId() const override
    { return mId; }

    bool IsBendingPlane() const override
    { return mIsBendingPlane; }

    int NofDualSampas() const override
    { return mMP.size(); }

    int NofPads() const override
    {
      int n{0};
      for (const auto& mp : mMP) {
        const MotifType& motifType = ArrayOfMotifTypes[mp.motifTypeId];
        n += motifType.GetNofPads();
      }
      return n;
    }
  private:

    void init() {
      for (const auto& mp: mMP) {
        gMotifTypeIdMax = std::max(gMotifTypeIdMax,mp.motifTypeId);
        gPadSizeIdMax = std::max(gPadSizeIdMax,mp.padSizeId);
        gFecIdMax = std::max(gFecIdMax,mp.fecId);
      }
    }

    int mId;
    bool mIsBendingPlane;
    std::vector <MotifPosition> mMP;
};
)";

  int index{0};
  for (auto& seg : segmentations.GetArray()) {
    generateCodeForOneSegmentation(index, true, decl, seg);
    generateCodeForOneSegmentation(index, false, decl, seg);
    ++index;
    decl << "\n";
  }

  decl << mappingNamespaceEnd();

  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
}

