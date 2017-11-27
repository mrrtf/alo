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


#ifndef O2_MCH_SVG_SEGMENTATIONINTERFACE_H
#define O2_MCH_SVG_SEGMENTATIONINTERFACE_H

namespace o2 {
namespace mch {
namespace svg {

void writeSegmentationInterface(const o2::mch::mapping::SegmentationInterface &seg, const char *filename, double x, double y);

}}}

#endif
