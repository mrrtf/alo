#define BOOST_TEST_MODULE mch mapping test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "mapping.h"


BOOST_FIXTURE_TEST_SUITE(mch_aliroot_mapping, Mapping)

BOOST_AUTO_TEST_CASE(loadMapping)
{
  BOOST_TEST_REQUIRE(ddlStore());
  BOOST_TEST_REQUIRE(mseg());
  BOOST_TEST_REQUIRE(deids().size() == 156);
}


BOOST_AUTO_TEST_SUITE_END()
