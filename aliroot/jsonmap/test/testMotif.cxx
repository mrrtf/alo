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
#include "AliMpVMotif.h"
#include "mapping.h"
#include "motif.h"
#include "seg.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <padsize.h>
#include <pcb.h>
#include <string>

std::array<std::string, 222> motifNames{
  "L7-9", "Z3-9", "O10-9", "Z4-9", "L8-9", "L6-9", "Z2-9", "O9-9", "Z1-9", "L5-9", "L7-14", "Z3-14", "O10-14", "Z4-14",
  "L8-14", "L6-14", "Z2-14", "O9-14", "Z1-14", "L5-14", "L19-16", "O12-16", "L20-16", "O11-16", "L10-16", "L9-16",
  "O2-12", "O1-12", "P3-15", "Q3-15", "Q4-15", "P4-15", "L4-15", "O4-15", "L3-15", "L1-15", "O3-15", "L2-15", "Q2-15",
  "P2-15", "P1-15", "Q1-15", "O7-17", "O8-17", "O6-17", "O5-17", "C6-9", "C7-9", "C8-9", "C9-9", "C10-9", "O25-9",
  "C1-12", "C2-12", "C3-12", "C4-12", "C5-12", "O23-9", "O24-9", "A8-9", "A9-9", "A10-9", "A11-9", "A12-9", "A13-9",
  "A14-9", "A15-9", "A16-9", "A17-9", "A18-9", "A20-9", "A19-9", "O21-12", "A1-12", "A7-12", "A6-12", "A5-12", "A4-12",
  "A3-12", "A2-12", "O22-12", "E7-9", "E8-9", "E9-9", "E10-9", "E11-9", "E12-9", "E13-9", "E14-9", "E15-9", "E16-9",
  "E17-9", "E18-9", "E19-9", "O27-12", "E6-12", "E5-12", "E4-12", "E3-12", "E2-12", "E1-12", "O26-12", "S0-14", "S1-14",
  "S2-14", "S3-14", "S4-14", "S5-14", "S6-14", "S7-14", "S8-14", "I1-14", "L18-14", "Z5-14", "1BD-0", "1BD-3", "1BD-7",
  "1BG-7", "1BE-7", "1BC-0", "1BF-7", "1BB-0", "1BH-7", "1BA-0", "1BI-7", "2BB-2", "2BF-5", "2BD-10", "2BO-10", "2BA-2",
  "2BJ-10", "2BM-5", "2BU-10", "2Bd1-10", "2Be1-10", "2BV-10", "2BR-10", "2Bb1-10", "2Bi1-10", "2Bv4-2", "2Bg1-10",
  "2BY-10", "2BH-10", "2Bv2-2", "2BC-5", "2BE-5", "2Bu1-2", "2Ba1-10", "2Bt1-2", "2Bc1-10", "2BI-10", "2BT-10",
  "2BL-10", "2BP-10", "2Bh1-10", "2BQ-10", "2BW-10", "2Bv5-2", "2BS-10", "2BX-10", "2Bv3-2", "2Bm1-2", "2Bf1-10",
  "2Bv1-2", "2BK-10", "2BG-10", "2Bn1-2", "2BN-10", "2Bp1-2", "1NA-0", "1NE-4", "1NG-8", "1NF-8", "1NC-0", "1NL-0",
  "1NI-8", "1NM-0", "1NH-8", "1NB-0", "1NK-8", "1NN-0", "1NJ-8", "1ND-0", "2NF-6", "2NC-6", "2NE-6", "2NB-2", "2NI-11",
  "2Nt1-2", "2Nd1-11", "2Nl1-11", "2NL-11", "2NM-11", "2NA-2", "2NK-11", "2NG-11", "2NJ-11", "2Nr1-11", "2NH-11",
  "2Nv5-2", "2No1-11", "2Nw1-11", "2Nv3-2", "2Ni1-11", "2Nm1-2", "2Nv1-2", "2Nb1-11", "2NN-11", "2Ne1-11", "2Nn1-2",
  "2Nk1-11", "2Np1-2", "2Nf1-11", "2Ns1-11", "2Nq1-11", "2Nc1-11", "2Nv4-2", "2Nj1-11", "2Ng1-11", "2Nv2-2", "2Nu1-2",
  "2Na1-11"
};

struct MOTIFS
{
    MOTIFS()
    {
      Mapping m;
      std::vector<AliMpPCB *> pcbs = get_allpcbs(m.ddlStore(), m.mseg());
      auto padsizes = get_padsizes(m.ddlStore(), m.mseg());
      slatMotif = get_allslatmotifs(pcbs, padsizes);
      std::vector<const AliMpSector *> sectors = get_allsectors(m.mseg());
      sectorMotif = get_allsectormotifs(sectors, padsizes);
      motif = get_allmotifs(pcbs, sectors, padsizes);
    }

    std::vector<AliMpVMotif *> slatMotif;
    std::vector<AliMpVMotif *> sectorMotif;
    std::vector<AliMpVMotif *> motif;
};

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, MOTIFS)

BOOST_AUTO_TEST_SUITE(motifs)

BOOST_AUTO_TEST_CASE(NumberOfMotifForSlats)
{
  BOOST_TEST_CHECK((slatMotif.size() == 114), "slatMotif.size()=" << slatMotif.size() << " vs expected 114");
}

BOOST_AUTO_TEST_CASE(NumberOfMotifForSectors)
{
  BOOST_TEST_CHECK((sectorMotif.size() == 108), "sectorMotif.size()=" << sectorMotif.size() << " vs expected 108");
}

BOOST_AUTO_TEST_CASE(NumberOfMotifs)
{
  BOOST_TEST_CHECK((motif.size() == motifNames.size()), "motif.size()=" << motif.size() << " vs expected 222");
}

BOOST_AUTO_TEST_CASE(MotifNames)
{
  std::vector<std::string> test;
  for (auto i = 0; i < motif.size(); ++i) {
    test.push_back(motif[i]->GetID().Data());
  }
  BOOST_TEST(std::is_permutation(test.begin(), test.end(), motifNames.begin()));
}

BOOST_AUTO_TEST_CASE(MotifClone)
{
  std::string newname = "toto";
  AliMpVMotif *c = static_cast<AliMpVMotif *>(motif[0]->Clone(newname.c_str()));
  BOOST_TEST_CHECK(newname == c->GetID().Data());
  BOOST_TEST_CHECK(newname != motif[0]->GetID().Data());
}

BOOST_AUTO_TEST_CASE(NoSpecialMotifInSlats)
{
  for (const auto &m : slatMotif) {
    BOOST_TEST_CHECK(m->GetNofPadDimensions() == 1);
  }
}

BOOST_AUTO_TEST_CASE(NumberOfSpecialMotifsInSectors)
{
  auto n = std::count_if(sectorMotif.begin(), sectorMotif.end(),
                         [](AliMpVMotif *m) { return m->GetNofPadDimensions() > 1; });
  BOOST_TEST_CHECK(n == 3);
}

BOOST_AUTO_TEST_CASE(MaxNumberOfPadSizesInSpecialMotifs)
{
  int n = 0;
  for (const auto &m : sectorMotif) {
    n = std::max(n, m->GetNofPadDimensions());
  }
  BOOST_TEST_CHECK(n == 2);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

