#ifndef ALO_ALIROOT_R23_LEGACY_RUN2TOYAML_H
#define ALO_ALIROOT_R23_LEGACY_RUN2TOYAML_H

#include "yaml-cpp/yaml.h"
#include "AliMUONPad.h"
#include "TObjArray.h"

class TObjArray;

YAML::Node readYAML(const char* filename);

std::vector<TObjArray*> getPads(YAML::Node desc, int detElemId);

namespace YAML
{
template <>
struct convert<AliMUONPad> {
  static Node encode(const AliMUONPad& pad)
  {
    Node node;
    node["x"] = pad.X();
    node["y"] = pad.Y();
    node["dx"] = pad.DX();
    node["dy"] = pad.DY();
    node["charge"] = pad.Charge();
    return node;
  }
};

template <>
struct convert<TObjArray> {
  static Node encode(const TObjArray& pixels)
  {
    Node node;
    for (int i = 0; i <= pixels.GetLast(); i++) {
      AliMUONPad* pix = static_cast<AliMUONPad*>(pixels.At(i));
      node["pixels"].push_back(*pix);
    }
    return node;
  }
};

} // namespace YAML
#endif

