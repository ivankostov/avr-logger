# avr-logger
Logging library for AVR microcontrollers

#include "SerialLogger.h"
``` cpp
void setup()
{
  Serial.begin(9600);
  Log.init(&Serial, LOG_LEVEL_VERBOSE);
}

void loop()
{
  Log.Info(F("Hello there"));
  Log.Verbose(F("I can do this as well '%F'"),F("Big nasty Progmem string"));
  Log.Debug(F("I can print HEX '%X'"),123);
  Log.Debug(F("Or binary '%b'"),123);
  delay(5000);
}
```
# Introduction

Debugging a microcontroller can be time consuming and usually requires special hardware. That is why most of the developers prefer to use
the good old "printf" debugging. However most of the AVR microcontrollers have limited SRAM and storing debug strings is a memory waste.
Even worse, can cause the software to stop working. Overflow of the SRAM is difficult to find. The program has a strange behaviour.

In order to solve the problem one may want to store the debug string in the flash memory directly. However accessing the flash memory is not
that easy and all functions in the logging facility shall be able to access flash memory.

The AVR Logger provides solution to those problems. It:

- supports printing format strings from flash (program) memory 
- supports different sinks. The sink needs only to implement `void write( unsigned char u );`
- comes with a set of mocks for compiling on PC ( see /test for some examples )

# Printing

The AVR Logger support a semantic similar to the <i>printf()</i>.

- %s - print a normal string
- %F or %f - print flash ( progmem ) string
- %d or %i - print signed int
- %u - print unsigned int.
- %l - long modificator. Can be used as %%ld, %%li, %%lu. 
- %X or %x - print hex int
- %b - print binary representation of an integer.
- %c - prints single character

The AVR Logger supports the following log levels. Setting the Logger to a specific level forces it to ignore messages below that level.
Code is optimized away and takes no resources. For "Release" version of the software consider LOG_LEVEL_ERROR or LOG_LEVEL_INFO.

- LOG_LEVEL_NOOUTPUT - no output at all this is the "highest" level
- LOG_LEVEL_ERROR - only error messages are printed.
- LOG_LEVEL_INFO - error and info messages are printed.
- LOG_LEVEL_DEBUG - error, info, and debug messages are printed.
- LOG_LEVEL_VERBOSE - all messages are printed

# Log Sinks

Storing the printed logs from a microcontroller can be tricky. In contrast to PCs microcontrollers have a limited memory.
Storing megabytes of logs is not an option. Different boards solve the problem in a different way. 
If you have and SD card in your board you can store them. 
If you have a network connection ( WiFi, Ethernet, ... ) you can stream the logs to a PC and store them there.
Or you can rely on the good old seral console and log the messages there. Implementing different sinks of the AVR Logger is very easy.

All you need to do is to implement a class with a write function

	``` cpp
	struct CppLogSink
	{
		void write(unsigned char u)
		{
			ss << u;
		}
		std::stringstream ss;
	};
	typedef Logging<CppLogSink> CppLog;

	extern CppLogSink Sink;
	extern CppLog Log;
	```
 
And then in your code use it like this

	``` cpp
	CppLogSink Sink;
	CppLog Log;

	void setup()
	{
		// init the sink if needed - open file, stream, etc.
		Log.init(&Sink,LOG_LEVEL_VERBOSE);
	}

	void loop()
	{
		Log.Info(F("Hello '%F'"),F("Ivan"));
	}
	```
# Testing

Testing the code on the microcontroller directly can be difficult and time consuming. That is why it is important to have 
a small hardware abstraction layer that allows you to run your code on a PC as well. Abstracting a logger is not an option.
That is one more reason to support different log sinks in the AVR Logger.

The AVR Logger comes with a ready to use CPP implementation of a CppSink and the corresponding mocks. 
Because of the flexible design, you can use the Logger with your favorite mock library if you want to. ( For example Turtle Mock )
For an example please have a look in the \a test folder of the project.

# Installation

Download the [ZIP file](https://github.com/ivankostov/avr-logger/archive/master.zip "ZIP file") and import it from your Arduino IDE. ( Sketch -> Include Library -> Add .ZIP Library ). Clean it up if you want to or have a look in the example. Or if you are using Eclipse or some other IDE, include SerialLogger.h and Logger.h in your project. 
