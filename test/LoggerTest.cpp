/*
 * LoggerTest.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: ivan
 */

#include "CppLog.hpp"
#include "Fixtures.hpp"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE( LoggerTests, LogFixture )


BOOST_AUTO_TEST_CASE( logLevelErrorNoOutput )
{
	Log.init(&Sink,LOG_LEVEL_ERROR);
	Log.Info(F("String: '%s'"),"The String");
	BOOST_CHECK_EQUAL(std::string(""),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( logLevelErrorNoOutput2 )
{
	Log.init(&Sink,LOG_LEVEL_ERROR);
	Log.Verbose(F("String: '%s'"),"The String");
	BOOST_CHECK_EQUAL(std::string(""),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( logLevelErrorNoOutput3 )
{
	Log.init(&Sink,LOG_LEVEL_ERROR);
	Log.Debug(F("String: '%s'"),"The String");
	BOOST_CHECK_EQUAL(std::string(""),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( normalString )
{
	Log.Info(F("String: '%s'"),"The String");
	BOOST_CHECK_EQUAL(std::string("I: String: 'The String'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( programString )
{
	Log.Info(F("Program string: '%f'"),F("Program String"));
	BOOST_CHECK_EQUAL(std::string("I: Program string: 'Program String'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( minusInt )
{
	Log.Info(F("Int -123 '%d'"),-123);
	BOOST_CHECK_EQUAL(std::string("I: Int -123 '-123'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( unsignedInt )
{
	Log.Info(F("UnsignedInt 23 '%u'"),23);
	BOOST_CHECK_EQUAL(std::string("I: UnsignedInt 23 '23'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( unsignedIntMax )
{
	Log.Info(F("UnsignedInt 4294967295 '%lu'"),4294967295);
	BOOST_CHECK_EQUAL(std::string("I: UnsignedInt 4294967295 '4294967295'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( intMax )
{
	Log.Info(F("Int 2147483647 '%ld'"),2147483647);
	BOOST_CHECK_EQUAL(std::string("I: Int 2147483647 '2147483647'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( minusIntMax )
{
	Log.Info(F("Int -2147483648 '%ld'"),-2147483648);
	BOOST_CHECK_EQUAL(std::string("I: Int -2147483648 '-2147483648'\n"),Sink.ss.str());
}

// the following tests differ from the ARDUINO output because of the different
// type size. This i OK however, because the tests shall cover the convert routines.
BOOST_AUTO_TEST_CASE( minusIntMaxHex )
{
	Log.Info(F("Int -2147483648 '%X'"),-2147483648);
	BOOST_CHECK_EQUAL(std::string("I: Int -2147483648 '0xFFFFFFFF80000000'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( minusIntHex )
{
	Log.Info(F("Int -31 '%X'"),-31);
	BOOST_CHECK_EQUAL(std::string("I: Int -31 '0xFFFFFFFFFFFFFFE1'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( intMaxHex )
{
	Log.Info(F("Int 2147483647 '%X'"),2147483647);
	BOOST_CHECK_EQUAL(std::string("I: Int 2147483647 '0x7FFFFFFF'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( intHex )
{
	Log.Info(F("Int 31 '%X'"),31);
	BOOST_CHECK_EQUAL(std::string("I: Int 31 '0x1F'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( intMaxBin )
{
	Log.Info(F("Int 2147483647 '%b'"),2147483647);
	BOOST_CHECK_EQUAL(std::string("I: Int 2147483647 '1111111111111111111111111111111'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_CASE( intBin )
{
	Log.Info(F("Int 31 '%b'"),31);
	BOOST_CHECK_EQUAL(std::string("I: Int 31 '11111'\n"),Sink.ss.str());
}

BOOST_AUTO_TEST_SUITE_END()


