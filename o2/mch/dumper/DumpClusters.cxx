#include "DumpClusters.h"
#include "Run3_generated.h"
#include "SegmentationPair.h"
#include <fstream>
#include <iostream>
#include <map>
#include "boost/format.hpp"

void dumpPreCluster(const o2::mch::PreCluster& preCluster) {
  std::cout << boost::format("%4d B and %4d NB digits") % preCluster.digitBendingIds()->Length() %
          preCluster.digitNonBendingIds()->Length();
}

void dumpCluster(const o2::mch::Cluster& cluster)
{
  std::cout << boost::format("X %7.2f Y %7.2f Z %7.2f") % cluster.pos()->x() % cluster.pos()->y() % cluster.pos()->z();

  auto pre = cluster.pre();
  std::cout << " | ";
  dumpPreCluster(*pre);
  std::cout << "\n";
}

void dumpClusters(const std::string& filename)
{
  std::ifstream in(filename);

  int size;
  int nevents{0};

  while (!in.eof()) {
    in.read(reinterpret_cast<char*>(&size), sizeof(int));
//     std::cout << "size=" << size << "\n";
    char* buf = new char[size];
    if (in.eof()) {
      continue;
    }

    nevents++;

    in.read(buf, size);

    auto clusterDE = o2::mch::GetClusterDE(buf);

    int detElemId = clusterDE->detElemId();

    int nblocks = clusterDE->clusterTimeBlocks()->Length();

  //   std::cout << "DE " << detElemId << " " << nblocks << " blocks\n";

    for (auto i = 0; i < nblocks; i++) {
      auto clusterTB = clusterDE->clusterTimeBlocks()->Get(i);
      for (auto j = 0; j < clusterTB->clusters()->Length(); ++j) {
        auto cluster = clusterTB->clusters()->Get(j);
        dumpCluster(*cluster);
      }
    }
    delete[] buf;
  }

  std::cout << "nevents=" << nevents << std::endl;
}

