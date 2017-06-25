//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "boost/program_options.hpp"
#include "chamber.h"
#include "codeWriter.h"
#include "detectionElement.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <string>
#include <vector>

using namespace rapidjson;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {

  po::variables_map vm;
  try {
    po::options_description generic("Generic options");
    generic.add_options()
        ("help", "produce help message")
        ("de", "read detection element information")
        ("ch", "read chamber information");

    po::options_description hidden("hidden options");
    hidden.add_options()
        ("input-file", po::value<std::vector<std::string> >(), "input file");

    po::options_description cmdline;
    cmdline.add(generic).add(hidden);

    po::positional_options_description p;
    p.add("input-file", -1);

    po::store(po::command_line_parser(argc, argv).
        options(cmdline).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << generic << std::endl;
      return 2;
    }
    if (vm.count("input-file") == 0) {
      std::cout << "no input file specified" << std::endl;
      return 1;
    }
  }
  catch(std::exception& e)
  {
    std::cout << e.what() << "\n";
    return 1;
  }

  std::vector<std::string> inputfiles = vm["input-file"].as<std::vector<std::string>>();

  for ( auto&& file : inputfiles) {
    if (vm.count("ch")) {
      readChambers(file);
    }

    if (vm.count("de")) {
      std::pair<std::string,std::string> code = readDetectionElements(file);
      outputCode(code.first,code.second,"de");
    }
  }
  return 0;
}
