// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "HvToMID.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/array.hpp>

#include "TMap.h"
#include "TObjArray.h"
#include "AliCDBManager.h"
#include "AliCDBEntry.h"
#include "AliDCSValue.h"

//______________________________________________________________________________
void hvToMID(const char* outFilename)
{
  AliCDBEntry* entry = AliCDBManager::Instance()->Get("MUON/Calib/TriggerDCS");
  TMap* dcsMap = static_cast<TMap*>(entry->GetObject());

  std::array<double, 72> hvArray;

  for (int iside = 0; iside < 2; ++iside) {
    std::string sideName = (iside == 0) ? "IN" : "OUT";
    for (int ist = 0; ist < 2; ++ist) {
      for (int ich = 0; ich < 2; ++ich) {
        for (int irpc = 0; irpc < 9; ++irpc) {
          std::stringstream ss;
          ss << "MTR_" << sideName << "SIDE_MT" << ist + 1 << ich + 1 << "_RPC" << irpc + 1 << "_HV.vEff";
          TPair* dcsPair = static_cast<TPair*>(dcsMap->FindObject(ss.str().data()));
          if (!dcsPair) {
            std::cerr << "Error: cannot find " << ss.str() << std::endl;
            continue;
          }
          TObjArray* values = static_cast<TObjArray*>(dcsPair->Value());
          TIter nextVal(values);
          AliDCSValue* val = nullptr;
          UInt_t previousTS = 0;
          double hv = 0.;
          double elapsed = 0;
          while ((val = static_cast<AliDCSValue*>(nextVal()))) {
            if (previousTS != 0) {
              double delta = (double)(val->GetTimeStamp() - previousTS);
              hv += val->GetFloat() * delta;
              elapsed += delta;
            }
            previousTS = val->GetTimeStamp();
          }

          int deId = 36 * iside + 9 * (2 * ist + ich) + irpc;
          hvArray[deId] = hv / elapsed;
        } // loop on RPC
      }   // loop on chamber in station
    }     // loop on stations
  }       // loop on side

  std::ofstream outFile(outFilename);
  boost::archive::binary_oarchive oa(outFile);
  oa << hvArray;
}
