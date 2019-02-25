#include <string>
#include <array>
#include <random>
#include "TSystem.h"
#include "AliMpDDLStore.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include "AliMpDataMap.h"
#include "AliMpDataStreams.h"
#include "AliMpDataProcessor.h"
#include "AliMUONGeometryTransformer.h"
#include "boost/program_options.hpp"
#include "json.h"

namespace po = boost::program_options;

int convertFromLegacyDeId(int detElemId)
{
  /// Converts the detection element ID (Run2 format)
  /// into the new ID (Run3 format)
  int ich = (detElemId / 100 - 11);
  int irpc = (detElemId % 100 + 4) % 18;
  if (irpc >= 9) {
    irpc = 17 - irpc;
    ich += 4;
  }
  return ich * 9 + irpc;
}

template <typename WRITER>
void writePoint(const char* key, const std::array<double, 3>& point, WRITER& writer)
{
  writer.Key(key);
  writer.StartArray();
  for (int ic = 0; ic < 3; ++ic) {
    writer.Double(point[ic]);
  }
  writer.EndArray();
}

void generateFile(int npoints)
{
  AliMUONGeometryTransformer geoTrans;

  if (geoTrans.GetNofModuleTransformers() == 0) {
    if (!AliMpDDLStore::Instance()) {
      AliMpDataProcessor mp;
      AliMpDataMap* dataMap = mp.CreateDataMap("data");
      AliMpDataStreams dataStreams(dataMap);
      AliMpDDLStore::ReadData(dataStreams);
    }
    geoTrans.LoadGeometryData(gSystem->ExpandPathName("transform.dat"));
  }

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distX(-127.5, 127.5);
  std::uniform_real_distribution<double> distY(-40., 40.);
  std::uniform_int_distribution<int> distCh(11, 14);
  std::uniform_int_distribution<int> distRPC(0, 17);

  OFC js("test_geometry_list.json");
  auto& writer = js.Writer();
  writer.StartObject();
  writer.Key("testpositions");
  writer.StartArray();

  std::array<Double_t, 3>
    localPoint,
    globalPoint;

  for (int ipt = 0; ipt < npoints; ++ipt) {
    int detElemId = 100 * distCh(mt) + distRPC(mt);
    localPoint[0] = distX(mt);
    localPoint[1] = distY(mt);
    localPoint[2] = 0.;
    geoTrans.Local2Global(detElemId, localPoint[0], localPoint[1], localPoint[2], globalPoint[0], globalPoint[1], globalPoint[2]);
    writer.StartObject();
    writer.Key("deId");
    writer.Int(convertFromLegacyDeId(detElemId));
    writePoint("local", localPoint, writer);
    writePoint("global", globalPoint, writer);
    writer.EndObject();
  }
  writer.EndArray();
  writer.EndObject();
}

int main(int argc, char** argv)
{
  po::variables_map vm;
  po::options_description generic("Generic options");

  int npoints = 100;

  // clang-format off
  generic.add_options()
    ("help", "produce help message")
    ("npoints", po::value<int>(&npoints), "number of points");
  // clang-format on
  po::options_description cmdline;
  cmdline.add(generic);

  po::store(
    po::command_line_parser(argc, argv).options(cmdline).run(),
    vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << cmdline << std::endl;
    return 2;
  }

  generateFile(npoints);

  return 0;
}
