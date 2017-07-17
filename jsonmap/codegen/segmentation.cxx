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

void generateCodeForMotifPosition(std::ostringstream& code, const rapidjson::Value& mp)
{
  code << "MotifPosition{ " << mp["x"].GetDouble() << "," << mp["y"].GetDouble() << ",";
  code << mp["motif"].GetInt() << "," << mp["padsize"].GetInt() << ",";
  code << mp["fec"].GetInt() << "}";
}

void generateCodeForOneSegmentation(int index, bool isBending, std::ostringstream& decl,
                                    std::ostringstream& impl,
                                    const rapidjson::Value& seg)
{
  assert(seg.IsObject());
  const rapidjson::Value& plane = isBending ? seg["bending"] : seg["non-bending"];
  assert(plane.IsObject());

  impl << "template<>\n";
  impl << "Segmentation<" << index << "," << (isBending ? "true":"false") << ">::Segmentation() : mId(" << index;
  impl << "), mIsBendingPlane(" << (isBending ? "true":"false") << "), mMP{";

  auto ab = plane["motifpositions"].GetArray();
  impl << "  ";
  int i{0};
  for (auto& mp: ab) {
    generateCodeForMotifPosition(impl, mp);
    if (++i < ab.Size()) { impl << ", "; }
    impl << "\n  ";
  }
  impl << "}\n";
  impl << "{}\n";
}

void generateMotifPositionCode(std::ostringstream& decl, std::ostringstream& impl)
{
  decl << R"(struct MotifPosition
{
    float x;
    float y;
    int motifTypeId;
    int padSizeId;
    int fecId;
};
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

  impl << mappingNamespaceBegin();

  decl << "#include <vector>\n";

  decl << mappingNamespaceBegin();

  generateMotifPositionCode(decl, impl);

  decl << R"(
template<int N, bool isBendingPlane>
class Segmentation
{
  public:
    Segmentation()
    { throw std::out_of_range("invalid segmentation initialization"); }

    int Id() const
    { return mId; }

    int IsBendingPlane() const
    { return mIsBendingPlane; }

    int NofDualSampas() const
    { return mMP.size(); }

  private:
    int mId;
    bool mIsBendingPlane;
    std::vector <MotifPosition> mMP;
};
)";


  int index{0};
  for (auto& seg : segmentations.GetArray()) {
    generateCodeForOneSegmentation(index, true, decl, impl, seg);
    generateCodeForOneSegmentation(index, false, decl, impl, seg);
    ++index;
    impl << "\n";
  }

  decl << mappingNamespaceEnd();
  impl << mappingNamespaceEnd();

  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
}

