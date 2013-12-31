#ifndef TFSTR_GUARD
#define TFSTR_GUARD

#include <string>
#include <sstream>

using namespace std;

template<class T>
T from_string(string& s);

template<class T>
string to_string(T i);

#endif
