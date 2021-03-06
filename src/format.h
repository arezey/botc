/*
	Copyright 2012-2014 Santeri Piippo
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BOTC_FORMAT_H
#define BOTC_FORMAT_H

#include "string.h"
#include "list.h"

class FormatArgument
{
public:
	FormatArgument (const String& a) : m_text (a) {}
	FormatArgument (char a) : m_text (a) {}
	FormatArgument (int a) : m_text (String::fromNumber (a)) {}
	FormatArgument (long a) : m_text (String::fromNumber (a)) {}
	FormatArgument (size_t a) : m_text (String::fromNumber ((long) a)) {}
	FormatArgument (const char* a) : m_text (a) {}

	FormatArgument (void* a)
	{
		m_text.sprintf ("%p", a);
	}

	FormatArgument (const void* a)
	{
		m_text.sprintf ("%p", a);
	}

	FormatArgument (std::nullptr_t) :
		m_text (FormatArgument ((void*) 0).text()) {}

	template<class T> FormatArgument (const List<T>& list)
	{
		if (list.isEmpty())
		{
			m_text = "{}";
			return;
		}

		m_text = "{ ";

		for (const T& a : list)
		{
			if (&a != &list[0])
				m_text += ", ";

			m_text += FormatArgument (a).text();
		}

		m_text += " }";
	}

	inline const String& text() const
	{
		return m_text;
	}

private:
	String m_text;
};

#ifndef IN_IDE_PARSER
# ifdef DEBUG
#  define devf(...) PrintTo (stderr, __VA_ARGS__)
#  define dvalof( A ) PrintTo (stderr, "value of '%1' = %2\n", #A, A)
# else
#  define devf(...)
#  define dvalof( A )
# endif // DEBUG
#else
// print something in debug builds
void devf (void, ...);

// print the value of @a
void dvalof (void a);
#endif // IN_IDE_PARSER


//
// Formats the given string with the given args.
//
String formatArgs (const String& fmtstr, const std::vector<String>& args);

//
// Expands the given arguments into a vector of strings.
//
template<typename T, typename... RestTypes>
void expandFormatArguments (std::vector<String>& data, const T& arg, const RestTypes& ... rest)
{
	data.push_back (FormatArgument (arg).text());
	expandFormatArguments (data, rest...);
}

static void expandFormatArguments (std::vector<String>& data) __attribute__ ( (unused));
static void expandFormatArguments (std::vector<String>& data)
{
	(void) data;
}

//
// Formats the given formatter string and args and returns the string.
// This is essentially a modernized sprintf.
//
// Args in the format string take the form %n where n is a digit. The argument
// will be expanded to the nth argument passed. This is essentially Qt's
// QString::arg() syntax. Note: %0 is invalid.
//
// Arguments can be passed a modifier which takes the form of a character
// just before the digit. Currently supported modifiers are s, d and x.
//
// - s: The argument will expand into "s" if it would've not expanded into "1"
//      otherwise. If it would have expanded into "1" it will expand into an
//      empty string.
//
// - d: The argument expands into the numeric form of the first character of
//      its previous expansion. Use this to get numeric forms of @c char
//      arguments.
//
// - x: The numeric argument will be represented in hexadecimal notation. This
//      will work if the argument is a string representing a number. If the
//      argument did not expand into a number in the first place, 0 is used
//      and 0x0 is printed.
//
template<typename... argtypes>
String format (const String& fmtstr, const argtypes&... raw_args)
{
	std::vector<String> args;
	expandFormatArguments (args, raw_args...);
	ASSERT_EQ (args.size(), sizeof... (raw_args))
	return formatArgs (fmtstr, args);
}

//
// This is an overload of format() where no arguments are supplied.
// It returns the formatter string as-is.
//
static String format (const String& fmtstr) __attribute__ ( (unused));
static String format (const String& fmtstr)
{
	return fmtstr;
}

//
// Processes the given formatter string using format() and prints it to the
// specified file pointer.
//
template<typename... argtypes>
void printTo (FILE* fp, const String& fmtstr, const argtypes&... args)
{
	fprintf (fp, "%s", format (fmtstr, args...).c_str());
}

//
// Processes the given formatter string using format() and appends it to the
// specified file by name.
//
template<typename... argtypes>
void printTo (const String& filename, const String& fmtstr, const argtypes&... args)
{
	FILE* fp = fopen (filename, "a");

	if (fp != null)
	{
		fprintf (fp, "%s", format (fmtstr, args...).c_str());
		fflush (fp);
		fclose (fp);
	}
}

//
// Processes the given formatter string using format() and prints the result to
// stdout.
//
template<typename... argtypes>
void print (const String& fmtstr, const argtypes&... args)
{
	printTo (stdout, fmtstr, args...);
}

//
// Throws an std::runtime_error with the processed formatted string. The program
// execution terminates after a call to this function as the exception is first
// caught in main() which prints the error to stderr and then exits.
//
template<typename... argtypes>
void error (const char* fmtstr, const argtypes&... args)
{
	error (format (String (fmtstr), args...));
}

//
// An overload of error() with no string formatting in between.
//
void error (const String& msg);

#endif // BOTC_FORMAT_H
