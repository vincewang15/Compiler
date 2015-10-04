// Unifier.cpp: passed by two well formed horn clauses, 
// performs any substitutions needed to unify the head of the first clause with a predicate in the body of the second clause
// then replace the matched predicate in the copy of the second Horn clause with the (possibly updated after unification) body of the copy of the first clause
// returns the vectors that contains new generated well formed horn clauses 

#include "stdafx.h"
#include "Unifier.h"
#include "ASTParser.h"
#include <vector>
#include <iterator>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>


using namespace std;

// default constructor
subRule::subRule(){}

// add a rule into the rule list
void subRule::addRule(string s){
	rulelist.push_back(s);
}

subRule sr;						// hold substitution rule list

vector<shared_ptr<ASTNODE>> unifier::unify(shared_ptr<ASTNODE> horn1, shared_ptr<ASTNODE> horn2, table tab){
	vector<shared_ptr<ASTNODE>> newhornvec;
	int matched = 0;		// to maintain the index of the matched predicate, initialized to 1 is because it counts from the second predicate as the first is head
	
	// after finding another predicate, we begin matching, comparing the symbols and substituting
	// if they are not match, break out current loop to find the next predicate
	for (unsigned int n = 0; n < horn2->child.back()->predlist1.size(); n++, matched++){		// n indicates the index of the predicate within a body	
		// create two working copies
		// take out token from head of horn1
		vector<Token> head1;
		for (auto i : horn1->child.front()->predlist1.front().pretok){
			head1.push_back(*i);
		}
		// take out token from body of horn2
		vector<vector<Token>> body1;
		for (auto i : horn1->child.back()->predlist1){
			vector<Token> tmp;
			for (auto j : i.pretok)
				tmp.push_back(*j);
			body1.push_back(tmp);
		}
		// take out token from head of horn2
		vector<Token> head2;
		for (auto i : horn2->child.front()->predlist1.front().pretok){
			head2.push_back(*i);
		}
		// take out token from body of horn2
		vector<vector<Token>> body2;
		for (auto i : horn2->child.back()->predlist1){
			vector<Token> tmp;
			for (auto j : i.pretok)
				tmp.push_back(*j);
			body2.push_back(tmp);
		}

		// if length and name are not same finding the next
		if (!match(head1, body2[n])){
			continue;
		}

		// if constants are not same, finding the next predicate
		if (!compare(head1, body2[n])){
			continue;
		}

		// start substitutng
		if (substitute(head1, body2[n], tab) != -1){
			// unification of the head predicate
			for (auto r : sr.rulelist){
				replace(head2, r);
			}

			// unification of the body of the new generated horn clause
			// remove the matched predicate first
			vector<vector<Token>>::iterator body1it = body1.begin();
			vector<vector<Token>>::iterator body2it = body2.begin();
			for (unsigned int i = 0; i < body2.size(); i++){
				// if the first horn clause is a fact, just remove the matched predicate
				if (i == matched && horn1->child.size() == 1){
					body2.erase(body2it++);
					break;			// since every time there is noly one matched predicate, as if we finish the manipulation on it, we needn't to do the rest iteration
				}
				// else just remove the matched predicate, and insert the body of the first horn at the position of matched predicate in the body of horn2
				else if (i == matched && horn1->child.size() > 1){
					body2.erase(body2it);
					while (body1it != body1.end()){
						body2it = body2.begin();		// "body2it" no longer valid, get a new one
						body2.insert(body2it + matched, *body1it);
						body1it++;
						matched++;		// need to put the next predicate behind
					}
					break;		// since every time there is noly one matched predicate, as if we finish the manipulation on it, we needn't to do the rest iteration
				}
				// else if it is not the end of body, and its not the matched predicate, move to the next predicate
				else{
					body2it++;
				}
			}
			// unifying the body of horn 2
			for (unsigned int i = 0; i < body2.size(); i++){
				for (auto r : sr.rulelist)
					replace(body2[i], r);
			}

			// write the new generated horn clause into a file stream for ASTParser to parse the abstract syntax tree
			ofstream out("temp.txt");
			out << "(" << " ";
			for (auto i : head2){
				out << i.s << " ";
			}
			out << "(" << " ";
			for (auto i : body2){
				for (auto j : i)
					out << j.s << " ";
			}
			out << ")" << " " << ")";
			out.close();
			// parse this new horn clause
			ASTParser ast;
			shared_ptr<ASTNODE> newhorn = ast.Parse("temp.txt").front();
			newhornvec.push_back(newhorn);
		}
		matched = 0;
		sr.rulelist.clear();
	}

	return newhornvec;
}


