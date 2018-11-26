#include "yaml-cpp/yaml.h"
#include <iostream>
#include "Run2_generated.h"
#include "Run2FB2YAML.h"

void createOneEvent(flatbuffers::FlatBufferBuilder& fbb)
{
  float x = -13.862791;
  float y = 24.571938;
  float z = -530.339172;

  float charge = 41.258621;

  std::vector<flatbuffers::Offset<run2::Digit>> clusterDigits;
  std::vector<flatbuffers::Offset<run2::Cluster>> clusters;

  clusterDigits.push_back(run2::CreateDigit(fbb, 330, 100, 99, 0, 2.229247));
  clusterDigits.push_back(run2::CreateDigit(fbb, 348, 100, 99, 3, 4.728873));

  auto pre = run2::CreatePreClusterDirect(fbb, &clusterDigits);

  auto pos = run2::CreateClusterPos(fbb, x, y, z);
  clusters.push_back(run2::CreateCluster(fbb, pre, pos, charge));

  uint16_t bc = 698;
  uint32_t period = 7;
  int ntracklets = 0;
  bool isMB = false;

  auto eventBuffer = run2::CreateEventDirect(fbb, bc, period, ntracklets, isMB, &clusters);

  fbb.Finish(eventBuffer);
}

int main()
{
  flatbuffers::FlatBufferBuilder fbb{ 1024 };

  createOneEvent(fbb);

  auto event = run2::GetEvent(fbb.GetBufferPointer());

  YAML::Emitter out;

  out << *(event->clusters());

  std::cout << out.c_str() << "\n";
  return 0;
}
