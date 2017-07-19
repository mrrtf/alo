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

struct MotifPosition
{
    int fecId;
    int motifTypeId;
    float dx;
    float dy;
    float x;
    float y;
};

struct Pad
{
    Pad() : fecId{0}, fecChannel{0}, x{0}, y{0}, dx{0}, dy{0}
    {}

    Pad(int f, int c, float x_, float y_, float dx_, float dy_) : fecId{f},
                                                                  fecChannel{c},
                                                                  x{x_},
                                                                  y{y_},
                                                                  dx{dx_},
                                                                  dy{dy_}
    {}

    int fecId;
    int fecChannel;
    float x;
    float y;
    float dx;
    float dy;
};

void generateCodeForMotifPosition(std::ostringstream& code,
                                  int fec,
                                  float x, float y,
                                  float dx, float dy, /* TODO : special motifs with 2 pad sizes */
                                  const rapidjson::Value& mt,
                                  const rapidjson::Value& bergpins)
{
  assert(bergpins.IsArray());
  auto bergArray = bergpins.GetArray();
  auto jsonpads = mt["pads"].GetArray();
  std::array<Pad, 64> pads{};
  for (auto& pad: jsonpads) {
    int b = pad["berg"].GetInt();
    auto& pin = bergArray[b - 1];
    assert(pin.IsObject());
    float padx = pad["ix"].GetInt() * dx + x;
    float pady = pad["iy"].GetInt() * dy + y;
    pads[atoi(pin["manu"].GetString())] = {
      fec, (atoi(pin["manu"].GetString())), padx-dx, pady-dy, padx+dx, pady+dy
    };
  }

  for (auto i = 0; i < pads.size(); ++i) {
    const Pad& pad = pads[i];
    code << "  {" << pad.x << "," << pad.y << "," << pad.dx << "," << pad.dy << "}";
    if (pad.dx < 0.1 && pad.dy < 0.1) { code << " /* not connected */"; }
    if (i < pads.size() - 1) { code << ",\n"; }
  }
}

void generateCodeForOneSegmentation(int index, bool isBending, std::ostringstream& code,
                                    const rapidjson::Value& seg,
                                    const rapidjson::Value& motiftypes,
                                    const rapidjson::Value& padsizes, const rapidjson::Value& bergpins)
{
  //if (index!=6 || isBending) return;

  assert(seg.IsObject());
  const rapidjson::Value& plane = isBending ? seg["bending"] : seg["non-bending"];
  assert(plane.IsObject());

  code << "template<>\n";
  code << "Segmentation<" << index << "," << (isBending ? "true" : "false") << ">::Segmentation() "
       << " : mId(" << index;
  code << "), mIsBendingPlane(" << (isBending ? "true" : "false")
       << ") {\n";

  auto mpArray = plane["motifpositions"].GetArray();
  auto mtArray = motiftypes.GetArray();
  auto psArray = padsizes.GetArray();

  std::vector<MotifPosition> motifpositions;

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

  code << "  mFecId = { ";

  int i{0};
  for (auto& mp: motifpositions) {
    code << mp.fecId;
    ++i;
    if (i < motifpositions.size()) { code << ","; }
  }

  code << "};\n";

  code << "  mPads = {\n";

  int npads{0};
  i = 0;
  for (auto& mp: motifpositions) {
    auto& mt = mtArray[mp.motifTypeId];
    generateCodeForMotifPosition(code, mp.fecId, mp.x, mp.y, mp.dx, mp.dy, mt, bergpins);
    npads += mt["pads"].GetArray().Size();
    if (i < motifpositions.size() - 1) { code << ",\n"; }
  }
  code << "};\n";
  code << "  mNofPads = " << npads << ";\n";
  code << "}\n";
}

std::pair<std::string, std::string>
generateCodeForSegmentationCommon()
{
  std::ostringstream decl;
  std::ostringstream impl;

  decl << R"(#include <vector>
#include "genMotifType.h"
#include <tuple>
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
    virtual bool hasPadByPosition(float x, float y) const = 0;
    virtual bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const = 0;
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
    { return mFecId.size(); }

    int nofPads() const override
    {
      return mNofPads;
    }

    bool hasPadByPosition(float x, float y) const override {
       return false;
    }

    bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const override {
       return false;
    }

  private:

    struct Pad {
      float xBottomLeft;
      float yBottomLeft;
      float xTopRight;
      float yTopRight;
    };

    int mId;
    bool mIsBendingPlane;
    int mNofPads;
    std::vector<Pad> mPads;
    std::vector<int> mFecId;
};
)";
  decl << mappingNamespaceEnd();
  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
}

std::pair<std::string, std::string>
generateCodeForSegmentationType(int index, const rapidjson::Value& segmentations, const rapidjson::Value& motiftypes,
                             const rapidjson::Value& padsizes,
                             const rapidjson::Value& bergs)
{
  std::ostringstream decl;
  std::ostringstream impl;
  assert(motiftypes.IsArray());
  assert(padsizes.IsArray());
  assert(segmentations.IsArray());
  assert(bergs.IsArray());

  decl << "#include \"genSegmentation.h\"\n";

  decl << mappingNamespaceBegin();

  auto berg80 = (bergs[0]["id"].GetInt() == 80) ? bergs[0].GetObject() : bergs[1].GetObject();
  auto berg100 = (bergs[0]["id"].GetInt() == 100) ? bergs[0].GetObject() : bergs[1].GetObject();
  auto& bergpins = (index < 1) ? berg80["pins"] : berg100["pins"];
  const auto& seg = segmentations.GetArray()[index];
  generateCodeForOneSegmentation(index, true, decl, seg, motiftypes, padsizes, bergpins);
  generateCodeForOneSegmentation(index, false, decl, seg, motiftypes, padsizes, bergpins);

  decl << mappingNamespaceEnd();

  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
}

