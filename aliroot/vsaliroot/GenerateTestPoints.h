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


#ifndef O2_MCH_MAPPING_IMPL1_GENERATETESTPOINTS_H
#define O2_MCH_MAPPING_IMPL1_GENERATETESTPOINTS_H

#include <vector>
#include <utility>

namespace o2 {
namespace mch {
namespace mapping {

std::vector<std::pair<double, double>>
generateTestPoints(int n, double xmin, double ymin, double xmax, double ymax, int extent);
}
}
}
#endif
