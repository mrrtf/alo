// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef ALO_MID_FILEHANDLER_H
#define ALO_MID_FILEHANDLER_H

#include <fstream>
#include <map>
#include <boost/archive/binary_oarchive.hpp>

namespace alo
{
namespace mid
{
class FileHandler
{
 public:
  FileHandler(const char* baseFilename);

  boost::archive::binary_oarchive& getArchive(int ifile);
  boost::archive::binary_oarchive& getArchive()
  {
    return getArchive(-1);
  }
  std::string getFilename(int ifile);

 private:
  std::string mBaseFilename;
  std::map<int, std::ofstream> mOutFile;
  std::map<int, boost::archive::binary_oarchive> mOutArchive;
};

} // namespace mid
} // namespace alo
#endif