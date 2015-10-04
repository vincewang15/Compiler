// Table.cpp: receives the root of a well formed horn clause,
// add predicates and BOUND tokens within that clause into the table

#include "stdafx.h"
#include "Table.h"
#include "Token.h"
#include "Predicate.h"
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>

using namespace std;

// NUMBER and BOUND constructor
entry::entry(TYPE t, shared_ptr<Token> tok){
	type = t;
	p = tok;
}

// PREDICATE constructor
entry::entry(TYPE t, string s, vector<entry> l){
	type = t;
	str = s;
	symbols = l;
}


// put each entry into the table
table& table::operator<<(const entry &ent){
	entlist.push_back(ent);
	return *this;
}


void table::add(shared_ptr<ASTNODE> ptr){
	list<shared_ptr<ASTNODE>> childlist = (*ptr).child;
	list<shared_ptr<ASTNODE>>::iterator it = childlist.begin();		// in order to access head and body nodes

	// run two times for head and body nodes that including predicate
	while (it != childlist.end()){
		shared_ptr<ASTNODE> curnode = *it;

		// the loop is designed to traverse each predicate within body or head
		list<Predicate>::iterator it1 = (*curnode).predlist1.begin();
		while (it1 != (*curnode).predlist1.end()){
			Predicate pred = *it1;		// get the current predicate
			vector<entry> symbollist;		

			list<shared_ptr<Token>>::iterator it2 = pred.pretok.begin();	// in order to iterate each token in one predicate
			string name = (*++it2)->s;		// skip the first leftparen token, get the name attribute

			// put constant or variables into the table, and set pointers attribute of predicate 
			it2++;
			while (it2 != pred.pretok.end()){
				// construct const
				if ((*it2)->type == Token::NUMBER){
					entry intent(entry::CONSTANT, *it2);		// one constant entry										
					symbollist.push_back(intent);		// set pointer to this symbol
				}

				//construct variable
				if ((*it2)->type == Token::UNBOUND){
					entry unboundent(entry::UNBOUND, *it2);
					symbollist.push_back(unboundent);		// set pointer to this symbol
				}
				
				// construct bound variable
				if ((*it2)->type == Token::BOUND){
					entry* thebound = ifexist((*it2)->s);
					// if not exsited, push back to the table
					if (!thebound){
						entry boundent(entry::BOUND, *it2);		// one bound variable entry
						*this << boundent;				// push the BOUND entry to the table
						symbollist.push_back(boundent);
					}
					else
						symbollist.push_back(*thebound);		// set pointer to this symbol
				}

				it2++;
			}

			entry preent(entry::PREDICATE, name, symbollist);		// one predicate entry
			if (ifpredicateexist(preent)){}
			else
				*this << preent;		// push predicate entry into table

			it1++;			// for next predicate
		}
		it++;
	}
}


void table::print(){
	cout << "Symbol Table:" << endl;

	for (auto i : (*this).entlist){
		if (i.type == entry::BOUND)
			cout << i.p->s << " " << i.p->num << endl;
		else if (i.type == entry::PREDICATE){		
			// print out the name attribute
			cout << i.str << " ";
			// print out variables
			for (unsigned int j = 0; j < i.symbols.size(); j++){
				cout << i.symbols[j].p->s << " ";
			}
			cout << endl;
		}
	}
}


entry* table::ifexist(string s){
	vector<entry>::iterator it = (*this).entlist.begin();
	for (; it != (*this).entlist.end(); it++){
		if (it->type != entry::BOUND){
			continue;
		}
		if (it->p->s == s)
			return &(*it);
	}

	return NULL;
}

// return true if predicate existed
bool table::ifpredicateexist(entry e){
	int flag = 0;		// if flag equals symbol order's size, which means same
	for (auto i : (*this).entlist){
		if (i.type == entry::PREDICATE){
			if (i.str == e.str){
				if (i.symbols.size() == e.symbols.size()){
					for (unsigned int j = 0; j < i.symbols.size(); j++){
						if (i.symbols[j].p->s != e.symbols[j].p->s)
							break;
						else
							flag++;
					}
					if (flag == e.symbols.size())
						return true;
				}
			}
		}
		flag = 0;
	}
	return false;
}
