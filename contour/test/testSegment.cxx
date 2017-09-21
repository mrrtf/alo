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
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "segment.h"

using namespace o2::mch::contour;

BOOST_AUTO_TEST_SUITE(o2_mch_geometry)

BOOST_AUTO_TEST_SUITE(segment)

BOOST_AUTO_TEST_CASE(ConstructSegmentFromVerticalEdge)
{
  VerticalEdge edge{ 0, 1, 2 };

  Segment s(edge);

  BOOST_CHECK(s.begin()==Vertex<int>(0,1));
  BOOST_CHECK(s.end()==Vertex<int>(0,2));
}

BOOST_AUTO_TEST_CASE(ConstructSegmentFromHorizontalEdge)
{
  HorizontalEdge edge{ 0, 1, 2 };

  Segment s(edge);

  BOOST_CHECK(s.begin()==Vertex<int>(1,0));
  BOOST_CHECK(s.end()==Vertex<int>(2,0));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


