// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef ALO_MID_COLUMNDATA_H
#define ALO_MID_COLUMNDATA_H

#include <iostream>
#include <array>

#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>

namespace alo
{
namespace mid
{
struct ColumnData {
  uint8_t deId;                     ///< Index of the detection element
  uint8_t columnId;                 ///< Column in DE
  std::array<uint16_t, 5> patterns; ///< patterns

  friend class boost::serialization::access;

  /// Serializes the struct
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar& deId;
    ar& columnId;
    ar& patterns;
  }
};

//______________________________________________________________________________
std::ostream& operator<<(std::ostream& stream, const ColumnData& columnData)
{
  /// Overload ostream operator
  stream << "DeId " << int(columnData.deId) << "  ColumnId " << int(columnData.columnId) << "  BP:";
  for (int ip = 0; ip < 4; ++ip) {
    stream << " 0x" << std::bitset<16>(columnData.patterns[ip]);
  }
  stream << "  NBP: " << std::bitset<16>(columnData.patterns[4]);
  return stream;
}

} // namespace mid
} // namespace alo
#endif