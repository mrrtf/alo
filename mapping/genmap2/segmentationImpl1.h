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


#ifndef O2_MCH_MAPPING_SEGMENTATIONIMPL1_H
#define O2_MCH_MAPPING_SEGMENTATIONIMPL1_H

#include "segmentationInterface.h"

namespace o2 {
namespace mch {
namespace mapping {

class SegmentationImpl1 : public SegmentationInterface
{
  public:
    int getId() const override
    { return 1; }

    bool isBendingPlane() const override
    { return true; }

    int nofDualSampas() const override
    { return 0; }

    int nofPads() const override
    { return 0; }

    int getSampaId(int sampaIndex) const override
    { return -1; }

    bool hasPadByPosition(double x, double y) const override
    { return false; }

    bool hasPadByFEE(int dualSampaId, int dualSampaChannel) const override
    { return false; }

    std::vector<int> getPads(int dualSampaId) const override
    { return {}; }

    void getPadPosition(int ph, double &x, double &y) const override
    {}

    void getPadDimension(int ph, double &x, double &y) const override
    {}

};

}}}

#endif
