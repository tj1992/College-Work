#include "tfstr.hpp"

template<class T>
T from_string(string& s)
{
  stringstream is(s);
	s.clear();
	T i = T();
	is>>i>>s;
	return i;
}

template<class T>
string to_string(T i)
{
	ostringstream os;
	os<<i;
	return os.str();
}
