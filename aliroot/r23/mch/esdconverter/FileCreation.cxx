#include <fstream>
#include <map>
#include <vector>
#include <string>

std::map<int, std::ofstream> createDetElemFiles(const std::string& baseName, const std::vector<int>& detElemIds)
{
  std::map<int, std::ofstream> outputFiles;

  if (baseName.size() > 0) {
    for (auto detElemId : detElemIds) {
      outputFiles[detElemId].open(baseName + std::to_string(detElemId) + ".dat");
    }
  }
  return outputFiles;
}
