#include "AliMUONCluster.h"
#include "AliMUONClusterFinderMLEMdebug.h"
#include "AliMUONPad.h"
#include "AliMUONPreClusterFinder.h"
#include "AliMpArea.h"
#include "Run2ToYAML.h"
#include "TObjArray.h"
#include "boost/program_options.hpp"
#include "Mapping.h"
#include <string>
#include <fstream>
#include <iostream>

namespace po = boost::program_options;

static float lowestPadCharge{ 0.92 };
static float lowestClusterCharge{ 0.46 };

AliMUONClusterFinderMLEMdebug* getClusterFinder(std::vector<TObjArray*>& pads, int detElemId)
{
  bool debug{ true };
  auto* clusterFinder = new AliMUONClusterFinderMLEMdebug(debug, new AliMUONPreClusterFinder);

  const AliMpVSegmentation* seg[2] = { getSegmentation(detElemId, AliMp::kCath0),
                                       getSegmentation(detElemId, AliMp::kCath1) };

  clusterFinder->SetChargeHints(lowestPadCharge, lowestClusterCharge);
  clusterFinder->Prepare(detElemId, &pads[0], AliMpArea{}, seg);

  return clusterFinder;
}

void message(const char* msg)
{
  std::cout << std::string(10, '-') << msg << "\n";
}

void print(AliMUONCluster& cluster)
{
  cluster.Print();
}

void print(AliMUONPad& pad)
{
  pad.Print();
}

void print(TObjArray& pixels)
{
  for (int i = 0; i <= pixels.GetLast(); i++) {
    AliMUONPad* pad = static_cast<AliMUONPad*>(pixels.At(i));
    print(*pad);
  }
}

YAML::Node clusterize(YAML::Node desc, int detElemId)
{
  YAML::Node node{ desc };

  auto pads = getPads(desc, detElemId);

  if (!pads[0]) {
    std::cerr << "Could not get bending pads !\n";
    exit(1);
  }
  if (!pads[1]) {
    std::cerr << "Could not get non-bending pads !\n";
    exit(1);
  }

  std::unique_ptr<AliMUONClusterFinderMLEMdebug> clusterFinder(getClusterFinder(pads, detElemId));

  AliMUONCluster* preCluster = clusterFinder->fPreClusterFinder->NextCluster();
  message("PRECLUSTER");
  print(*preCluster);
  clusterFinder->fPreCluster = preCluster;

  message("CHECK PRECLUSTER");
  AliMUONCluster* cluster = clusterFinder->CheckPrecluster(*preCluster);
  print(*cluster);

  message("BUILD PIXEL ARRAY ONE CATHODE");
  // clusterFinder->BuildPixArray(*cluster);
  clusterFinder->BuildPixArrayOneCathode(*cluster);
  print(*(clusterFinder->fPixArray));

  auto pixelNode = YAML::convert<TObjArray>::encode((*(clusterFinder->fPixArray)));

  node["steps"].push_back(pixelNode);

  delete pads[0];
  delete pads[1];

  return node;
}

int main(int argc, char** argv)
{
  po::variables_map vm;
  po::options_description generic("Generic options");
  std::string input, output;
  int detElemId;

  // clang-format off
  generic.add_options()
          ("help,h", "produce help message")
          ("input,i", po::value<std::string>(&input), "input cluster(s) (text file in YAML format)")
          ("output,o", po::value<std::string>(&output), "output clustering steps (text file in YAML format)")
          ("deid,d", po::value<int>(&detElemId)->default_value(100),"detection element ID to consider");
  // clang-format on
  po::options_description cmdline;
  cmdline.add(generic);

  po::store(
    po::command_line_parser(argc, argv).options(cmdline).run(),
    vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << cmdline << std::endl;
    return 2;
  }

  if (vm.count("input") == 0) {
    std::cout << "no input file specified" << std::endl;
    std::cout << cmdline << std::endl;
    return 1;
  }

  if (vm.count("output") == 0) {
    std::cout << "no output file specified" << std::endl;
    std::cout << cmdline << std::endl;
    return 1;
  }

  YAML::Node top = readYAML(input.c_str());
  if (top.IsNull()) {
    std::cerr << "Could not read YAML from " << argv[1] << "\n";
    exit(1);
  }

  YAML::Node out;

  YAML::Node clusters = top["clusters"];

  out["deid"] = top["deid"];
  out["clusters"] = {};

  for (int i = 0; i < clusters.size(); i++) {
    auto node = clusterize(clusters[i], detElemId);
    out["clusters"].push_back(node);
  }

  std::ofstream ofile(output);
  ofile << out;
  return 0;
}

