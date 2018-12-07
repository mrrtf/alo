#include "DumpRun2.h"
#include "Run2_generated.h"
#include "Run2FB2YAML.h"
#include "boost/format.hpp"
#include <fstream>
#include <iostream>
#include <map>

void dumpDigit(const run2::Digit& digit)
{
  std::cout << boost::format(" {%4d,%4d,%2d}") %
                             digit.deid() % digit.manuid() %
                             static_cast<int>(digit.manuchannel());
}

void dumpPreCluster(const run2::PreCluster& preCluster)
{
  std::cout << boost::format("%4d digits") % preCluster.digits()->Length();
  int n = 0;
  for (int i = 0; i < preCluster.digits()->Length(); i++) {
    if (n == 0) {
      std::cout << "\n" << std::string(7,' ');
    }
    dumpDigit(*((preCluster.digits()->Get(i))));
    n++;
    if (n == 5) {
      n = 0;
    }
  }
}

void event2yaml(const flatbuffers::Vector<flatbuffers::Offset<run2::Cluster>>& clusters, int eventID)
{
  std::ofstream ofile("cluster-" + std::to_string(eventID) + ".yaml");
  YAML::Emitter out;
  out << clusters;
  ofile << out.c_str();
  ofile.close();
}

void dumpCluster(const run2::Cluster& cluster)
{
  std::cout << boost::format("\tX %7.2f Y %7.2f Z %7.2f Q %7.2f") % cluster.pos()->x() %
                 cluster.pos()->y() % cluster.pos()->z() % cluster.charge();

  auto pre = cluster.pre();
  std::cout << " | ";
  dumpPreCluster(*pre);
  std::cout << "\n";
}

int dumpRun2(const std::string& filename, int maxevents, bool silent)
{
  std::ifstream in(filename);
  int size;
  int nevents{ 0 };

  while (!in.eof()) {
    in.read(reinterpret_cast<char*>(&size), sizeof(int));
    char* buf = new char[size];
    if (in.eof()) {
      continue;
    }

    nevents++;

    if (nevents > maxevents) {
      break;
    }

    in.read(buf, size);
    auto event = run2::GetEvent(buf);

    if (!silent) {
      std::cout << boost::format("EVENT %6d PERIOD %10d BC %10d NTRACKLETS %6f MB %d\n") % nevents %
                     event->period() % event->bc() % event->ntracklets() %
                     event->isMB();
    }
    int nclusters = event->clusters()->Length();
    if (nclusters > 0) {
      event2yaml(*(event->clusters()), nevents);
    }

    for (auto i = 0; i < nclusters; i++) {
      auto cluster = event->clusters()->Get(i);
      if (!silent) {
        dumpCluster(*cluster);
      }
    }
    delete[] buf;
  }
  return nevents;
}

