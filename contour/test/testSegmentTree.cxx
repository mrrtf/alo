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
#include "segmentTree.h"

using namespace o2::mch::contour;

struct YPOS
{

    YPOS()
    {

      Node* left = new Node{Interval{0, 4}};
      Node* right = new Node{Interval{4, 8}};

      testNode.setLeft(left).setRight(right);

      left->cardinality(dummyCardinality);
      right->cardinality(dummyCardinality);

    }

    std::vector<double> ypos{0, 1, 2, 3, 4, 5, 6, 7, 8};
    Node node{Interval{0, 8}};
    Node testNode{Interval{0, 8}};
    int dummyCardinality{3};

};

BOOST_AUTO_TEST_SUITE(o2_mch_geometry)

BOOST_FIXTURE_TEST_SUITE(segmenttree, YPOS)

BOOST_AUTO_TEST_CASE(NeedAtLeastTwoValuesToBuildASegmentTree)
{
  BOOST_CHECK_THROW(createSegmentTree({1}), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(NodeInsertAndDelete)
{
  std::unique_ptr<Node> t{createSegmentTree(ypos)};

  t->insertInterval(Interval{1, 5});
  t->insertInterval(Interval{5, 8});
  t->deleteInterval(Interval{6, 7});

  std::ostringstream os;

  os << '\n' << (*t);

  std::string expectedOutput =
    R"(
[0,8] potent
     [0,4] potent
           [0,2] potent
                 [0,1]
                 [1,2] C=1
           [2,4] C=1
                 [2,3]
                 [3,4]
     [4,8] potent
           [4,6] C=1
                 [4,5]
                 [5,6]
           [6,8] potent
                 [6,7]
                 [7,8] C=1
)";

  BOOST_CHECK_EQUAL(os.str(), expectedOutput);
}

BOOST_AUTO_TEST_CASE(JustCreatedNodeIsNotPotent)
{
  BOOST_CHECK_EQUAL(node.isPotent(), false);
}

BOOST_AUTO_TEST_CASE(JustCreatedNodeHasCardinalityEqualsZero)
{
  BOOST_CHECK_EQUAL(node.cardinality(), 0);
}

BOOST_AUTO_TEST_CASE(PromoteNode)
{
  testNode.promote();

  BOOST_CHECK_EQUAL(testNode.cardinality(), 1);
  BOOST_CHECK_EQUAL(testNode.left()->cardinality(), dummyCardinality - 1);
  BOOST_CHECK_EQUAL(testNode.right()->cardinality(), dummyCardinality - 1);
}

BOOST_AUTO_TEST_CASE(DemoteNode)
{
  testNode.promote();
  testNode.demote();
  BOOST_CHECK_EQUAL(testNode.cardinality(), 0);
  BOOST_CHECK_EQUAL(testNode.left()->cardinality(), dummyCardinality);
  BOOST_CHECK_EQUAL(testNode.right()->cardinality(), dummyCardinality);
  BOOST_CHECK_EQUAL(testNode.isPotent(), true);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


