#include "Run2FB2YAML.h"

YAML::Emitter& operator<<(YAML::Emitter& out, const run2::Digit& d)
{
  out << YAML::convert<run2::Digit>::encode(d);
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const vod& vd)
{
  out << YAML::convert<vod>::encode(vd);
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const run2::PreCluster& pre)
{
  out << YAML::convert<run2::PreCluster>::encode(pre);
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const run2::ClusterPos& pos)
{
  out << YAML::convert<run2::ClusterPos>::encode(pos);
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const run2::Cluster& clu)
{
  out << YAML::convert<run2::Cluster>::encode(clu);
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const voc& vc)
{
  out << YAML::convert<voc>::encode(vc);
  return out;
}

