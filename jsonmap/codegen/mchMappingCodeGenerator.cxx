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
#include "padGroupType.h"
#include "jsonReader.h"
#include "motifPosition1.h"
#include "motifType.h"
#include "padSize.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "segmentation1.h"
#include "segmentation2.h"
#include "writer.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "padGroupType.h"

using namespace rapidjson;
namespace po = boost::program_options;

using namespace jsonmap::codegen;

int main(int argc, char *argv[])
{
  po::variables_map vm;
  po::options_description generic("Generic options");
  generic.add_options()
    ("help", "produce help message")
    ("detection_elements", "read detection element information")
    ("chambers", "read chamber information")
    ("segmentations", "read segmentation information")
    ("bergs", "read berg connector information")
    ("padsizes", "read padsize information")
    ("motiftypes", "read motif type information");

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

  std::vector<std::string> inputfiles = vm["input-file"].as<std::vector<std::string>>();
  std::map<std::string, std::unique_ptr<InputWrapper> > documents;

  for (auto &&file : inputfiles) {

    for (const auto &opt: generic.options()) {
      if (vm.count(opt->long_name())) {
        std::unique_ptr<InputWrapper> doc = std::make_unique<InputWrapper>(file.c_str());
        if (doc->document().HasMember(opt->long_name().c_str())) {
          documents.insert(std::make_pair(opt->long_name(), std::make_unique<InputWrapper>(file.c_str())));
        }
      }
    }
  }

  if (documents.count("chambers")) {
    readChambers(documents["chambers"]->document());
  }

  if (documents.count("motiftypes")) {
    Document &doc = documents["motiftypes"]->document();
    auto impl = generateCodeForMotifTypes(doc["motiftypes"]);
    outputCode("", impl, "genMotifType");
    impl = generateCodeForPadGroupTypes(doc["motiftypes"]);
    outputCode("",impl,"genPadGroupType");
  }

  if (documents.count("padsizes")) {
    Document &doc = documents["padsizes"]->document();
    outputCode("", generateCodeForPadSizes(doc["padsizes"]), "genPadSize");
  }

  if (documents.count("segmentations") && documents.count("motiftypes") && documents.count("padsizes")
      && documents.count("detection_elements")) {
    Document &segmentations = documents["segmentations"]->document();
    Document &motiftypes = documents["motiftypes"]->document();
    Document &padsizes = documents["padsizes"]->document();
    Document &detection_elements = documents["detection_elements"]->document();
    impl1::generateCodeForSegmentations(segmentations["segmentations"],
                                        motiftypes["motiftypes"],
                                        padsizes["padsizes"],
                                        detection_elements["detection_elements"]);

    impl2::generateCodeForSegmentations(segmentations["segmentations"],
                                        motiftypes["motiftypes"],
                                        padsizes["padsizes"],
                                        detection_elements["detection_elements"]);
  }

  if (documents.count("segmentations") && documents.count("motiftypes") && documents.count("padsizes") &&
      documents.count("bergs")) {
    Document &segmentations = documents["segmentations"]->document();
    Document &motiftypes = documents["motiftypes"]->document();
    Document &padsizes = documents["padsizes"]->document();
    Document &bergs = documents["bergs"]->document();

    impl1::generateCodeForMotifPositions(segmentations["segmentations"],
                                         motiftypes["motiftypes"],
                                         padsizes["padsizes"],
                                         bergs["bergs"]
    );
  }

  return 0;
}

