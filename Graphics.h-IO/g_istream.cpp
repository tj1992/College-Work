#include "g_istream.hpp"
#include "tfstr.cpp"

void g_istream::fill_buf()
{
	char ch;
	int x = getx();
	int y = gety();
	fillsettingstype fset;
	getfillsettings(&fset);
	setfillstyle(SOLID_FILL, getbkcolor());
	
	if (!buf.size())
	{
		while (!isspace(ch = getch()) || !buf.size())
		{
			//	TODO : echo the characters
			if (ch == '\b' && buf.size())
			{
					bar(x, y, x+textwidth((char*)buf.c_str()), y+textheight("Wy"));
					buf.erase(buf.size()-1);
					outtextxy(x, y, (char*)buf.c_str());
			}
			else if (isgraph(ch))
			{
				buf.push_back(ch);
				outtextxy(x, y, (char*)buf.c_str());
				
			}
		}
	}
	moveto(0, y+textheight("Wy"));
	setfillstyle(fset.pattern, fset.color);
}

char g_istream::get()
{
	char ch = getch();
	char a[2];
	a[0] = ch;
	a[1] = '\0';
	outtext(a);
	return ch;
}

void g_istream::get(char& c)
{
	c = this->get();
}

const string g_istream:: showbuf() const
{
	return buf;
}

g_istream& g_istream::operator>>(int& i)
{
	fill_buf();
	i = from_string<int>(buf);
	return *this;
}

g_istream& g_istream::operator>>(char& i)
{
	fill_buf();
	i = from_string<char>(buf);
	return *this;
}

g_istream& g_istream::operator>>(float& i)
{
	fill_buf();
	i = from_string<float>(buf);
	return *this;
}

g_istream& g_istream::operator>>(double& i)
{
	fill_buf();
	i = from_string<double>(buf);
	return *this;
}

g_istream& g_istream::operator>>(char* i)
{
	fill_buf();
	i = from_string<char*>(buf);
	return *this;
}

g_istream& g_istream::operator>>(string& i)
{
	fill_buf();
	i = from_string<string>(buf);
	return *this;
}
