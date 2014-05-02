// Program for modelling associations with multiplicity of one to many and many to many.

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Attribute {
	string name;
	string value;

	Attribute () : name ("UNDEFINED"), value ("UNDEFINED") {	}
	Attribute (const string& a_name, const string& a_value) : name (a_name), value (a_name) {	}
};

struct Entity {
	string name;
	vector <Attribute> attr;

	Entity () : name ("UNDEFINED"), attr () {	}
	Entity (const string& e_name) : name (e_name), attr () {	}
};

struct Association_one_many {
	string name;
	Entity from;
	vector <Entity> to;

	Association_one_many () : name ("UNDEFINED"), from (), to () {	}
	Association_one_many (const string& a_name) : name (a_name), from (), to () {	}

	void print (ostream& out = cout) const {
		out << name << ":\n"
		    << from.name << " ----> ";
		for (size_t i = 0; i < to.size(); ++i)
			out << to[i].name << ((i == to.size() - 1) ? "" : ", ");
		out.put('\n');
	}
};

struct Association_many_many {
	string name;
	vector <Entity> from;
	vector <Entity> to;

	Association_many_many () : name ("UNDEFINED"), from (), to () {	}
	Association_many_many (const string& a_name) : name (a_name), from (), to () {	}

	void print (ostream& out = cout) const {
		out << name << ":\n";
		for (size_t i = 0; i < from.size(); ++i)
			out << from[i].name << ((i == from.size() - 1) ? "" : ", ");
		out << "----> ";
		for (size_t i = 0; i < to.size(); ++i)
			out << to[i].name << ((i == to.size() - 1) ? "" : ", ");
		out.put('\n');
	}
};

int main () {
	Entity book1 ("K & R");
	book1.attr.push_back (Attribute ("Name", "The C Programming Language"));
	book1.attr.push_back (Attribute ("Category", "Programming; C language"));

	Entity book2 ("TUPE");
	book2.attr.push_back (Attribute ("Name", "The UNIX Programming Environment"));
	book2.attr.push_back (Attribute ("Category", "Operating Systems; UNIX"));

	Entity author1 ("Kernighan");
	author1.attr.push_back (Attribute ("Name", "Brian Kernighan"));

	Entity author2 ("Ritchie");
	author2.attr.push_back (Attribute ("Name", "Dennis Ritchie"));

	Association_one_many book_authors("Book:Author(s)");
	book_authors.from = book1;
	book_authors.to.push_back (author1);
	book_authors.to.push_back (author2);

	Association_many_many books_authors ("Book(s):Author(s)");
	books_authors.from.push_back (book1);
	books_authors.from.push_back (book2);
	books_authors.to.push_back (author1);
	books_authors.to.push_back (author2);

	book_authors.print ();
	books_authors.print ();

	return 0;
}
