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
/// @author  Laurent Apaduidecetche


#include "segmentation.h"
#include "svgContour.h"
#include "segmentationContours.h"
#include <ostream>
#include <string>
#include <boost/program_options.hpp>

using namespace o2::mch::contour;
using namespace o2::mch::svg;
using namespace o2::mch::mapping;
namespace po = boost::program_options;

void svgSegmentation(const Segmentation &seg, std::ostream &out,
                     bool hidedes, bool hidedualsampas, bool hidepads)
{
  std::vector<Contour<double>> dualSampaContours;
  std::vector<Polygon<double>> polygons;
  std::vector<std::vector<Polygon<double>>> dualSampaPads;

  for (auto i = 0; i < seg.nofDualSampas(); ++i) {
    std::vector<Polygon<double>> pads;
    seg.forEachPadInDualSampa(seg.dualSampaId(i), [&pads, &seg](int paduid) {
      double x = seg.padPositionX(paduid);
      double y = seg.padPositionY(paduid);
      double dx = seg.padSizeX(paduid) / 2.0;
      double dy = seg.padSizeY(paduid) / 2.0;

      pads.emplace_back(Polygon<double>{{x - dx, y - dy},
                                        {x + dx, y - dy},
                                        {x + dx, y + dy},
                                        {x - dx, y + dy},
                                        {x - dx, y - dy}});
    });
    dualSampaPads.push_back(pads);
    dualSampaContours.push_back(o2::mch::contour::createContour(pads));
    for (auto &p : dualSampaContours.back().getPolygons()) {
      polygons.push_back(p);
    }
  }

  auto deContour = o2::mch::contour::createContour(polygons);
  auto box = getBBox(deContour);

  //FIXME: style should be an external CSS for maximal flexibility ?

  out << R"(
<html>
<style>
.pads {
  fill: #EEEEEE;
  stroke-width: 0.025px;
  stroke: #666666;
}
.dualsampas {
  fill:none;
  stroke-width: 0.1px;
  stroke: #333333;
}
.detectionelements {
  fill:none;
  stroke-width:0.2px;
  stroke: #000000;
}
</style>
<body>
)";

  {
    Writer w(out, 1000, box);

    if (hidepads) {
      w.svgGroupStart("pads");
      for (auto &dsp: dualSampaPads) {
        for (auto &p: dsp) {
          w.polygon(p);
        }
      }
      w.svgGroupEnd();
    }

    if (hidedualsampas) {
      w.svgGroupStart("dualsampas");
      for (auto &dsp: dualSampaContours) {
        w.contour(dsp);
      }
      w.svgGroupEnd();
    }

    if (hidedes) {
      w.svgGroupStart("detectionelements");
      w.contour(deContour);
    }
  }

  out << "</body></html>\n";
}


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
      svgSegmentation(seg, out, vm.count("hidedes") == 0,
                      vm.count("hidedualsampas") == 0,
                      vm.count("hidepads") == 0);
    }
  }

  return 0;
}