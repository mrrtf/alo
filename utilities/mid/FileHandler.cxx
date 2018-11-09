// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "FileHandler.h"
#include <sstream>
#include <iostream>

namespace alo
{
namespace mid
{
FileHandler::FileHandler(const char* baseFilename) : mBaseFilename(baseFilename), mOutFile(), mOutArchive()
{
  /// Constr.
}

boost::archive::binary_oarchive& FileHandler::getArchive(int ifile)
{
  auto entry = mOutArchive.find(ifile);
  if (entry == mOutArchive.end()) {
    std::string filename = getFilename(ifile);
    mOutFile.emplace(ifile, filename.data());
    mOutArchive.emplace(ifile, mOutFile.at(ifile));
  }

  return mOutArchive.at(ifile);
}

std::string FileHandler::getFilename(int ifile)
{
  if (ifile < 0) {
    return mBaseFilename;
  }
  std::stringstream suffix;
  suffix << "_" << ifile;
  std::string filename = mBaseFilename;
  size_t lastDot = mBaseFilename.find_last_of(".");
  if (lastDot == std::string::npos) {
    filename += suffix.str();
  } else {
    suffix << ".";
    filename = filename.replace(lastDot, 1, suffix.str());
  }
  return filename;
}

} // namespace mid
} // namespace alo
