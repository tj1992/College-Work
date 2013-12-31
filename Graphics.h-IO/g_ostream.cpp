#include "g_ostream.hpp"
#include "tfstr.cpp"
#include <sstream>

string g_ostream::format_line(string& s, int len, bool* term1)
{
	int sp, l;
	
	l = 0;		//	current line character count
	sp = -1;	//	no space
	for (int i = 0; i < s.length(); ++i)
	{
		if (l == len)
		{
			if (sp >= 0)	//	space?
			{
				s[sp] = '\n';
				l = i-sp;
				sp = -1;	//	reset to no space
			}
			else
			{
				s.insert(s.begin()+i-1, '\n');	//	insert at i'th-position; s[i] gets shifted right
				l = 1;	//	s[i]
			}
			if (term1 != 0 && *term1)
				break;
		}
		else
			++l;
		if (isspace(s[i]))
		 	if (s[i] != '\n')
		 		sp = i;		//	space at sp=i'th postion
		 	else
		 	{
		 		l = 0;
		 		sp = -1;
		 	}
	}
	return s;
}

/*	sub_string : str
	Extracts the starting '\n' terminated part of 'str' ('\n' inclusive) and returns it.
	'nlflag' is for checking whether a '\n' is present in the string or not.
	(If no '\n' is found then returns 'str' itself)
	Assigns 'str' to the remaining part of itself.
*/
string sub_string(string& str, bool* nlflag = 0)
{
	string retstr, newstr;
	int i = 0;

	while (i < str.size())
	{
		retstr += str[i];
		if (str[i++] == '\n')
		{
			if (nlflag != 0)
				*nlflag = true;
			break;
		}
	}
	
	while (i < str.size())
		newstr += str[i++];
	str = newstr;		//	makes 'str' equal to remaining part
	return retstr;
}

void g_ostream::print_line(string& str)
{
	bool nlflag = false;
	int xchars = getx()/textwidth("a");
	if (!maxx)
		maxx = getmaxx();	//	can't place it in constructor as graphics system may not have initialised at that time.
	int nchars = maxx/textwidth("a");
	
	if (xchars > 0)
	{
		bool term1 = true;		//	terminate 'format_line' after one formatting.
		string substr = sub_string(str);		//	First substring
		format_line(substr, nchars-xchars, &term1);	//	Format to the space left (only one)
		string ss = sub_string(substr, &nlflag);	//	Get the final formatted substring.
		
		ostringstream os(ss);
		outstream(os);
		
		str = substr+str;
		
		if (nlflag)
			moveto(0, gety());
	}
	
	format_line(str, nchars);
	ostringstream oss(str);
	outstream(oss);
}	
	
void g_ostream::put(char c)
{
	string str = to_string(c);
	print_line(str);
}

g_ostream& g_ostream::operator<<(int t)
{
	string str = to_string(t);
	print_line(str);
	return *this;
}

g_ostream& g_ostream::operator<<(char t)
{
	string str = to_string(t);
	print_line(str);
	return *this;
}

g_ostream& g_ostream::operator<<(float t)
{
	string str = to_string(t);
	print_line(str);
	return *this;
}

g_ostream& g_ostream::operator<<(double t)
{
	string str = to_string(t);
	print_line(str);
	return *this;
}

g_ostream& g_ostream::operator<<(string t)
{
	string str = to_string(t);
	print_line(str);
	return *this;
}

g_ostream& g_ostream::operator<<(const char* t)
{
	string str = to_string(t);
	print_line(str);
	return *this;
}
