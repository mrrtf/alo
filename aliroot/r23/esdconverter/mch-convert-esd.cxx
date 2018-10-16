#include "AliMpDEIterator.h"
#include "AliMpDEManager.h"
#include "ConvertESD.h"
#include "FileCreation.h"
#include "MappingHelper.h"
#include "boost/program_options.hpp"
#include <iostream>
#include <vector>

namespace po = boost::program_options;

int main(int argc, char **argv) {
  po::variables_map vm;
  po::options_description generic("Generic options");
  std::string digitBasename;
  std::string clusterBasename;
  std::vector<int> detElemIds;

  // clang-format off
  generic.add_options()
          ("help", "produce help message")
          ("detelemids", po::value<std::vector<int>>(&detElemIds), "produce digit for those detetcion elements (all if not specified)")
          ("cluster-basename", po::value<std::string>(&clusterBasename),"basename of output cluster files")
          ("digit-basename", po::value<std::string>(&digitBasename),"basename of output digit files");

  po::options_description hidden("hidden options");
  hidden.add_options()
          ("input-file", po::value<std::vector<std::string>>(),"input file");
  // clang-format on

  po::options_description cmdline;
  cmdline.add(generic).add(hidden);

  po::positional_options_description p;
  p.add("input-file", -1);

  po::store(
      po::command_line_parser(argc, argv).options(cmdline).positional(p).run(),
      vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << generic << std::endl;
    return 2;
  }
  if (vm.count("input-file") == 0) {
    std::cout << "no input file specified" << std::endl;
    return 1;
  }

  std::vector<std::string> inputfiles{
      vm["input-file"].as<std::vector<std::string>>()};

  if (detElemIds.empty()) {
    std::cout << "no detection element specified : using all of them\n";
    getDDLStore();
    AliMpDEIterator it;
    it.First();
    int detElemId;
    while (!it.IsDone()) {
            auto detElemId = it.CurrentDEId();
      if (AliMpDEManager::GetStationType(detElemId) != AliMp::kStationTrigger) {
        detElemIds.push_back(detElemId);
      }
      it.Next();
    }
  }

  // create segmentations
  SegmentationMap segmentations{getSegmentations(detElemIds)};

  // create output files
  auto digitFiles = createDetElemFiles(digitBasename, detElemIds);
  auto clusterFiles = createDetElemFiles(clusterBasename, detElemIds);

  for (auto input : inputfiles) {
    std::cout << "Converting file " << input << "\n";
    convertESD(segmentations, input.c_str(), clusterFiles, digitFiles);
  }
  return 0;
}

