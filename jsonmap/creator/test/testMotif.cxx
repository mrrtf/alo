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

std::array<std::string, 238> motifNames{
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
  BOOST_TEST_CHECK((sectorMotif.size() == 124), "sectorMotif.size()=" << sectorMotif.size() << " vs expected 124");
}

BOOST_AUTO_TEST_CASE(NumberOfMotifs)
{
  BOOST_TEST_CHECK((motif.size() == 238), "motif.size()=" << motif.size() << " vs expected 238");
}

BOOST_AUTO_TEST_CASE(MotifNames)
{
  for (auto i = 0; i < motif.size(); ++i) {
    //std::cout << "\"" << motif[i]->GetID() << "\",";
    BOOST_TEST_CHECK((motif[i]->GetID() == motifNames[i].c_str()),
                     motif[i]->GetID().Data() << " vs expected " << motifNames[i]);
  }
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

