#include "DumpRun2.h"
#include "boost/program_options.hpp"
#include <fstream>
#include <iostream>
#include <map>

namespace po = boost::program_options;

int main(int argc, char **argv) {
  po::variables_map vm;
  po::options_description generic("Generic options");

  // clang-format off
  generic.add_options()
          ("help,h", "produce help message");

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
      vm["input-file"].as<std::vector<std::string>>()};

  for (auto fileName : inputfiles) {
    dumpRun2(fileName);
  }

  return 0;
}

