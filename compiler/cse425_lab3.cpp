// Author: Shilai Lu, Chunyang Zhu, Botao Wang
// cse425_lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "Scanner.h"
#include "Token.h"
#include "ASTParser.h"
#include "ASTNODE.h"
#include "Table.h"
#include "Unifier.h"

using namespace std;

const int One = 1;
const int Two = 2;
const int Zero = 0;
const int NegativeOne = -1;
const int NegativeTwo = -2;
const int defaulrow = 46;
int displayrow;
int upper = 0;
int lower = 0;
vector<shared_ptr<ASTNODE>> myhornlist;
table tab;


void print_hornlist(int n){
	// first need to check the overflow of subscript of hornclause list
	// if overflow happens, we should stop at the top or bottom of the list
	if (upper + n < Zero){
		upper = Zero;
		lower = displayrow - 1;
		if (lower > int(myhornlist.size()))
			lower = myhornlist.size() - 1;
	}
	else if (lower + n > int(myhornlist.size() - 1)){
		upper = myhornlist.size() - displayrow;
		lower = myhornlist.size() - 1;
		if (upper < Zero)
			upper = Zero;
	}
	else{
		upper = upper + n;
		lower = lower + n;
	}

	cout << "Well-Formed HornClause List:" << endl;
	for (int i = upper; i <= lower; i++){
		cout << i + 1 << ". " << myhornlist[i]->paren.front().s << " ";
		// traverse each child
		list<shared_ptr<ASTNODE>>::iterator it = myhornlist[i]->child.begin();
		while (it != myhornlist[i]->child.end()){
			if ((*it)->paren.size() != Zero)	cout << (*it)->paren.front().s << " ";		// to display parens of body
			// traverse each predicate within each child
			list<Predicate>::iterator it1 = (*it)->predlist1.begin();
			while (it1 != (*it)->predlist1.end()){
				// traverse each token within each predicate within each child
				list<shared_ptr<Token>>::iterator it2 = it1->pretok.begin();
				while (it2 != it1->pretok.end()){
					cout << (*it2)->s << " ";
					it2++;
				}
				it1++;
			}
			if ((*it)->paren.size() != Zero)	cout << (*it)->paren.back().s << " ";		// to display parens of body
			it++;
		}
		cout << myhornlist[i]->paren.back().s << endl;
	}
}


bool ifidentical(shared_ptr<ASTNODE> horn1, shared_ptr<ASTNODE> horn2){
	if (horn1->predlist1.size() != horn2->predlist1.size()){
		return false;
	}
	// traverse each predicate within each horn clause
	list<Predicate>::iterator it1 = horn1->predlist1.begin();
	list<Predicate>::iterator it2 = horn2->predlist1.begin();
	for (; it1 != horn1->predlist1.end(); it1++, it2++){
		if (it1->pretok.size() != it2->pretok.size()){
			return false;
		}
		// traverse each token within each predicate
		list<shared_ptr<Token>>::iterator it11 = it1->pretok.begin();
		list<shared_ptr<Token>>::iterator it22 = it2->pretok.begin();
		for (; it11 != it1->pretok.end(); it11++, it22++) {
			if ((*it11)->s != (*it22)->s)
				return false;
		}
	}
	return true;
}


