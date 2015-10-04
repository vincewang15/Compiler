// Unifier.h: all methods that will be used in unification algorithm is declared in this class

#ifndef UNIFIER_H
#define UNIFIER_H

#include "Table.h"

struct  subRule{		
	vector<string> rulelist;	// member variable, each rule is expressed by a string
	subRule();				// default constructor
	void addRule(string);			// add rule to the list
};


class unifier{
private:
	bool match(vector<Token>, vector<Token>);		// if two entries has same name and number of symbols
	bool compare(vector<Token>, vector<Token>);		// if two entries has same constant at same position
	int substitute(vector<Token>&, vector<Token>&, table);	// do substition according to rule
	void replace(vector<Token>&, string);		// replce certain variable by another symbol according to a rule
	
public:
	vector<shared_ptr<ASTNODE>> unify(shared_ptr<ASTNODE>, shared_ptr<ASTNODE>, table);
};





#endif