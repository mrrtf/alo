#ifndef FILECREATION_H
#define FILECREATION_H

#include <map>
#include <fstream>
#include <vector>
#include <string>

std::map<int, std::ofstream> createDetElemFiles(const std::string& baseName, const std::vector<int>& detElemIds);

#endif
