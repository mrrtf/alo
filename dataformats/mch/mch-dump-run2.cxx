#include "DumpRun2.h"
#include "boost/program_options.hpp"
#include <fstream>
#include <iostream>
#include <map>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  po::variables_map vm;
  po::options_description generic("Generic options");
  int maxevents;
  bool silent;

  // clang-format off
  generic.add_options()
          ("help,h", "produce help message")
          ("max-events,m",po::value<int>(&maxevents)->default_value(100000000),"max number of events")
          ("silent",po::value<bool>(&silent)->default_value(true),"only output to YAML files, no screen output");

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
    std::cout << cmdline << std::endl;
    return 2;
  }
  if (vm.count("input-file") == 0) {
    std::cout << "no input file specified" << std::endl;
    std::cout << cmdline << std::endl;
    return 1;
  }

  std::vector<std::string> inputfiles{
    vm["input-file"].as<std::vector<std::string>>()
  };

  int nevents{ 0 };
  for (auto fileName : inputfiles) {
    nevents += dumpRun2(fileName, maxevents,silent);
    if (nevents > maxevents) {
      break;
    }
  }

  return 0;
}

