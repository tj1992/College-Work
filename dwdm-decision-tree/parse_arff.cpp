#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
#include <algorithm>

using namespace std;

enum Type { NOMINAL, REAL, NUMERIC, BAD, MISSING };
struct Field {
	enum Type type;
	union Value_type {
		double rval;
		long nval;

		Value_type() { }
		~Value_type() { }

	} value;

	Field() : type(BAD) {	 }
	Field (enum Type) : type(MISSING) {	}

	Field(long val, int) : type(NOMINAL) { 
		value.nval = val;
	}

	Field(double val) : type(REAL) {
		value.rval = val;
	}

	Field(long val) : type(NUMERIC) {
		value.nval = val;
	}

	Field(const Field& f) {
		this->type = f.type;
		switch (this->type) {
			case NOMINAL:
			case NUMERIC:
				this->value.nval = f.value.nval;
				break;
			case REAL:
				this->value.rval = f.value.rval;
				break;
		}
	}

	long get_num() const { return value.nval; }
	long get_real() const { return value.rval; }

	~Field() { }
};

string strip_spaces(string& str) {
	string s;
	for (int i = 0; i < str.length(); ++i)
		if (str[i] != ' ')
			s.push_back(str[i]);
	str = s;
	return str;
}

template<typename T>
bool is_type(const string& s, T& i) {
	stringstream ss(s);
	return (ss>>i);
}

struct Row {
	vector<Field> fields;
};

struct Attribute {
	string name;
	enum Type type;
	vector<string> domain;

	Attribute() : name("UNDEFINED"), type(BAD)
	{	}
};

Field make_field(string& str, Attribute& attr) {
	strip_spaces(str);
	long v;
	double d;
	vector<string>::iterator iter;
	if (str == "?")
		return Field(MISSING);
	else if (attr.type == NUMERIC && is_type<long>(str, v))
		return Field(v);
	else if (attr.type == REAL && is_type<double>(str, d))
		return Field(d);
	else if (attr.type == NOMINAL && (iter=find(attr.domain.begin(), attr.domain.end(), str)) != attr.domain.end())
		return Field(iter-attr.domain.begin(), 0);
	else
		return Field();
}

struct Table {
	string name;
	vector<Attribute> header;
	vector<Row> data;

	bool add_data_field(const Field& f, int index) {
		if (f.type == BAD) {
			cerr<<"DOMAIN ERROR: for field "<<header[index].name<<": value does not belong to the domain of "<<header[index].name<<'\n'; 
			return false;
		}
		data.back().fields.push_back(f);
		return true;
	}

	void add_row() {
		data.push_back(Row());
	}

	void add_header(const Attribute& h) {
		header.push_back(h);
	}
};

void print_table(Table& tab) {
	cout<<tab.name<<'\n';

	for (int i = 0; i < tab.header.size(); ++i)
		cout<<tab.header[i].name<<'\t';
	for (int i = 0; i < tab.data.size(); ++i) {
		cout<<'\n';
		for (int j = 0; j < tab.data[i].fields.size(); ++j) {
			switch (tab.data[i].fields[j].type) {
				case NOMINAL:
					cout<<tab.header[j].domain[tab.data[i].fields[j].get_num()]<<'\t';
					break;
				case NUMERIC:
					cout<<tab.data[i].fields[j].get_num()<<'\t';
					break;
				case REAL:
					cout<<tab.data[i].fields[j].get_real()<<'\t';
					break;
				case MISSING:
					cout<<'?'<<'\t';
					break;
			}
		}
	}
	cout<<'\n';
}

void make_vector(vector<string>& v, string s) {
	strip_spaces(s);
	s.push_back(',');
	stringstream ss(s);
	while (getline(ss, s, ','))
		v.push_back(s);
}

string tolower(string& str) {
	for (int i = 0; i < str.length(); ++i)
		str[i] = tolower(str[i]);
	return str;
}

bool isspace(string& str) {
	for (int i = 0; i < str.length(); ++i)
		if (!isspace(str[i]))
			return false;
	return true;
}

bool is_comment(const string& str) {
	int i;
	for (i = 0; i < str.size() && isspace(str[i]); ++i)
		;
	if (str[i] == '%')
		return true;
	return false;
}

Table* read_arff(string name) {
	ifstream fin(name);

	if (!fin) {
		cerr<<"Error opening file "<<name<<"\nAborting!\n";
		return nullptr;
	}

	Table* tab = new Table;
	string word, line;
	char c;

	while (getline(fin, line) && (isspace(line) || is_comment(line)))
		;

	stringstream ss(line);
	if (ss>>word) {
		if (tolower(word) == "@relation") {
			if (getline(ss, word))
				tab->name = word;
		}
		else {
			cerr<<"Parsing error: expected @relation got "<<word<<'\n';
			return nullptr;
		}
	}

	while (!fin.eof() && getline(fin, line)) {
		if (isspace(line) || is_comment(line))
			continue;
		string name, kind;
		Attribute attr;
		stringstream ss(line);

		if (ss>>word && tolower(word) != "@attribute") {
			break;
		}

		if (ss>>name>>c) {
			attr.name = name;
			if (c == '{') {
				getline(ss, word);
				int i;
				if ((i = word.find("}")) == string::npos) {
					cerr<<"Parsing error: matching } not found\n";
					return nullptr;
				}
				make_vector(attr.domain, string(word.begin(), word.begin()+i));
				attr.type = NOMINAL;
			}
			else {
				ss.unget();
				if (ss>>kind) {
					if (tolower(kind) == "numeric")
						attr.type = NUMERIC;
					else if (kind == "real")
						attr.type = REAL;
					else {
						cerr<<"Parsing error: unknown type "<<kind<<'\n';
						return nullptr;
					}
				}
				else {
					cerr<<"Parsing error: syntax error: type expected\n";
					return nullptr;
				}
			}
		}

		tab->add_header(attr);
	}

	if (word != "@data") {
			cerr<<"Parsing error: syntax error: @data expected got "<<word;
			return nullptr;
		}
	
	while (!fin.eof() && getline(fin, line)) {
		if (isspace(line) || is_comment(line))
			continue;

		line.push_back(',');
		stringstream ss(line);

		tab->add_row();
		for (int i = 0; i < tab->header.size(); ++i) {
			if (!getline(ss,word, ',')) {
				cerr<<"Parsing error: data fields less than attributes' length\n";
				return nullptr;
			}
			if (!(tab->add_data_field(make_field(word, tab->header[i]), i))) {
				cerr<<"Parsing error: illegal format\n";
				return nullptr;
			}
		}
	}
	return tab;
}

Table* init() {
	Table* tab = read_arff("classifier.arff");
	if (tab != nullptr)
		print_table(*tab);

	return tab;
}
