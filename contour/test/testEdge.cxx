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
#include "edge.h"

using namespace o2::mch::geometry;

struct EDGES
{
    std::vector<VerticalEdge> testVerticals{{0, 7, 1},
                                            {1, 1, 0},
                                            {3, 0, 1},
                                            {5, 1, 0},
                                            {6, 0, 7},
                                            {2, 5, 3},
                                            {4, 3, 5}};

};

BOOST_AUTO_TEST_SUITE(o2_mch_geometry)

BOOST_FIXTURE_TEST_SUITE(edge, EDGES)

BOOST_AUTO_TEST_CASE(AVerticalLeftEdgeIsTopToBottom)
{
  int dummy{0};
  VerticalEdge edge{dummy, 12, 1};
  BOOST_CHECK(isLeftEdge(edge));
  BOOST_CHECK(isTopToBottom(edge));
}

BOOST_AUTO_TEST_CASE(AVerticalRightEdgeIsBottomToTop)
{
  int dummy{0};
  VerticalEdge edge{dummy, 1, 12};
  BOOST_CHECK(isRightEdge(edge));
  BOOST_CHECK(isBottomToTop(edge));
}

BOOST_AUTO_TEST_CASE(ALeftToRightHorizontalEdgeHasEndPointGreaterThanStartPoint)
{
  int dummy{0};
  HorizontalEdge edge{dummy, 1, 12};
  BOOST_CHECK(isLeftToRight(edge));
}

BOOST_AUTO_TEST_CASE(ARightToLeftHorizontalEdgeHasEndPointSmallerThanStartPoint)
{
  int dummy{0};
  HorizontalEdge edge{dummy, 12, 1};
  BOOST_CHECK(isRightToLeft(edge));
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeWithBeginAboveEndIsALefty)
{
  VerticalEdge vi{0, 12, 10};
  BOOST_CHECK_EQUAL(isLeftEdge(vi), true);
  BOOST_CHECK_EQUAL(isRightEdge(vi), false);
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeWithBeginAboveEndIsARighty)
{
  VerticalEdge vi{0, 10, 12};
  BOOST_CHECK_EQUAL(isRightEdge(vi), true);
  BOOST_CHECK_EQUAL(isLeftEdge(vi), false);
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeIntervalIsAnIntervalOfPositiveIndices)
{
  BOOST_CHECK_THROW(VerticalEdge(0, -1, 0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(AVerticalEdgeHasATopAndBottom)
{
  VerticalEdge edge{2, 10, 12};
  BOOST_CHECK_EQUAL(bottom(edge), 10);
  BOOST_CHECK_EQUAL(top(edge), 12);
}

BOOST_AUTO_TEST_CASE(BeginAndEndForALeftEdgeVertical)
{
  VerticalEdge e{0, 7, 1};

  BOOST_CHECK_EQUAL(begin(e), 7);
  BOOST_CHECK_EQUAL(end(e), 1);
}

BOOST_AUTO_TEST_CASE(BeginAndEndForARightEdgeVertical)
{
  VerticalEdge e{0, 1, 7};

  BOOST_CHECK_EQUAL(begin(e), 1);
  BOOST_CHECK_EQUAL(end(e), 7);
}

BOOST_AUTO_TEST_CASE(BeginAndEndForALeftToRightHorizontal)
{
  HorizontalEdge e{0, 1, 7};
  BOOST_CHECK_EQUAL(begin(e), 1);
  BOOST_CHECK_EQUAL(end(e), 7);
}

BOOST_AUTO_TEST_CASE(BeginAndEndForARightToLeftHorizontal)
{
  HorizontalEdge e{0, 7, 1};
  BOOST_CHECK_EQUAL(begin(e), 7);
  BOOST_CHECK_EQUAL(end(e), 1);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

