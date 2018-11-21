#include "RawToMID.h"
#include "FileHandler.h"
#include "boost/program_options.hpp"
#include <iostream>
#include <vector>
#include "AliCDBManager.h"
#include "AliMpCDB.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  po::variables_map vm;
  po::options_description generic("Generic options");
  std::string outFilename = "digits_mid.dat";
  std::string ocdb = "local://$ALIROOT_OCDB_ROOT/OCDB";
  int runNumber = 0;

  // clang-format off
  generic.add_options()
          ("help", "produce help message")
          ("output", po::value<std::string>(&outFilename),"basename of output track refs file")
          ("ocdb", po::value<std::string>(&ocdb),"ocdb")
          ("runNumber", po::value<int>(&runNumber),"run number");


  po::options_description hidden("hidden options");
  hidden.add_options()
          ("input", po::value<std::vector<std::string>>(),"path to simulation");
  // clang-format on

  po::options_description cmdline;
  cmdline.add(generic).add(hidden);

  po::positional_options_description pos;
  pos.add("input", -1);

  po::store(
    po::command_line_parser(argc, argv).options(cmdline).positional(pos).run(),
    vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << generic << std::endl;
    return 2;
  }
  if (vm.count("input") == 0) {
    std::cout << "no input file specified" << std::endl;
    return 1;
  }

  std::vector<std::string> inputfiles{
    vm["input"].as<std::vector<std::string>>()
  };

  AliCDBManager::Instance()->SetDefaultStorage(ocdb.c_str());
  AliCDBManager::Instance()->SetRun(runNumber);
  AliCDBManager::Instance()->SetSpecificStorage("MUON/Calib/MappingData", "local://$ALIROOT_OCDB_ROOT/OCDB"); // Save time

  AliMpCDB::LoadDDLStore();

  alo::mid::FileHandler fileHandler(outFilename.data());

  for (auto input : inputfiles) {
    std::cout << "Converting file " << input << "\n";
    rawToMID(input.c_str(), fileHandler);
  }

  return 0;
}
