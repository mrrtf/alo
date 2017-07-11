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

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <AliMpPCB.h>
#include <pcb.h>
#include "mapping.h"
#include <array>
#include <string>
#include <iostream>
#include "pcbnumbers.h"

struct PCBS
{
    PCBS()
    {
      Mapping m;
      pcbs = get_allpcbs(m.ddlStore(), m.mseg());
    }

    std::vector<AliMpPCB*> pcbs;
};

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, PCBS)

BOOST_AUTO_TEST_SUITE(pcbs)

BOOST_AUTO_TEST_CASE(checkNumberOfPCBs)
{
  BOOST_TEST_CHECK(pcbs.size() == 18);
}

BOOST_AUTO_TEST_CASE(checkPCBNames)
{
  for (auto i = 0; i < pcbs.size(); ++i) {
    BOOST_TEST_CHECK((pcbNames[i] == pcbs[i]->GetID()));
  }
}

BOOST_AUTO_TEST_CASE(checkPCBNofPads)
{
  for (auto i = 0; i < pcbs.size(); ++i) {
    BOOST_TEST_CHECK((pcbNofPads[i] == pcbs[i]->NofPads()),
                     "got " << pcbs[i]->NofPads() << " pads for PCB " << pcbs[i]->GetID()
                            << " was expecting " << pcbNofPads[i]);
  }
}

BOOST_AUTO_TEST_CASE(checkPCBNofPadsX)
{
  for (auto i = 0; i < pcbs.size(); ++i) {
    BOOST_TEST_CHECK((pcbNofPadsX[i] == pcbs[i]->GetNofPadsX()),
                     "got " << pcbs[i]->GetNofPadsX() << " pads for PCB " << pcbs[i]->GetID()
                            << " was expecting " << pcbNofPadsX[i]);
  }
}

BOOST_AUTO_TEST_CASE(checkPCBNofPadsY)
{
  for (auto i = 0; i < pcbs.size(); ++i) {
    BOOST_TEST_CHECK((pcbNofPadsY[i] == pcbs[i]->GetNofPadsY()),"got " << pcbs[i]->GetNofPadsY() << " pads for PCB " << pcbs[i]->GetID()
                                                                       << " was expecting " << pcbNofPadsY[i]);
  }
}

BOOST_AUTO_TEST_CASE(checkPCBXlimits,* boost::unit_test::tolerance(1E-4)) {
  for (auto i = 0; i < pcbs.size(); ++i) {
    BOOST_TEST((pcbs[i]->Xmin()==pcbXmin[i]));
    BOOST_TEST((pcbs[i]->Xmax()==pcbXmax[i]));
  }
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

