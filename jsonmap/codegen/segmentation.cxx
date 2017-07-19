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
  code << "{ }\n";
}

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
#include "MotifPosition.h"
#include "padfinder.h"
#include "genPadSize.h"
)";

  decl << mappingNamespaceBegin();

  decl << R"(
class SegmentationInterface {
  public:
    virtual bool isBendingPlane() const = 0;
    virtual int getId() const = 0;
    virtual int nofDualSampas() const = 0;
    virtual int nofPads() const = 0;
};

template<int N, bool>
class Segmentation : public SegmentationInterface
{
  public:
    Segmentation()
    { throw std::out_of_range("invalid segmentation initialization"); }

    int getId() const override
    { return mId; }

    bool isBendingPlane() const override
    { return mIsBendingPlane; }

    int nofDualSampas() const override
    { return mMP.size(); }

    int nofPads() const override
    {
      int n{0};
      for (const auto& mp : mMP) {
        const MotifType& motifType = arrayOfMotifTypes[mp.getMotifTypeId()];
        n += motifType.getNofPads();
      }
      return n;
    }

    int padIdByPosition(float x, float y) const {
       for (const auto& mp: mMP) {
         int padid = o2::mch::mapping::padIdByPosition(mp,arrayOfMotifTypes,arrayOfPadSizes,x,y);
         if (padid>=0) return padid;
       }
       return -1;
    }

  private:

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

