#define BOOST_TEST_MODULE mylib_test

#include <boost/test/included/unit_test.hpp>
#include "my_lib.h"

#include "singleton.h"

BOOST_AUTO_TEST_SUITE( test_suite_project )

//initialize some variables or declare some functions


BOOST_AUTO_TEST_CASE(test_returning_boostversion)
{
    bool testversion = print_boost_version();
    BOOST_REQUIRE(testversion);	
}


BOOST_AUTO_TEST_CASE(testVector)
{
    linalg::aliases::float3  testVector = print_linalg_vector();
    BOOST_CHECK(1 == testVector[0]);
    BOOST_TEST(1 == testVector[0]);
    BOOST_CHECK_EQUAL( 2, testVector[1] );	
}

//actually bad example, rather an integration test since it depends on the presence of the data source (singleton bad idea if there's a hard dependency)
BOOST_AUTO_TEST_CASE(test_computingtotalpopulation)
{
    SingletonRecordFinder rf;
    vector<string> names{"Seoul", "Mexico City"};
    int tp = rf.total_population(names);
    BOOST_CHECK_EQUAL(17500000+17400000,tp);	
}

//this example uses some CI style
BOOST_AUTO_TEST_CASE(test_dependenttotalpopulation)
{
    DummyDatabase db;
    ConfigurableRecordFinder rf{db};
    BOOST_CHECK_EQUAL(4,rf.total_population(vector<string>{"alpha","gamma"}));
}


BOOST_AUTO_TEST_SUITE_END()
