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

#include "segmentationImpl1.h"
#include <array>
#include <vector>
#include <set>

namespace o2 {
namespace mch {
namespace mapping {

namespace impl1 {

std::unique_ptr<SegmentationInterface> getSegmentationByDetElemIndex(int deIndex, bool isBendingPlane);

std::vector<int> getDetElemIds()
{
  return {
    100, 101, 102, 103, 200, 201, 202, 203, 300, 301, 302, 303, 400, 401, 402, 403, 500, 501, 502, 503, 504, 505, 506,
    507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611,
    612, 613, 614, 615, 616, 617, 700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716,
    717, 718, 719, 720, 721, 722, 723, 724, 725, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813,
    814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910,
    911, 912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923, 924, 925, 1000, 1001, 1002, 1003, 1004, 1005, 1006,
    1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 1025
  };
}

namespace {
std::array<int, 156> segtype{
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 3, 4, 5, 6, 6, 5, 4, 3, 7, 8, 9, 5, 6,
  6, 5, 9, 8, 7, 8, 9, 5, 6, 6, 5, 9, 8, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16,
  16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14,
  13, 12, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11,
  10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11
};
}

int getSegTypeIndexFromDetElemIndex(int deIndex)
{
  if (deIndex >= segtype.size() || deIndex < 0) { throw std::out_of_range("deIndex is incorrect"); }
  return segtype[deIndex];
}

int getDetElemIdFromDetElemIndex(int deIndex)
{
  static auto deids = getDetElemIds();
  if (deIndex < 0 || deIndex >= deids.size()) { throw std::out_of_range("deIndex out of range"); }
  return deids[deIndex];
}

int getDetElemIndexFromDetElemId(int deId)
{
  auto deids = getDetElemIds();
  return std::distance(deids.begin(), std::find(deids.begin(), deids.end(), deId));
}

std::vector<int> getOneDetElemIdPerSegmentation()
{
  // return {100,300,500,501,502,503,504,600,601,602,700,701,702,703,704,705,706,902,903,904,905};

  std::vector<int> des;
  std::set<int> segtypeset;
  auto deids{getDetElemIds()};
  for (auto i = 0; i < deids.size(); ++i) {
    for (auto bending : {true, false}) {
      int id = segtype[i];
      if (std::find(cbegin(segtypeset), cend(segtypeset), id) == segtypeset.end()) {
        segtypeset.insert(id);
        des.push_back(deids[i]);
      }
    }
  }
  return des;
}


std::unique_ptr<SegmentationInterface> getSegmentationByType(int segtype, bool isBendingPlane)
{
  return std::make_unique<SegmentationImpl1>();
}

std::unique_ptr<SegmentationInterface> getSegmentationByDetElemIndex(int deIndex, bool isBendingPlane)
{
  return getSegmentationByType(getSegTypeIndexFromDetElemIndex(deIndex), isBendingPlane);
}

}

template<>
std::unique_ptr<SegmentationInterface>
getSegmentation<SegmentationImplementation::Simple>(int detElemId, bool isBendingPlane)
{
  return impl1::getSegmentationByDetElemIndex(impl1::getDetElemIndexFromDetElemId(detElemId), isBendingPlane);
}

}
}
}
