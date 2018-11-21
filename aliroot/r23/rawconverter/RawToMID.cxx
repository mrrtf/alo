// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "RawToMID.h"

#include <iostream>
#include <vector>
#include <map>
#include <bitset>

#include "ColumnData.h"
#include "ConvertDE.h"
#include "FileHandler.h"

// AliRoot
#include "AliRawReader.h"
#include "AliMUONDigitMaker.h"
#include "AliMUONVDigit.h"
#include "AliMUONDigitStoreV2R.h"
#include "AliMUONTriggerStoreV1.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

//______________________________________________________________________________
void decodeDigit(UInt_t uniqueId, int& detElemId, int& boardId, int& strip, int& cathode)
{
  /// Decodes the digit given as a uniqueId in the Run2 format
  detElemId = uniqueId & 0xFFF;
  boardId = (uniqueId & 0xFFF000) >> 12;
  strip = (uniqueId & 0x3F000000) >> 24;
  cathode = (uniqueId & 0x40000000) >> 30;
}

//______________________________________________________________________________
void boardToPattern(int boardId, int detElemId, int cathode, int& deId, int& column, int& line)
{
  /// Converts old Run2 local board Id into the new format
  deId = alo::mid::convertFromLegacyDeId(detElemId);
  int iboard = (boardId - 1) % 117;
  int halfBoardId = iboard + 1;
  int endBoard[7] = { 16, 38, 60, 76, 92, 108, 117 };
  for (int icol = 0; icol < 7; ++icol) {
    if (halfBoardId > endBoard[icol]) {
      continue;
    }
    column = icol;
    break;
  }
  line = 0;

  if (cathode == 1) {
    return;
  }

  std::vector<int> lines[3];
  lines[0] = { 3, 19, 41, 63, 79, 95, 5, 21, 43, 65, 81, 97, 7, 23, 45, 67, 83, 99, 27, 49, 69,
               85, 101, 9, 31, 53, 71, 87, 103, 13, 35, 57, 73, 89, 105, 15, 37, 59, 75, 91, 107 };
  lines[1] = { 8, 24, 46, 28, 50, 10, 32, 54 };
  lines[2] = { 25, 47, 29, 51, 11, 33, 55 };
  for (int il = 0; il < 3; ++il) {
    for (auto& val : lines[il]) {
      if (halfBoardId == val) {
        line = il + 1;
        return;
      }
    }
  }
}

//______________________________________________________________________________
std::map<int,std::vector<alo::mid::ColumnData>> digitsToColumnData(const AliMUONVDigitStore& digitStore)
{
  /// Converts digits in the old Run2 format to ColumnData
  int detElemId, boardId, channel, cathode, icolumn, iline;
  int deId;
  std::map<int,std::vector<alo::mid::ColumnData>> columnsMap;
  TIter next(digitStore.CreateTriggerIterator());
  AliMUONVDigit* digit;
  while ((digit = static_cast<AliMUONVDigit*>(next()))) {
    decodeDigit(digit->GetUniqueID(), detElemId, boardId, channel, cathode);
    boardToPattern(boardId, detElemId, cathode, deId, icolumn, iline);
    auto itCol = columnsMap[deId].begin();
    for ( ; itCol != columnsMap[deId].end(); ++itCol ) {
      if (itCol->columnId == icolumn) {
        break;
      }
    }
    if (itCol == columnsMap[deId].end()) {
      columnsMap[deId].emplace_back(alo::mid::ColumnData{ (uint8_t)deId, (uint8_t)icolumn });
      itCol = columnsMap[deId].end();
      --itCol;
    }
    itCol->patterns[(cathode == 1) ? 4 : iline] |= (1 << channel);
  }
  return columnsMap;
}

//______________________________________________________________________________
void rawToMID(const char* inputName, alo::mid::FileHandler& fileHandler)
{
  AliRawReader* rawReader = AliRawReader::Create(inputName);
  rawReader->Select("MUONTRG", 0, 1);

  AliMUONDigitMaker digitMaker;
  digitMaker.SetMakeTrackerDigits(false);
  digitMaker.SetMakeTriggerDigits(true);

  AliMUONDigitStoreV2R digitStore;
  AliMUONTriggerStoreV1 triggerStore;

  while (rawReader->NextEvent()) {
    digitMaker.Raw2Digits(rawReader, &digitStore, &triggerStore);
    auto columnsMap = digitsToColumnData(digitStore);
    for ( auto& entry : columnsMap ) {
      fileHandler.getArchive(entry.first) << entry.second;
    }
  }
}
