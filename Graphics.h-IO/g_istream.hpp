#ifndef WINBGI_ISTREAM_GUARD
#define WINBGI_ISTREAM_GUARD

#include "graphics.h"
#include <string>

using namespace std;

class g_istream
{
	string buf;
	
	void fill_buf();
	
public:
	g_istream()
	{	}
	
	void get(char&);
	char get();

	g_istream& operator>>(int&);
	g_istream& operator>>(char&);
	g_istream& operator>>(float&);
	g_istream& operator>>(double&);
	g_istream& operator>>(char*);
	g_istream& operator>>(string&);
	
	const string showbuf() const;
	
	~g_istream()
	{	}	
};
#endif
