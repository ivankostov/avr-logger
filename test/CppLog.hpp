/*
Copyright (c) 2015 Ivan Kostov <ikostov@gmail.com>

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:



The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef CPPLOGSINK_HPP_
#define CPPLOGSINK_HPP_

// include this file before the Logger.h
#include "LoggerMocks.hpp"
#include "Logger.h"

#include <sstream>

struct CppLogSink
{
	void write(unsigned char u)
	{
		ss << u;
	}
	std::stringstream ss;
};


typedef Logging<CppLogSink> CppLog;

// this is a bad C++ design,
// but we want to have the same structure as in an Arduino project
extern CppLogSink Sink;
extern CppLog Log;

#endif /* CPPLOGSINK_HPP_ */
