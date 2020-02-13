#define BOOST_TEST_MODULE mch esdconvertor
#define BOOST_TEST_DYN_LINK
#include "FileCreation.h"
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <iostream>

std::string uniqueBaseName(const char *sbase) {
  std::string base{sbase};
  base += boost::filesystem::unique_path().native();
  base += "-";
  return base;
}

BOOST_AUTO_TEST_CASE(fileCreationShouldCreateDistinctFiles) {
  std::vector<int> detElemIds{100, 101, 102, 103};

  std::string file{uniqueBaseName("testDE-")};

  auto fileMap = createDetElemFiles(file.c_str(), detElemIds);

  file += "101.dat";

  std::cout << file << "\n";

  std::remove(file.c_str());

  for (auto detElemId : detElemIds) {
    try {
      fileMap[detElemId].write(reinterpret_cast<char *>(&detElemId),
                               sizeof(detElemId));
    } catch(...) {
    };
  }

  BOOST_TEST_REQUIRE(fileMap.size() == 4);

  for (auto &f : fileMap) {
    std::cout << f.first << "\n";
  }
}

