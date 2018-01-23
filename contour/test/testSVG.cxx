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
#include <sstream>
#include <svgContour.h>

using namespace o2::mch::svg;

BOOST_AUTO_TEST_SUITE(o2_mch_contour)

BOOST_AUTO_TEST_SUITE(svg)

BOOST_AUTO_TEST_CASE(WriterInteger)
{
  std::stringstream out;
  Writer w(out, 10, 20);
  w << 2;
  BOOST_CHECK(out.str() == "2");
}

BOOST_AUTO_TEST_CASE(WriterHTML)
{
  std::stringstream out;
  Writer w(out, 10, 20);
  w.htmlStart();
  w << "\n";
  w.htmlEnd();
  w << "toto";
  BOOST_CHECK(out.str()=="<html><body>\n</html></body>toto");
}

BOOST_AUTO_TEST_CASE(WriterSVG)
{
  std::stringstream out;
  Writer w(out, 10, 20);
  w.svgStart();
  w.svgEnd();
  BOOST_CHECK(out.str()==R"(<svg width="10" height="20"></svg>)");
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

