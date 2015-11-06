/*
 * Fixtures.hpp
 *
 *  Created on: Nov 6, 2015
 *      Author: ivan
 */

#ifndef FIXTURES_HPP_
#define FIXTURES_HPP_


struct LogFixture
{
	LogFixture()
	{
		Log.init(&Sink,LOG_LEVEL_VERBOSE);
	}
	~LogFixture()
	{
		Sink.ss.str(std::string());
	}

};



#endif /* FIXTURES_HPP_ */