int main(int argc, char * argv[]){
	if (argc <= Two){		// only one command argument or none
		// if it is a number, otherwise display error information
		if (argc == Two && isdigit(*argv[One])){
			int row;		// to store the number to display
			istringstream(argv[One]) >> row;		// convert string to int
			if (row < defaulrow)
				displayrow = row;
		}
		else if (argc == One){ 
			displayrow = defaulrow;		// default value
		}
		else{
			cout << "argument should be an integer" << endl;
			return NegativeOne;
		}

		string str, command;
		while (true){
			cout << "input commands: ";
			getline(cin, str);
			istringstream str2(str);
			str2 >> command;		// to get the first keyword, in order to identify the operation following
			
			if (command == "process"){
				// the input file can be opened for reading, and if not should display a helpful usage message and then return a unique non-zero error code
				// input file is checked in parser function, which will return a non-zero code if it cannot open
				string filename;
				str2 >> filename;
				ifstream in(filename);
				if (!in){		// check if file exists
					cout << "Error Code: " << errno << endl << "No Such File" << endl;
					print_hornlist(Zero);
					continue;
				}
				// build abstract sytax tree, put all pointers to root nodes in the ASTROOT vector
				ASTParser ast;
				vector<shared_ptr<ASTNODE>> ASTROOT = ast.Parse(filename);
				vector<shared_ptr<ASTNODE>>::iterator nodeit;
				// put well formed hornclause into symble table	and deductive database of horn clause	
				for (nodeit = ASTROOT.begin(); nodeit < ASTROOT.end(); nodeit++){
					myhornlist.push_back(*nodeit);
					tab.add(*nodeit);
				}
				// display the list, first define the upper bound and lower bound of the list of hornclause
				// we assume to display the last displayrow hornclause
				if (displayrow < int(myhornlist.size())){
					upper = myhornlist.size() - displayrow;
					lower = myhornlist.size() - 1;
				}
				else{
					upper = Zero;
					lower = myhornlist.size() - 1;
				}
				print_hornlist(Zero);		// 0 indicates no scroll up or down
			}

			else if (command == "assert"){
				ofstream a_file("temp.txt"); //this creates a file for writing hornclause we input. 
				string token;
				while (str2 >> token){
					a_file << token;		//use this to write to it.
					a_file << " ";
				}
				a_file.close();
				// then put all tokens in that file we just wrote into a vector for abstract syntax tree construction
				ASTParser ast;
				vector<shared_ptr<ASTNODE>> ASTROOT = ast.Parse("temp.txt");
				vector<shared_ptr<ASTNODE>>::iterator nodeit;
				// put well formed hornclause into symble table	and deductive database of horn clause	
				for (nodeit = ASTROOT.begin(); nodeit < ASTROOT.end(); nodeit++){
					myhornlist.push_back(*nodeit);
					tab.add(*nodeit);
				}
				// display the list, first define the upper bound and lower bound of the list of hornclause
				// we assume to display the last displayrow hornclause
				if (displayrow < int(myhornlist.size())){
					upper = myhornlist.size() - displayrow;
					lower = myhornlist.size() - 1;
				}
				else{
					upper = Zero;
					lower = myhornlist.size() - 1;
				}
				print_hornlist(Zero);		// 0 indicates no scroll up or down
			}

			else if (command == "up"){
				string index;
				if (str2 >> index){
					int decrement;		// since scroll up subscript should decrease
					istringstream(index) >> decrement;
					decrement = -decrement;
					print_hornlist(decrement);
				}
				// if no specific number, scroll full screen
				else
					print_hornlist(-displayrow);
			}

			else if (command == "down"){
				string index;
				if (str2 >> index){
					int increment;		// subscript should increase
					istringstream(index) >> increment;
					print_hornlist(increment);
				}
				// if no specific number, scroll full screen
				else
					print_hornlist(displayrow);
			}

			else if (command == "resolve"){
				string index1, index2;

				if (myhornlist.size() == Zero){
					cout << endl << "Empty horn clause list" << endl << endl;
					continue;
				}
				// first check if there are two numbers provided
				if (str2 >> index1 && str2 >> index2) {}
				else{
					print_hornlist(Zero);
					cout << endl << "Please give two different non-negative number" << endl << endl;
					continue;
				}
				// convert to int type
				unsigned int idx1, idx2;
				istringstream(index1) >> idx1;
				istringstream(index2) >> idx2;
				// compare their value
				if (idx1 == idx2){
					print_hornlist(Zero);
					cout << endl << "Please give two different non-negative number" << endl << endl;
					continue;
				}
				// check subscribes are within the range of list
				if (idx1 > myhornlist.size() || idx2 > myhornlist.size()){
					print_hornlist(Zero);
					cout << endl << "Out of range, please give valid two different non-negative number" << endl << endl;
					continue;
				}
				// compare two horn clauses to see if they are identical
				if (ifidentical(myhornlist[idx1 - 1], myhornlist[idx2 - 1])){
					print_hornlist(Zero);
					cout << endl << "Two identical horn clauses, please give other two different non-negative number" << endl << endl;
					continue;
				}
				// unification algorithm
				unifier myunifier;
				vector<shared_ptr<ASTNODE>> newhorn = myunifier.unify(myhornlist[idx1 - 1], myhornlist[idx2 - 1], tab);
				if (newhorn.size() != Zero){
					for (auto i : newhorn)
						myhornlist.push_back(i);

					// define the upper and lower index first for display
					if (displayrow < int(myhornlist.size())){
						upper = myhornlist.size() - displayrow;
						lower = myhornlist.size() - 1;
					}
					else{
						upper = Zero;
						lower = myhornlist.size() - 1;
					}
					print_hornlist(Zero);
				}
				else{
					print_hornlist(Zero);
					cout << endl << idx1 << " and " << idx2 << " cannot be unified!" << endl << endl;
					continue;
				}
			}
			
			else if (command == "randomize"){
				string var, val;
				int max = -1, value;
				str2 >> var;
				// first check if is a valid bound variable
				if (var.length() != One || !islower(var[Zero])){
					cout << "Not a valid variable!" << endl;
					continue;
				}
				// check valid numeric constant
				if (str2 >> val){
					if (!(istringstream(val) >> max)){		// if it is a value
						cout << "Not a valid numeric constant!" << endl;
						continue;
					}
					if (max < Zero){		// if it is negative
						cout << "Not a valid numeric constant!" << endl;
						continue;
					}
				}
				// generate random variable
				srand((unsigned int) time(0));
				if (max > Zero)
					value = rand() % (max + 1);
				else if (max == Zero)
					value = Zero;
				else
					value = rand();
				// if variable does not exsit, push back to the table
				entry* thebound = tab.ifexist(var);
				if (!thebound){
					cout << "new BOUND variable added! With value attribute " << value << endl;
					entry boundent(entry::BOUND, shared_ptr<Token>(new Token(Token::BOUND, var, 0)));		// one bound variable entry	
					tab << boundent;
				}
				else{
					cout << thebound->p->s << " value changed to " << value << endl;
					thebound->p->num = value;
				}
			}

			else if (command == "set"){
				string var, val;
				int value;
				str2 >> var;
				// first check if is a valid bound variable
				if (var.length() != One || !islower(var[Zero])){
					cout << "Not a valid variable!" << endl;
					continue;
				}
				// check valid numeric constant
				str2 >> val;
				if (!(istringstream(val) >> value)){		// if it is a value
					cout << "Not a valid numeric constant!" << endl;
					continue;
				}
				if (value <= Zero){		// if it is non-negative
					cout << "Not a valid numeric constant!" << endl;
					continue;
				}
				// if not exsited, push back to the table
				entry* thebound = tab.ifexist(var);
				if (!thebound){
					cout << "new BOUND variable added! With value attribute 0" << endl;
					entry boundent(entry::BOUND, shared_ptr<Token>(new Token(Token::BOUND, var, 0)));		// one bound variable entry	
					tab << boundent;
				}
				else{
					cout << "value changed!" << endl;
					thebound->p->num = value;
				}
			}

			else if (command == "print"){
				tab.print();
			}

			else if (command == "exit"){
				return Zero;
			}

			else{
				cout << "Not valid command!" << endl;
				continue;
			}
		}
	}
	else{
		cout << "too many arguments" << endl;
		return NegativeTwo;
	}

	return Zero;
}
