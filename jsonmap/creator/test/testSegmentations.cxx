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
#include "seg.h"
#include "segnumbers.h"
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "de.h"

std::array<std::string, 156> desegnames{
  "st1", "st1", "st1", "st1", "st1", "st1", "st1", "st1", "st2", "st2", "st2", "st2", "st2", "st2", "st2", "st2",
  "122000SR1", "112200SR2", "122200S", "222000N", "220000N", "220000N", "222000N", "122200S", "112200SR2", "122000SR1",
  "112200SR2", "122200S", "222000N", "220000N", "220000N", "222000N", "122200S", "112200SR2", "122000NR1", "112200NR2",
  "122200N", "222000N", "220000N", "220000N", "222000N", "122200N", "112200NR2", "122000NR1", "112200NR2", "122200N",
  "222000N", "220000N", "220000N", "222000N", "122200N", "112200NR2", "122330N", "112233NR3", "112230N", "222330N",
  "223300N", "333000N", "330000N", "330000N", "333000N", "223300N", "222330N", "112230N", "112233NR3", "122330N",
  "112233NR3", "112230N", "222330N", "223300N", "333000N", "330000N", "330000N", "333000N", "223300N", "222330N",
  "112230N", "112233NR3", "122330N", "112233NR3", "112230N", "222330N", "223300N", "333000N", "330000N", "330000N",
  "333000N", "223300N", "222330N", "112230N", "112233NR3", "122330N", "112233NR3", "112230N", "222330N", "223300N",
  "333000N", "330000N", "330000N", "333000N", "223300N", "222330N", "112230N", "112233NR3", "122330N", "112233NR3",
  "112233N", "222333N", "223330N", "333300N", "333000N", "333000N", "333300N", "223330N", "222333N", "112233N",
  "112233NR3", "122330N", "112233NR3", "112233N", "222333N", "223330N", "333300N", "333000N", "333000N", "333300N",
  "223330N", "222333N", "112233N", "112233NR3", "122330N", "112233NR3", "112233N", "222333N", "223330N", "333300N",
  "333000N", "333000N", "333300N", "223330N", "222333N", "112233N", "112233NR3", "122330N", "112233NR3", "112233N",
  "222333N", "223330N", "333300N", "333000N", "333000N", "333300N", "223330N", "222333N", "112233N", "112233NR3"
};

std::map<std::string, std::vector<int>> deperseg = {
  {"st1",       {100, 101, 102, 103, 200,  201,  202,  203}},
  {"st2",       {300, 301, 302, 303, 400,  401,  402,  403}},
  {"122000SR1", {500, 509}},
  {"112200SR2", {501, 508, 510, 517}},
  {"122200S",   {502, 507, 511, 516}},
  {"222000N",   {503, 506, 512, 515, 603,  606,  612,  615}},
  {"220000N",   {504, 505, 513, 514, 604,  605,  613,  614}},
  {"122000NR1", {600, 609}},
  {"112200NR2", {601, 608, 610, 617}},
  {"122200N",   {602, 607, 611, 616}},
  {"122330N",   {700, 713, 800, 813, 900,  913,  1000, 1013}},
  {"112233NR3", {701, 712, 714, 725, 801,  812,  814,  825, 901, 912, 914, 925, 1001, 1012, 1014, 1025}},
  {"112230N",   {702, 711, 715, 724, 802,  811,  815,  824}},
  {"222330N",   {703, 710, 716, 723, 803,  810,  816,  823}},
  {"223300N",   {704, 709, 717, 722, 804,  809,  817,  822}},
  {"333000N",   {705, 708, 718, 721, 805,  808,  818,  821, 906, 907, 919, 920, 1006, 1007, 1019, 1020}},
  {"330000N",   {706, 707, 719, 720, 806,  807,  819,  820}},
  {"112233N",   {902, 911, 915, 924, 1002, 1011, 1015, 1024}},
  {"222333N",   {903, 910, 916, 923, 1003, 1010, 1016, 1023}},
  {"223330N",   {904, 909, 917, 922, 1004, 1009, 1017, 1022}},
  {"333300N",   {905, 908, 918, 921, 1005, 1008, 1018, 1021}}};

namespace bdata = boost::unit_test::data;

BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, Mapping)

BOOST_AUTO_TEST_SUITE(segmentation)

BOOST_AUTO_TEST_CASE(segmentationOrder)
{
  for (auto i = 0; i < b_segs().size(); ++i) {
    BOOST_TEST_CHECK((get_segtype(*(b_segs()[i])) == segnames[i]));
  }
  for (auto i = 0; i < nb_segs().size(); ++i) {
    BOOST_TEST_CHECK((get_segtype(*(nb_segs()[i])) == segnames[i]));
  }
}

BOOST_AUTO_TEST_CASE(segmentationNames)
{
  std::vector<std::string> names = get_all_segmentation_names(ddlStore(), mseg());
  BOOST_TEST_CHECK(names.size() == segnames.size());
  for (auto i = 0; i < names.size(); ++i) {
    BOOST_TEST_CHECK(names[i] == segnames[i]);
  }
}

BOOST_AUTO_TEST_CASE(DetectionElementToSegmentationName)
{
  std::vector<std::string> names = get_de_seg(ddlStore(), mseg());

  BOOST_TEST_CHECK(names.size() == desegnames.size());

  for (auto i = 0; i < names.size(); ++i) {
    BOOST_TEST_CHECK(names[i] == desegnames[i]);
  }
}

BOOST_DATA_TEST_CASE(DetectionElementIdsPerSegmentationName,
                     (bdata::make(segnames)), segname)
{
  std::vector<int> idperseg = get_deids_per_segname(ddlStore(), mseg(), segname);
  BOOST_TEST(idperseg == deperseg[segname]);
}

//BOOST_DATA_TEST_CASE(segmentationMotifPositions, (bdata::make(deids)),deid) {
//std::vector<AliMpVSegmentation*> segs = get_segs(mseg(),std::vector<int>{deid},AliMp::kBendingPlane));
//}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
