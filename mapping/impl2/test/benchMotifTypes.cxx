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

#include <cmath>
#include "benchmark/benchmark.h"
#include "motifType.h"
#include "padGroupType.h"
#include <vector>

using namespace o2::mch::mapping::impl1;
using namespace o2::mch::mapping::impl2;

int getIndex(int ix, int iy, int nx)
{
  return ix + iy * nx;
}

PadGroupType convert(const MotifType& mt) {

  std::vector<int> id,ix,iy;

  for ( auto i = 0; i < mt.getNofPads(); ++i ) {
    id.push_back(mt.getBerg(i));
    ix.push_back(mt.getIx(i));
    iy.push_back(mt.getIy(i));
  }

  std::vector<int> fi;

  int ixmax = std::distance(begin(ix), std::max(begin(ix), end(ix)));
  int iymax = std::distance(begin(iy), std::max(begin(iy), end(iy)));

  fi.resize(getIndex(ixmax,iymax,ix.size()), -1);

  for (auto i = 0; i < ix.size(); ++i) {
    fi[getIndex(ix[i], iy[i],ix.size())] = id[i];
  }
  return PadGroupType{ixmax,iymax,fi};
}

std::vector<PadGroupType> convert(const MotifTypeArray& mtArray)
{

  std::vector<PadGroupType> pgt;

  for (auto& mt: mtArray) {
    pgt.push_back(convert(mt));
  }
  return pgt;
}

static void benchMotifType1(benchmark::State &state)
{
  int npads{0};

  for (auto _: state) {
    npads = 0;
    for (auto mtindex = 0; mtindex < 210; mtindex++) {
      auto &mt = arrayOfMotifTypes[mtindex];
      for (int ix = 0; ix <= 27; ++ix) {
        for (int iy = 0; iy <= 63; ++iy) {
          int p = mt.padIdByIndices(ix, iy);
          if (p >= 0) {
            ++npads;
          }
        }
      }
    }
  }
  state.counters["npads"] = npads;
}

static void benchPadGroupType(benchmark::State &state)
{
  int npads{0};

  std::vector<PadGroupType> padGroupTypes = convert(arrayOfMotifTypes);
  for (auto _: state) {
    npads = 0;
    for (auto& pgt : padGroupTypes) {
      for (int ix = 0; ix <= 27; ++ix) {
        for (int iy = 0; iy <= 63; ++iy) {
          int p = pgt.id(ix, iy);
          if (p >= 0) {
            ++npads;
          }
        }
      }
    }
  }
  state.counters["npads"] = npads;
}

BENCHMARK(benchMotifType1)->Unit(benchmark::kMicrosecond);
BENCHMARK(benchPadGroupType)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN()

