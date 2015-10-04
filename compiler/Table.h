// Table.h: this class is used to provide methods to construct a symbol table,
// an entry object stands for entries within the symbol table
// the entry structure gives the defination of one entry

#ifndef TABLE_H
#define TABLE_H

#include "ASTNODE.h"
#include <memory>
#include <vector>
#include <string>

using namespace std;

struct entry{
	enum TYPE
	{
		BOUND,
		UNBOUND,
		CONSTANT,
		PREDICATE,
	};

	TYPE type;					// distinct type of constant variable and predicate
	string str;					// for name attribute
	shared_ptr<Token> p;		// point to BOUND and NUMBER token
	vector<entry> symbols;		// hold symbols' entres of predicate

	entry(TYPE, shared_ptr<Token>);				// NUMBER and BOOUND constructor
	entry(TYPE, string, vector<entry>);			// PREDICATE constructor
};


class table{
public:
	vector<entry> entlist;
	void add(shared_ptr<ASTNODE>);			// passing one abstract syntax tree which representing a well formed horn clause, parsing each predicate within it and add them into the table  
	void print();							// print out predicate within table
	table & operator<<(const entry &ent);	// add one entry to the table
	entry* ifexist(string);					// to see if it exists
	bool ifpredicateexist(entry);				// extra credit
};

#endif