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

#include "segnumbers.h"

std::array<std::string,21> segnames{
  "st1",
  "st2",
  "122000SR1",
  "112200SR2",
  "122200S",
  "222000N",
  "220000N",
  "122000NR1",
  "112200NR2",
  "122200N",
  "122330N",
  "112233NR3",
  "112230N",
  "222330N",
  "223300N",
  "333000N",
  "330000N",
  "112233N",
  "222333N",
  "223330N",
  "333300N"
};

std::array<int, 21> bpads = {
  14392, 13947, 2928, 3568, 3120, 1920, 1280, 3008, 3648, 3200, 3200, 4096, 4160, 2560, 1920, 960, 640, 4480, 2880,
  2240, 1280
};

std::array<int, 21> nbpads = {
  14280, 13986, 2048, 2496, 2176, 1344, 896, 2112, 2560, 2240, 2240, 2880, 2912, 1792, 1344, 672, 448, 3136, 2016,
  1568, 896
};
