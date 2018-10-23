#include "DumpRun2.h"
#include "Run2_generated.h"
#include "boost/format.hpp"
#include <fstream>
#include <iostream>
#include <map>

void dumpPreCluster(const run2::PreCluster &preCluster) {
  std::cout << boost::format("%4d digits") % preCluster.digits()->Length();
}

void dumpCluster(const run2::Cluster &cluster) {
  std::cout << boost::format("\tX %7.2f Y %7.2f Z %7.2f") % cluster.pos()->x() %
                   cluster.pos()->y() % cluster.pos()->z();

  auto pre = cluster.pre();
  std::cout << " | ";
  dumpPreCluster(*pre);
  std::cout << "\n";
}

void dumpRun2(const std::string &filename) {
  std::ifstream in(filename);

  int size;
  int nevents{0};

  while (!in.eof()) {
    in.read(reinterpret_cast<char *>(&size), sizeof(int));
    //     std::cout << "size=" << size << "\n";
    char *buf = new char[size];
    if (in.eof()) {
      continue;
    }

    nevents++;

    in.read(buf, size);

    auto event = run2::GetEvent(buf);

    std::cout << boost::format("EVENT PERIOD %10d BC %10d NTRACKLETS %6f MB %d\n") %
                     event->period() % event->bc() % event->ntracklets() %
                     event->isMB();
    int nclusters = event->clusters()->Length();

    for (auto i = 0; i < nclusters; i++) {
      auto cluster = event->clusters()->Get(i);
      dumpCluster(*cluster);
    }
    delete[] buf;
  }

  std::cout << "nevents=" << nevents << std::endl;
}

