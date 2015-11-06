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

#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

// change this to be more generic
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#include "string.h"
#include <avr/pgmspace.h>
#endif

// add other AVRs here

// you are on your own if you are using a different target - Windows, Linux, etc.
// include the concrete header before this one. See LoggerMocks.hpp in the examples.

enum LogLevel
{
	LOG_LEVEL_NOOUTPUT,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_VERBOSE
};

enum Base
{
	NONE = 0,
	BINARY = 2,
	DECIMAL = 10,
	HEXADECIMAL = 16
};



template<class LogSinkT>
class Logging
{
public:
	/*!
	 * default Constructor
	 */
	Logging() :
			level_(LOG_LEVEL_NOOUTPUT)
	{
	}

	void init(LogSinkT* sink, LogLevel level = LOG_LEVEL_INFO)
	{
		logSink_ = sink;
		level_ = level;
	}
	/**
	 * @param format format string
	 * @param ... variables
	 */
	void Error(const __FlashStringHelper * format, ...)
	{
		if (LOG_LEVEL_ERROR <= level_)
		{
			print(F("*** ERROR: "), 0);
			va_list args;
			va_start(args, format);
			print(format, args);
			print(F("\n"), 0);
		}
	}

	/**
	 * @param format format string
	 * @param ... variables
	 */
	void Info(const __FlashStringHelper * format, ...)
	{
		if (LOG_LEVEL_INFO <= level_)
		{
			print(F("I: "), 0);
			va_list args;
			va_start(args, format);
			print(format, args);
			print(F("\n"), 0);
		}
	}

	/**
	 * @param format format string
	 * @param ... variables
	 */
	void Debug(const __FlashStringHelper * msg, ...)
	{
		if (LOG_LEVEL_DEBUG <= level_)
		{
			print(F("DBG: "), 0);
			va_list args;
			va_start(args, msg);
			print(msg, args);
			print(F("\n"), 0);
		}
	}
	/**
	 * @param format format string
	 * @param ... variables
	 */
	void Verbose(const __FlashStringHelper * format, ...)
	{
		if (LOG_LEVEL_VERBOSE <= level_)
		{
			print(F("VER: "), 0);
			va_list args;
			va_start(args, format);
			print(format, args);
			print(F("\n"), 0);
		}
	}

private:
	LogSinkT* logSink_;

	int level_;
	long baud_;

	void print(const __FlashStringHelper * msg, va_list args)
	{
		PGM_P p = reinterpret_cast<PGM_P>(msg);
		while (42)
		{
			unsigned char c = pgm_read_byte(p++);
			if (c == 0)
			{
				break;
			}

			if (c == '%')
			{
				unsigned char next = pgm_read_byte(p++);
				if (next == '\0')
				{
					break;
				}
				if (next == '%')
				{
					logSink_->write(next);
					continue;
				}
				if (next == 's')
				{
					register char *s = (char *)va_arg( args, int );
					printString(s);
					continue;
				}
				if (next == 'F' || next == 'f')
				{
					register const __FlashStringHelper *s
					= (const __FlashStringHelper *)va_arg( args, int );
					printProgramString(s);
					continue;
				}
				if (next == 'd' || next == 'i')
				{
					printSignedNumber((long)va_arg( args, int ),DECIMAL);
					continue;
				}
				if (next == 'u')
				{
					printUnsignedNumber(
							(unsigned long)va_arg( args, unsigned int ),DECIMAL
					);
					continue;
				}
				if (next == 'l')
				{
					unsigned char nextnext = pgm_read_byte(p++);
					if (nextnext == '\0')
					{
						break;
					}
					if (nextnext == 'd' || nextnext == 'i')
					{
						printSignedNumber((long)va_arg( args, long ),DECIMAL);
						continue;
					}
					if (nextnext == 'u')
					{
						printUnsignedNumber(
								(unsigned long)va_arg( args, unsigned long ),
								DECIMAL
						);
						continue;
					}

					printUnsignedNumber(
							(unsigned long)va_arg( args, unsigned int ),DECIMAL
					);
					continue;
				}
				if (next == 'x')
				{
					printUnsignedNumber((unsigned long)va_arg( args, int ),HEXADECIMAL);
					continue;
				}
				if (next == 'X')
				{
					printString("0x");
					printUnsignedNumber((unsigned long)va_arg( args, int ),HEXADECIMAL);
					continue;
				}
				if (next == 'b')
				{
					printUnsignedNumber(va_arg( args, int ),BINARY);
					continue;
				}
				if (next == 'c')
				{
					logSink_->write((const uint8_t)va_arg( args, int ));
					continue;
				}
			}
			logSink_->write((const uint8_t)c);
		}
	}
	void printString(const char* s)
	{
		if (s == NULL) return;
		size_t size = strlen(s);
		while (size--)
		{
			logSink_->write((const uint8_t)(*s++));
		}
	}
	void printProgramString(const __FlashStringHelper *s)
	{
		PGM_P p = reinterpret_cast<PGM_P>(s);
		while (42)
		{
			unsigned char c = pgm_read_byte(p++);
			if (c == 0) break;
			logSink_->write((const uint8_t)c);
		}
	}
	void printSignedNumber(long num, Base base)
	{
		if (base == DECIMAL)
		{
			if (num < 0)
			{
				printString("-");
				num = -num;
				printUnsignedNumber(num, DECIMAL);
			}
			else
			{
				printUnsignedNumber(num, DECIMAL);
			}
		}
		else
		{
			printUnsignedNumber(num, base);
		}
	}

	void printUnsignedNumber(unsigned long num, Base base = DECIMAL)
	{
		char buf[8 * sizeof(long) + 1];
		char *str = &buf[sizeof(buf) - 1];
		*str = '\0';
		do
		{
			unsigned long m = num;
			num /= base;
			char c = m - base * num;
			*--str = c < 10 ? c + '0' : c + 'A' - 10;
		}
		while(num);
		printString(str);
	}
};

#endif // LOGGER_H
