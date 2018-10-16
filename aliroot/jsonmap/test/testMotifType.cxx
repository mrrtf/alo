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
#include "mapping.h"
#include <array>
#include <string>
#include <iostream>
#include <pcb.h>
#include "motifType.h"
#include "seg.h"
#include "AliMpMotifType.h"
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <AliMpPad.h>
#include "AliMpVPadIterator.h"
#include "AliMpConnection.h"
#include <limits>
#include <vector>

std::array<std::string, 210> motifTypeNames{
  "1BA", "1BB", "1BC", "1BD", "1BE", "1BF", "1BG", "1BH", "1BI", "1NA", "1NB", "1NC", "1ND", "1NE", "1NF", "1NG", "1NH",
  "1NI", "1NJ", "1NK", "1NL", "1NM", "1NN", "2BA", "2BB", "2BC", "2BD", "2BE", "2BF", "2BG", "2BH", "2BI", "2BJ", "2BK",
  "2BL", "2BM", "2BN", "2BO", "2BP", "2BQ", "2BR", "2BS", "2BT", "2BU", "2BV", "2BW", "2BX", "2BY", "2Ba1", "2Bb1",
  "2Bc1", "2Bd1", "2Be1", "2Bf1", "2Bg1", "2Bh1", "2Bi1", "2Bm1", "2Bn1", "2Bp1", "2Bt1", "2Bu1", "2Bv1", "2Bv2",
  "2Bv3", "2Bv4", "2Bv5", "2NA", "2NB", "2NC", "2NE", "2NF", "2NG", "2NH", "2NI", "2NJ", "2NK", "2NL", "2NM", "2NN",
  "2Na1", "2Nb1", "2Nc1", "2Nd1", "2Ne1", "2Nf1", "2Ng1", "2Ni1", "2Nj1", "2Nk1", "2Nl1", "2Nm1", "2Nn1", "2No1",
  "2Np1", "2Nq1", "2Nr1", "2Ns1", "2Nt1", "2Nu1", "2Nv1", "2Nv2", "2Nv3", "2Nv4", "2Nv5", "2Nw1", "A1", "A10", "A11",
  "A12", "A13", "A14", "A15", "A16", "A17", "A18", "A19", "A2", "A20", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "C1",
  "C10", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "E1", "E10", "E11", "E12", "E13", "E14", "E15", "E16", "E17",
  "E18", "E19", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "I1", "L1", "L10", "L18", "L19", "L2", "L20", "L3",
  "L4", "L5", "L6", "L7", "L8", "L9", "O1", "O10", "O11", "O12", "O2", "O21", "O22", "O23", "O24", "O25", "O26", "O27",
  "O3", "O4", "O5", "O6", "O7", "O8", "O9", "P1", "P2", "P3", "P4", "Q1", "Q2", "Q3", "Q4", "S0", "S1", "S2", "S3",
  "S4", "S5", "S6", "S7", "S8", "Z1", "Z2", "Z3", "Z4", "Z5"
};

struct MOTIFTYPES
{
    MOTIFTYPES()
    {
      Mapping m;
      std::vector<AliMpPCB *> pcbs = get_allpcbs(m.ddlStore(), m.mseg());
      slatmt = get_allslatmotiftypes(pcbs);
      std::vector<const AliMpSector *> sectors = get_allsectors(m.mseg());
      sectormt = get_allsectormotiftypes(sectors);
      mt = get_allmotiftypes(pcbs, sectors);
    }

    std::vector<AliMpMotifType *> slatmt;
    std::vector<AliMpMotifType *> sectormt;
    std::vector<AliMpMotifType *> mt;
};

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, MOTIFTYPES)

BOOST_AUTO_TEST_SUITE(types_of_motif)

BOOST_AUTO_TEST_CASE(NumberOfMotifTypeForSlats)
{
  BOOST_TEST_CHECK((slatmt.size() == 104), "slatmt.size()=" << slatmt.size() << " vs expected 104");
}

BOOST_AUTO_TEST_CASE(NumberOfMotifTypeForSectors)
{
  BOOST_TEST_CHECK((sectormt.size() == 106), "sectormt.size()=" << sectormt.size() << " vs expected 106");
}

BOOST_AUTO_TEST_CASE(NumberOfMotifTypes)
{
  BOOST_TEST_CHECK((mt.size() == 210), "mt.size()=" << mt.size() << " vs expected 210");
}

BOOST_AUTO_TEST_CASE(MotifTypeNames)
{
  for (auto i = 0; i < mt.size(); ++i) {
    BOOST_TEST_CHECK((mt[i]->GetID() == motifTypeNames[i].c_str()));
  }
}

BOOST_AUTO_TEST_CASE(MotifTypeClone)
{
  std::string newname = "toto";
  AliMpMotifType *m = mt[0];
  AliMpMotifType *c = static_cast<AliMpMotifType *>(m->Clone(newname.c_str()));
  BOOST_TEST_CHECK((newname == c->GetID().Data()));
  BOOST_TEST_CHECK((m->GetID() != newname.c_str()));
}

BOOST_DATA_TEST_CASE(IxAndIyShouldStartAtZero, boost::unit_test::data::xrange(0, 210), i)
{
  MOTIFTYPES MT;
  AliMpMotifType *m = MT.mt[i];
  AliMpPad pad;
  std::unique_ptr<AliMpVPadIterator> it(m->CreateIterator());
  it->First();
  int ixmin{std::numeric_limits<int>::max()};
  int iymin{std::numeric_limits<int>::max()};
  while (!it->IsDone()) {
    pad = it->CurrentItem();
    it->Next();
    ixmin = std::min(ixmin, pad.GetIx());
    iymin = std::min(iymin, pad.GetIy());
  }
  BOOST_TEST(ixmin == 0, "ixmin=" << ixmin << " > 0 for motif " << m->GetID());
  BOOST_TEST(iymin == 0, "iymin=" << iymin << " > 0 for motif " << m->GetID());
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

