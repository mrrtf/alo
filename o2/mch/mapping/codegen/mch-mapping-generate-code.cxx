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
#include "jsonReader.h"
#include "goImplementation.h"
#include "motifPosition1.h"
#include "motifType.h"
#include "padGroupType.h"
#include "padSize.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "cathodeSegmentation1.h"
#include "cathodeSegmentation2.h"
#include "writer.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace rapidjson;
namespace po = boost::program_options;

using namespace jsonmap::codegen;

void GenerateGoImpl(std::map<std::string, std::unique_ptr<InputWrapper>>& documents)
{
  if (documents.count("catsegs") && documents.count("motiftypes") &&
      documents.count("padsizes") && documents.count("detection_elements") && documents.count("bergs")) {
    Document& motiftypes = documents["motiftypes"]->document();
    Document& padsizes = documents["padsizes"]->document();
    Document& bergs = documents["bergs"]->document();
    Document& catsegs = documents["catsegs"]->document();
    Document& detection_elements = documents["detection_elements"]->document();
    go::generateDetElemId2SegType(catsegs["catsegs"],
                                  detection_elements["detection_elements"]);
    go::generateCodeForCathodeSegmentations(catsegs["catsegs"],
                                            motiftypes["motiftypes"],
                                            padsizes["padsizes"],
                                            detection_elements["detection_elements"],
                                            bergs["bergs"]);
  }
}

void GenerateImpl1(std::map<std::string, std::unique_ptr<InputWrapper>>& documents)
{
  if (documents.count("motiftypes")) {
    Document& motiftypes = documents["motiftypes"]->document();
    outputCode("", generateCodeForMotifTypes(motiftypes["motiftypes"]), "MotifType");
  }
  if (documents.count("padsizes")) {
    Document& padsizes = documents["padsizes"]->document();
    outputCode("", generateCodeForPadSizes("impl1", padsizes["padsizes"]), "PadSize", true, true);
  }

  if (documents.count("catsegs") &&
      documents.count("motiftypes") &&
      documents.count("padsizes") &&
      documents.count("detection_elements")) {
    Document& catsegs = documents["catsegs"]->document();
    Document& motiftypes = documents["motiftypes"]->document();
    Document& padsizes = documents["padsizes"]->document();
    Document& detection_elements = documents["detection_elements"]->document();
    std::cout << "impl1 : will generate code for CathodeSegmentations\n";
    impl1::generateCodeForCathodeSegmentations(catsegs["catsegs"],
                                               motiftypes["motiftypes"],
                                               padsizes["padsizes"],
                                               detection_elements["detection_elements"]);
  }

  if (documents.count("catsegs") &&
      documents.count("motfitypes") &&
      documents.count("padsizes") &&
      documents.count("bergs")) {
    Document& catsegs = documents["catsegs"]->document();
    Document& motiftypes = documents["motiftypes"]->document();
    Document& padsizes = documents["padsizes"]->document();
    Document& bergs = documents["bergs"]->document();
    impl1::generateCodeForMotifPositions(catsegs["catsegs"],
                                         motiftypes["motiftypes"],
                                         padsizes["padsizes"],
                                         bergs["bergs"]);
  }
}

void GenerateImpl2(std::map<std::string, std::unique_ptr<InputWrapper>>& documents)
{
  if (documents.count("padsizes")) {
    Document& padsizes = documents["padsizes"]->document();
    outputCode("", generateCodeForPadSizes("impl2", padsizes["padsizes"]), "PadSize", true,
               true);
  }

  if (documents.count("motiftypes") &&
      documents.count("bergs")) {
    Document& motiftypes = documents["motiftypes"]->document();
    Document& bergs = documents["bergs"]->document();
    outputCode("", impl2::generateCodeForPadGroupTypes(motiftypes["motiftypes"], bergs["bergs"]), "PadGroupType");
  }

  if (documents.count("catsegs") &&
      documents.count("motiftypes") &&
      documents.count("padsizes") &&
      documents.count("detection_elements") &&
      documents.count("bergs")) {
    Document& catsegs = documents["catsegs"]->document();
    Document& motiftypes = documents["motiftypes"]->document();
    Document& padsizes = documents["padsizes"]->document();
    Document& detection_elements = documents["detection_elements"]->document();
    Document& bergs = documents["bergs"]->document();
    impl2::generateCodeForCathodeSegmentations("impl2", catsegs["catsegs"],
                                               motiftypes["motiftypes"],
                                               padsizes["padsizes"],
                                               detection_elements["detection_elements"],
                                               bergs["bergs"]);
  }
}

void showMissing(std::map<std::string, std::unique_ptr<InputWrapper>>& documents,
                 std::vector<std::string> what)
{
  for (auto w : what) {
    std::cout << "Missing " << w << " : " << (documents.count(w.c_str()) ? "no" : "yes")
              << "\n";
  }
}
void GenerateImpl3(std::map<std::string, std::unique_ptr<InputWrapper>>& documents, const char* implName = "impl3")
{
  if (documents.count("catsegs") &&
      documents.count("motiftypes") &&
      documents.count("padsizes") &&
      documents.count("detection_elements") &&
      documents.count("bergs")) {
    Document& catsegs = documents["catsegs"]->document();
    Document& motiftypes = documents["motiftypes"]->document();
    Document& padsizes = documents["padsizes"]->document();
    Document& detection_elements = documents["detection_elements"]->document();
    Document& bergs = documents["bergs"]->document();
    impl2::generateCodeForCathodeSegmentations(implName, catsegs["catsegs"],
                                               motiftypes["motiftypes"],
                                               padsizes["padsizes"],
                                               detection_elements["detection_elements"],
                                               bergs["bergs"]);
  } else {
    std::cout << "Missing some info to work : \n";
    showMissing(documents, { "catsegs", "motiftypes", "padsizes", "detection_elements", "bergs" });
  }
}

void GenerateImpl4(std::map<std::string, std::unique_ptr<InputWrapper>>& documents)
{
  return GenerateImpl3(documents, "impl4");
}

int main(int argc, char* argv[])
{
  po::variables_map vm;
  po::options_description generic("Generic options");
  int implToUse{ -1 };

  // clang-format off
  generic.add_options()
          ("help", "produce help message")
          ("detection_elements", "read detection element information")
          ("chambers", "read chamber information")
          ("catsegs", "read cathode segmentation information")
          ("bergs", "read berg connector information")
          ("padsizes", "read padsize information")
          ("motiftypes", "read motif type information")
          ("impl", po::value<int>(&implToUse), "implementation to generate")
          ("go", "generate Go interface");
  // clang-format on

  po::options_description hidden("hidden options");
  hidden.add_options()("input-file", po::value<std::vector<std::string>>(), "input file");

  po::options_description cmdline;
  cmdline.add(generic).add(hidden);

  po::positional_options_description p;
  p.add("input-file", -1);

  po::store(po::command_line_parser(argc, argv).options(cmdline).positional(p).run(), vm);
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
  std::map<std::string, std::unique_ptr<InputWrapper>> documents;

  for (auto&& file : inputfiles) {

    for (const auto& opt : generic.options()) {
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

  if (vm.count("go")) {
    GenerateGoImpl(documents);
  }

  if (implToUse == 1) {
    GenerateImpl1(documents);
  } else if (implToUse == 2) {
    GenerateImpl2(documents);
  } else if (implToUse == 3) {
    GenerateImpl3(documents);
  } else if (implToUse == 4) {
    GenerateImpl4(documents);
  }
  return 0;
}
