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

///
/// @author  Laurent Aphecetche

#include "boost/program_options.hpp"
#include "segmentation.h"
#include "segmentationContours.h"
#include "segmentationSVGWriter.h"
#include "svgWriter.h"
#include <fstream>
#include <iostream>

using namespace o2::mch::mapping;

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
  std::string prefix;
  std::vector<int> detElemIds;
  po::variables_map vm;
  po::options_description generic("Generic options");

  generic.add_options()
    ("help", "produce help message")
    ("hidepads", "hide pad outlines")
    ("hidedualsampas", "hide dualsampa outlines")
    ("hidedes", "hide detection element outline")
    ("de", po::value<std::vector<int>>(&detElemIds), "which detection element to consider")
    ("prefix", po::value<std::string>(&prefix)->default_value("seg"), "prefix used for outfile filename(s)")
    ("all", "use all detection elements");

  po::options_description cmdline;
  cmdline.add(generic);

  po::store(po::command_line_parser(argc, argv).options(cmdline).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << generic << "\n";
    return 2;
  }

  if (vm.count("de") && vm.count("all")) {
    std::cout << "--all and --de options are mutually exclusive. --all will be used\n";
    detElemIds.clear();
  }

  if (vm.count("all")) {
    o2::mch::mapping::forOneDetectionElementOfEachSegmentationType([&detElemIds](int detElemId) {
      detElemIds.push_back(detElemId);
    });
  }

  if (detElemIds.empty()) {
    std::cout << "Must give at least one detection element id to work with\n";
    std::cout << generic << "\n";
    return 3;
  }

  for (auto &detElemId: detElemIds) {
    for (auto isBendingPlane : {true, false}) {
      std::ofstream out(
        vm["prefix"].as<std::string>() + "-" + std::to_string(detElemId) + "-" + (isBendingPlane ? "B" : "NB") +
        ".html");
      Segmentation seg{detElemId, isBendingPlane};
      o2::mch::contour::SVGWriter w(getBBox(seg));
      w.addStyle(svgSegmentationDefaultStyle());
      svgSegmentation(seg, w, vm.count("hidedes") == 0,
                      vm.count("hidedualsampas") == 0,
                      vm.count("hidepads") == 0);
      w.writeHTML(out);
    }
  }

  return 0;
}
