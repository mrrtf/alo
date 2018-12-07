#ifndef ALO_DATAFORMATS_MCH_RUN2FB2YAML_H
#define ALO_DATAFORMATS_MCH_RUN2FB2YAML_H

#include "yaml-cpp/yaml.h"
#include "Run2_generated.h"

using vod = flatbuffers::Vector<flatbuffers::Offset<run2::Digit>>;
using voc = flatbuffers::Vector<flatbuffers::Offset<run2::Cluster>>;

namespace YAML
{
template <>
struct convert<run2::Digit> {
  static Node encode(const run2::Digit& d)
  {
    Node node;
    node["deid"] = d.deid();
    node["dsid"] = d.manuid();
    node["dsch"] = static_cast<int>(d.manuchannel());
    node["adc"] = d.adc();
    node["charge"] = d.charge();
    return node;
  }
};

template <>
struct convert<vod> {
  static Node encode(const vod& vd)
  {
    Node node;
    for (auto&& d : vd) {
      Node n = YAML::convert<run2::Digit>::encode(*d);
      node.push_back(n);
    }
    return node;
  }
};

template <>
struct convert<run2::PreCluster> {
  static Node encode(const run2::PreCluster& pre)
  {
    Node node;
    node["digitgroup"]["reftime"] = 0;
    node["digitgroup"]["digits"] = YAML::convert<vod>::encode(*(pre.digits()));
    return node;
  }
};

template <>
struct convert<run2::ClusterPos> {
  static Node encode(const run2::ClusterPos& pos)
  {
    Node node;
    node["x"] = pos.x();
    node["y"] = pos.y();
    node["z"] = pos.z();
    return node;
  }
};

template <>
struct convert<run2::Cluster> {
  static Node encode(const run2::Cluster& clu)
  {
    Node node;
    node["pre"] = YAML::convert<run2::PreCluster>::encode(*(clu.pre()));
    node["pos"] = YAML::convert<run2::ClusterPos>::encode(*(clu.pos()));
    node["charge"] = clu.charge();
    return node;
  }
};

template <>
struct convert<voc> {
  static Node encode(const voc& vc)
  {
    Node node;
    for (auto&& c : vc) {
      Node n = YAML::convert<run2::Cluster>::encode(*c);
      node.push_back(n);
    }
    return node;
  }
};

} // namespace YAML

YAML::Emitter& operator<<(YAML::Emitter& out, const run2::Digit& d);
YAML::Emitter& operator<<(YAML::Emitter& out, const vod& vd);
YAML::Emitter& operator<<(YAML::Emitter& out, const run2::PreCluster& pre);
YAML::Emitter& operator<<(YAML::Emitter& out, const run2::ClusterPos& pos);
YAML::Emitter& operator<<(YAML::Emitter& out, const run2::Cluster& clu);
YAML::Emitter& operator<<(YAML::Emitter& out, const voc& vc);

#endif
