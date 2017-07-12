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

///
/// @author  Laurent Aphecetche


#ifndef ALO_JSONMAP_CREATOR_TEST_PCB_H
#define ALO_JSONMAP_CREATOR_TEST_PCB_H

#include <array>
#include <string>

std::array<std::string, 18> pcbNames{
  "B1",
  "B2",
  "B3+",
  "B3-",
  "N1",
  "N2+",
  "N2-",
  "N3",
  "R1B",
  "R1N",
  "R2B",
  "R2N",
  "R3B",
  "R3N",
  "S2B+",
  "S2B-",
  "S2N",
  "S2N-"
};

std::array<int, 18> pcbNofPads{
  1280, 640, 320, 320, 896, 448, 448, 224, 448, 320, 1088, 768, 896, 640, 560, 560, 384, 384
};

std::array<int, 18> pcbNofPadsX{16, 8, 4, 4, 56, 56, 56, 56, 7, 26, 16, 56, 16, 56, 7, 7, 48, 48};
std::array<int, 18> pcbNofPadsY{80, 80, 80, 80, 16, 8, 8, 4, 80, 16, 80, 16, 80, 16, 80, 80, 8, 8};
std::array<double,18> pcbXmin{80,40,0,120,80,40,40,120,120,120,120,120,200,200,120,0,120,0};
std::array<double,18> pcbXmax{120,80,40,160,120,80,80,160,160,160,160,160,240,240,160,40,160,40};

#endif
