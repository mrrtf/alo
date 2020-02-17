#include "Run2ToYAML.h"
#include "AliMUONPad.h"
#include "AliMUONVDigit.h"
#include "Mapping.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool isRequiredPlane(int manuid, bool isBending)
{
  if (isBending && manuid >= 1024) {
    return false;
  }
  if (!isBending && manuid < 1024) {
    return false;
  }
  return true;
}

std::string containsDigits(const YAML::Node& node)
{
  if (!node.IsMap()) {
    return "node is not a map";
  }
  auto pre = node["pre"];
  if (!pre.IsMap()) {
    return "pre is not a map";
  }
  auto digitgroup = pre["digitgroup"];
  if (!digitgroup.IsMap()) {
      return "digitgroup is not a map";
  }
  auto digits = digitgroup["digits"];
  if (!digits.IsSequence()) {
    return "digits is not a sequence";
  }
  return "";
}

bool sanityCheck(YAML::Node& top) {
  if (!top["deid"]) {
    std::cerr << "Top node does not contain required deid key !\n";
    return false;
  }
  if (!top["clusters"]) {
    std::cerr << "Top node does not contain required clusters key !\n";
    return false;
  }
  if (!top["clusters"].IsSequence()) {
    std::cerr << "Key clusters does not refer to a sequence !\n";
    return false;
  }
  return true;
}

YAML::Node readYAML(const char* filename)
{
  std::ifstream in(filename);
  if (!in.is_open()) {
    std::cerr << "Cannot open " << filename << "\n";
    return YAML::Node{};
  }
  std::stringstream buffer;
  buffer << in.rdbuf();

  YAML::Node top = YAML::Load(buffer.str());

  if (!sanityCheck(top)) {
      return YAML::Node{};
  }

  return top;
}

TObjArray* getPads(const YAML::Node& node, int detElemId, bool isBending)
{
  auto msg = containsDigits(node);
  if (!msg.empty()) {
      std::cerr << "no digits to be found here...: " << msg << "\n";
    return nullptr;
  }

  auto digits = node["pre"]["digitgroup"]["digits"];

  const AliMp::CathodType cathode = getCathode(detElemId, isBending);
  const AliMpVSegmentation* seg = getSegmentation(detElemId, isBending);

  TObjArray* pads = new TObjArray;
  pads->SetOwner(kTRUE);

  for (int i = 0; i < digits.size(); i++) {
    auto d = digits[i];
    int manuid = d["dsid"].as<int>();
    if (!isRequiredPlane(manuid, isBending)) {
      continue;
    }

    int manuchannel = d["dsch"].as<int>();
    float charge = d["charge"].as<float>();

    AliMpPad pad = seg->PadByLocation(manuid, manuchannel);
    if (!pad.IsValid()) {
      std::cerr << "got an invalid pad!\n";
      exit(7);
    }

    AliMUONPad* mpad = new AliMUONPad(detElemId, cathode,
                                      pad.GetIx(), pad.GetIy(), pad.GetPositionX(), pad.GetPositionY(),
                                      pad.GetDimensionX(), pad.GetDimensionY(),
                                      charge);
    auto padId = AliMUONVDigit::BuildUniqueID(detElemId, pad.GetManuId(), pad.GetManuChannel(), cathode);
    pads->Add(mpad);
  }
  return pads;
}

std::vector<TObjArray*> getPads(YAML::Node desc, int detElemId)
{
  bool isBending{ true };

  std::vector<TObjArray*> pads = {
    getPads(desc, detElemId, isBending),
    getPads(desc, detElemId, !isBending)
  };

  return pads;
}

