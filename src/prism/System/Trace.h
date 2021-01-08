#pragma once
#include <iostream>
#include <assert.h>

#ifndef ASSERT
#define ASSERT assert
#endif

struct Trace
{
	char const* _filename;
	unsigned _line;

	Trace(char const* filename, unsigned const line) :
		_filename{ filename },
		_line{ line }
	{}

	void operator()(const char* msg) const
	{
		int a;
		std::cerr << _filename << ": " << _line << "  " << msg;
		std::cin >> a;
		std::exit(-1);
	}
};

#ifndef NDEBUG
#define TRACE Trace(__FILE__, __LINE__)
#else
#define TRACE __noop
#endif