int unifier::substitute(vector<Token> &head, vector<Token> &body, table tab){
	for (unsigned int i = 2; i < head.size() - 1; i++){
		string newrule;
		// if two constant, check if they are the same, move to the next position
		if (head[i].type == Token::NUMBER && body[i].type == Token::NUMBER){
			if (head[i].s == body[i].s){
				continue;
			}
			else{	// if they are mis-matched constant, stop unifying and break the loop, in this case that means return to the caller
				return -1;
			}		
		}

		// if one bound and one unbound
		else if ((head[i].type == Token::BOUND && body[i].type == Token::UNBOUND) || (head[i].type == Token::UNBOUND && body[i].type == Token::BOUND)){
			if (head[i].type == Token::BOUND){
				newrule = head[i].s + " / " + body[i].s;
			}
			else
				newrule = body[i].s + " / " + head[i].s;
		}

		// if one bound and one number at current position
		else if ((head[i].type == Token::BOUND && body[i].type == Token::NUMBER) || (head[i].type == Token::NUMBER && body[i].type == Token::BOUND)){
			if (head[i].type == Token::BOUND){
				entry* thebound = tab.ifexist(head[i].s);
				if (to_string(thebound->p->num) != body[i].s)		// bound must has the same value as number
					return -1;
				newrule = body[i].s + " / " + head[i].s;
			}
			else{
				entry* thebound = tab.ifexist(body[i].s);
				if (head[i].s != to_string(thebound->p->num))		// bound must has the same value as number
					return -1;
				newrule = head[i].s + " / " + body[i].s;
			}
		}

		// if one unbound and one number
		else if ((head[i].type == Token::UNBOUND && body[i].type == Token::NUMBER) || (head[i].type == Token::NUMBER && body[i].type == Token::UNBOUND)){
			if (head[i].type == Token::UNBOUND)
				newrule = body[i].s + " / " + head[i].s;
			else
				newrule = head[i].s + " / " + body[i].s;
		}

		// if there are two unbound
		else if (head[i].type == Token::UNBOUND && body[i].type == Token::UNBOUND){
			newrule = head[i].s + " / " + body[i].s;
		}

		// if there are two identical bound, do nothing
		else if (head[i].type == Token::BOUND && body[i].type == Token::BOUND){
			if (head[i].s != body[i].s){
				return -1;
			}
			else{
				continue;
			}
		}

		// else these two predicate cannot be matched
		else{
			return -1;
		}

		sr.addRule(newrule);	// add rules into the lruleist
		// replace all same variables by the other variables in both working copies
		replace(head, newrule);
		replace(body, newrule);
	}
	return 0;
}


void unifier::replace(vector<Token> &e, string s) {
	// split the rule string such as '3 / X' into seperate part
	string left, slash, right;
	istringstream ss(s);
	ss >> left >> slash >> right;
	// extract label and integer for substitution in next step
	string name = right, name1, value;		// the variable to be substituted by other variable or constant always shows on the right side of slash
	for (string::size_type i = 0; i < left.length(); i++){
		// if the left side is an integer
		if (!isalpha(left[i])){
			value = left;
			break;
		}
		else
			name1 = left;
	}
	// modify current attribute
	for (unsigned int i = 2; i < e.size() - 1; i++){
		// substitute constant for variable
		if (e[i].s == name && name1 == ""){
			e[i].s = value;
			e[i].type = Token::NUMBER;
			continue;
		}
		// substitute variable for variable
		else if (e[i].s == name && name1 != ""){
			e[i].s = name1;
			e[i].type = Token::BOUND;
			continue;
		}
	}
}


bool unifier::match(vector<Token> head, vector<Token> body){
	if (head[1].s == body[1].s){
		if (head.size() == body.size())
			return true;
		else
			return false;
	}
	else
		return false;
}


bool unifier::compare(vector<Token> head, vector<Token> body){
	// multiple symbol comparison
	for (unsigned int i = 2; i < head.size(); i++){
		// to see if two constants at same position are equal, if it's a variable, constval is 0, and do nothing
		if (head[i].type == Token::NUMBER && body[i].type == Token::NUMBER){
			if (head[i].s != body[i].s)
				return false;
		}
	}
	return true;
}
