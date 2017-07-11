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
#include <padsize.h>
#include "motif.h"
#include "seg.h"
#include "AliMpVMotif.h"

//  motifs.insert(motifs.end(),sectorMotifs.begin(),sectorMotifs.end());
//
//  out.open("motifs.list.txt");
//  for ( const auto& m: motifs ) {
//    out << m->GetID() << std::endl;
//  }
//  std::cout << motifs.size() << " different motifs in total" << std::endl;

std::array<std::string, 222> motifNames{
  "L7-13", "Z3-13", "O10-13", "Z4-13", "L8-13", "L6-13", "Z2-13", "O9-13", "Z1-13", "L5-13", "L7-16", "Z3-16", "O10-16",
  "Z4-16", "L8-16", "L6-16", "Z2-16", "O9-16", "Z1-16", "L5-16", "L19-17", "O12-17", "L20-17", "O11-17", "L10-17",
  "L9-17", "O2-4", "O1-4", "P3-5", "Q3-5", "Q4-5", "P4-5", "L4-5", "O4-5", "L3-5", "L1-5", "O3-5", "L2-5", "Q2-5",
  "P2-5", "P1-5", "Q1-5", "O7-6", "O8-6", "O6-6", "O5-6", "C6-13", "C7-13", "C8-13", "C9-13", "C10-13", "O25-13",
  "C1-4", "C2-4", "C3-4", "C4-4", "C5-4", "O23-13", "O24-13", "A8-13", "A9-13", "A10-13", "A11-13", "A12-13", "A13-13",
  "A14-13", "A15-13", "A16-13", "A17-13", "A18-13", "A20-13", "A19-13", "O21-4", "A1-4", "A7-4", "A6-4", "A5-4", "A4-4",
  "A3-4", "A2-4", "O22-4", "E7-13", "E8-13", "E9-13", "E10-13", "E11-13", "E12-13", "E13-13", "E14-13", "E15-13",
  "E16-13", "E17-13", "E18-13", "E19-13", "O27-4", "E6-4", "E5-4", "E4-4", "E3-4", "E2-4", "E1-4", "O26-4", "S0-16",
  "S1-16", "S2-16", "S3-16", "S4-16", "S5-16", "S6-16", "S7-16", "S8-16", "I1-16", "L18-16", "Z5-16", "1BD-0", "1BD-11",
  "1BD-14", "1BG*-14", "1BE-14", "1BC-0", "1BF-14", "1BB-0", "1BH-14", "1BA-0", "1BI-14", "2BB-7", "2BF-12", "2BD-15",
  "2BO-15", "2BA-7", "2BJ-15", "2BM-12", "2BU-15", "2Bd1-15", "2Be1-15", "2BV-15", "2BR-15", "2Bb1-15", "2Bi1-15",
  "2Bv4-7", "2Bg1-15", "2BY-15", "2BH-15", "2Bv2-7", "2BC-12", "2BE-12", "2Bu1-7", "2Ba1-15", "2Bt1-7", "2Bc1-15",
  "2BI-15", "2BT-15", "2BL-15", "2BP-15", "2Bh1-15", "2BQ-15", "2BW-15", "2Bv5-7", "2BS-15", "2BX-15", "2Bv3-7",
  "2Bm1-7", "2Bf1-15", "2Bv1-7", "2BK-15", "2BG-15", "2Bn1-7", "2BN-15", "2Bp1-7", "1NA-0", "1NE-1", "1NG*-2", "1NF-2",
  "1NC-0", "1NL-0", "1NI-2", "1NM-0", "1NH*-2", "1NB-0", "1NK-2", "1NN-0", "1NJ-2", "1ND-0", "2NF-8", "2NC-8", "2NE-8",
  "2NB-7", "2NI-9", "2Nt1-7", "2Nd1-9", "2Nl1-9", "2NL-9", "2NM-9", "2NA-7", "2NK-9", "2NG-9", "2NJ-9", "2Nr1-9",
  "2NH-9", "2Nv5-7", "2No1-9", "2Nw1-9", "2Nv3-7", "2Ni1-9", "2Nm1-7", "2Nv1-7", "2Nb1-9", "2NN-9", "2Ne1-9", "2Nn1-7",
  "2Nk1-9", "2Np1-7", "2Nf1-9", "2Ns1-9", "2Nq1-9", "2Nc1-9", "2Nv4-7", "2Nj1-9", "2Ng1-9", "2Nv2-7", "2Nu1-7",
  "2Na1-9"
};

struct MOTIFS
{
    MOTIFS()
    {
      Mapping m;
      std::vector<AliMpPCB*> pcbs = get_allpcbs(m.ddlStore(), m.mseg());
      std::vector<std::pair<float, float>> padsizes = get_padsizes(m.ddlStore(), m.mseg());
      slatMotif = get_allslatmotifs(pcbs, padsizes);
      std::vector<const AliMpSector*> sectors = get_allsectors(m.mseg());
      sectorMotif = get_allsectormotifs(sectors, padsizes);
      motif = get_allmotifs(pcbs, sectors, padsizes);
    }

    std::vector<AliMpVMotif*> slatMotif;
    std::vector<AliMpVMotif*> sectorMotif;
    std::vector<AliMpVMotif*> motif;
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
  BOOST_TEST_CHECK((motif.size() == 222), "motif.size()=" << motif.size() << " vs expected 222");
}

BOOST_AUTO_TEST_CASE(MotifNames)
{
  for (auto i = 0; i < motif.size(); ++i) {
    BOOST_TEST_CHECK((motif[i]->GetID() == motifNames[i].c_str()),
                     motif[i]->GetID().Data() << " vs expected " << motifNames[i]);
  }
}

BOOST_AUTO_TEST_CASE(MotifClone)
{
  std::string newname = "toto";
  AliMpVMotif* c = static_cast<AliMpVMotif*>(motif[0]->Clone(newname.c_str()));
  BOOST_TEST_CHECK(newname == c->GetID().Data());
  BOOST_TEST_CHECK(newname != motif[0]->GetID().Data());
}

BOOST_AUTO_TEST_CASE(NoSpecialMotifInSlats) {
  for (const auto& m : slatMotif) {
    BOOST_TEST_CHECK(m->GetNofPadDimensions()==1);
  }
}

BOOST_AUTO_TEST_CASE(NumberOfSpecialMotifsInSectors) {
  auto n = std::count_if(sectorMotif.begin(),sectorMotif.end(),[](AliMpVMotif* m) { return m->GetNofPadDimensions()>1; });
  BOOST_TEST_CHECK(n==3);
}

BOOST_AUTO_TEST_CASE(MaxNumberOfPadSizesInSpecialMotifs) {
  int n = 0;
  for (const auto&m : sectorMotif) {
     n = std::max(n,m->GetNofPadDimensions());
  }
  BOOST_TEST_CHECK(n==2);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

