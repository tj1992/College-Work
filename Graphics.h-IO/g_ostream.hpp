#ifndef WINBGI_OSTREAM_GUARD
#define WINBGI_OSTREAM_GUARD

#include "graphics.h"
#include <string>

using namespace std;

class g_ostream
{
	int maxx;
	string format_line(string&, int, bool* = 0);
	void print_line(string& str);
	
public:
	g_ostream() : maxx(0)
	{	}
	
	void put(char c);
	
	g_ostream& operator<<(int);
	g_ostream& operator<<(char);
	g_ostream& operator<<(float);
	g_ostream& operator<<(double);
	g_ostream& operator<<(string);
	g_ostream& operator<<(const char*);
	
	//	TODO : output flags
	//g_ostream operator<<(const g_oflags&);
	
	~g_ostream()
	{	}
};

class g_oflags
{
	
public:
	g_oflags()
	{	}
};
#endif
