#include "DumpDigits.h"
#include "DumpClusters.h"
#include "SegmentationPair.h"
#include "boost/program_options.hpp"
#include <fstream>
#include <iostream>
#include <map>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  po::variables_map vm;
  po::options_description generic("Generic options");
  std::string digits;
  std::string clusters;

  // clang-format off
  generic.add_options()
          ("help", "produce help message")
          ("clusters", po::value<std::string>(&clusters),"clusters file")
          ("digits", po::value<std::string>(&digits),"digits file");

  po::options_description hidden("hidden options");
  hidden.add_options()
          ("input-file", po::value<std::vector<std::string>>(),"input file");
  // clang-format on

  po::options_description cmdline;
  cmdline.add(generic).add(hidden);

  po::store(
    po::command_line_parser(argc, argv).options(cmdline).run(),
    vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << generic << std::endl;
    return 2;
  }

  if (digits.size() > 0) {
    dumpDigits(digits);
  }

  if (clusters.size() > 0) {
    dumpClusters(clusters);
  }

  return 0;
}

