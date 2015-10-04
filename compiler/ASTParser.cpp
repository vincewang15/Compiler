// ASTParser.cpp: defines those parsers declared in ASTParser.h
// returns a vector containing all root nodes of well-formed horn clauses 

#include "stdafx.h"
#include "ASTParser.h"
#include "Label.h"
#include "Number.h"
#include "Predicate.h"
#include "Scanner.h"
#include <sstream>
#include <fstream>



vector<shared_ptr<ASTNODE>> ASTParser::Parse(string ifname){
	// store smart pointers to each token object in a vector 
	string str;
	shared_ptr<Token> tok;
	vector<shared_ptr<ASTNODE>> ASTROOT;
	ifstream in(ifname);
	scanner sca(in);

	while (sca){
		sca >> tok;		// to classify Token objects in the input horn clause string
		tokvec.push_back(tok);
	}

	// recursive descent abstract syntax tree parsing
	while (!tokvec.empty()){
		shared_ptr<ASTNODE> astroot = horn();
		if (astroot != NULL)
			ASTROOT.push_back(astroot);
		// handle exception
		else if (tokvec.size() <= 1)
			break;
	}
	in.close();
	return ASTROOT;
}

shared_ptr<ASTNODE> ASTParser::horn(){
	list<Predicate> predlist;
	list<shared_ptr<ASTNODE>> child;
	vector<Token> parenpair;

	// to build the parenthesis object which is also deemed as one member of the node
	if (tokvec.front()->type == Token::LEFTPAREN){
		parenpair.push_back(*(tokvec.front()));
		GetNextToken();		// consume the leftparen
	}
	else{
		GetNextToken();
		return NULL;
	}

	// construct head node
	shared_ptr<ASTNODE> headnode = head();
	if (headnode != NULL){
		predlist.push_back((*headnode).predlist1.front());
		child.push_back(headnode);		// push it into the child list

		// extra credit, for possible multiple head part, if it doesn't work, delete this if block
		if (tokvec[1]->type != Token::LEFTPAREN){
			shared_ptr<ASTNODE> prenode_optional = predicate();
			while (prenode_optional != NULL){
				predlist.push_back((*prenode_optional).predlist1.front());
				child.push_back(prenode_optional);
				if (tokvec[1]->type != Token::LEFTPAREN){
					prenode_optional = predicate();
				}
				else
					prenode_optional = NULL;
			}
		}
	}
	else{
		GetNextToken();
		return NULL;
	}

	// construct body node
	shared_ptr<ASTNODE> bodynode = body();
	// for optional body node, need to check if there is a body, and then check if there is still body nodes left
	if (bodynode != NULL){
		list<Predicate>::iterator itt = (*bodynode).predlist1.begin();
		while (itt != (*bodynode).predlist1.end()){
			predlist.push_back((*itt));
			itt++;
		}
		child.push_back(bodynode);
	}

	// to build the parenthesis object which is also deemed as one member of the node
	if (tokvec.front()->type == Token::RIGHTPAREN){
		parenpair.push_back(*(tokvec.front()));
		GetNextToken();
	}
	else{
		GetNextToken();
		return NULL;
	}

	// construct the root node
	shared_ptr<ASTNODE> root(new ASTNODE(ASTNODE::ROOT, parenpair, predlist, child));
	return root;
}


shared_ptr<ASTNODE> ASTParser::head(){
	list<Predicate> predlist;
	list<shared_ptr<ASTNODE>> child;

	shared_ptr<ASTNODE> prenode = predicate();
	if (prenode != NULL){
		predlist.push_back((*prenode).predlist1.front());
		child.push_back(prenode);

		// extra credit, for possible multiple head part, if it doesn't work, delete this if block
		if (tokvec[1]->type != Token::LEFTPAREN){
			shared_ptr<ASTNODE> prenode_optional = predicate();
			while (prenode_optional != NULL){
				predlist.push_back((*prenode_optional).predlist1.front());
				child.push_back(prenode_optional);
				if (tokvec[1]->type != Token::LEFTPAREN){
					prenode_optional = predicate();
				}
				else
					prenode_optional = NULL;
			}
		}

	}
	else{
		return NULL;
	}

	// construct the head node
	vector<Token> parenpair;
	shared_ptr<ASTNODE> headnode(new ASTNODE(ASTNODE::HEAD, parenpair, predlist, child));
	return headnode;
}


shared_ptr<ASTNODE> ASTParser::body(){
	list<Predicate> predlist;
	list<shared_ptr<ASTNODE>> child;
	vector<Token> parenpair;

	// to build the parenthesis object which is also deemed as one member of the node
	if (tokvec.front()->type == Token::LEFTPAREN){
		parenpair.push_back(*(tokvec.front()));
		GetNextToken();		// consume the leftparen
	}
	else{
		return NULL;
	}

	shared_ptr<ASTNODE> prenode = predicate();
	if (prenode != NULL){
		predlist.push_back((*prenode).predlist1.front());
		child.push_back(prenode);

		// for possible multiple body part
		shared_ptr<ASTNODE> prenode_optional = predicate();
		while (prenode_optional != NULL){
			predlist.push_back((*prenode_optional).predlist1.front());
			child.push_back(prenode_optional);
			prenode_optional = predicate();
		}

		// to build the parenthesis object which is also deemed as one member of the node
		if (tokvec.front()->type == Token::RIGHTPAREN){
			parenpair.push_back(*(tokvec.front()));
			GetNextToken();
		}
		else{
			return NULL;
		}

		// construct body node
		shared_ptr<ASTNODE> bodynode(new ASTNODE(ASTNODE::BODY, parenpair, predlist, child));
		return bodynode;
	}
	else{
		return NULL;
	}	
}


shared_ptr<ASTNODE> ASTParser::predicate(){
	list<Predicate> predlist;
	list<shared_ptr<ASTNODE>> child;
	Predicate pred;

	if (tokvec.empty())
		return NULL;

	// check the first left paren
	if (tokvec.front()->type == Token::LEFTPAREN){
		pred.pretok.push_back(tokvec.front());		// leftparen is also one element of predicate, so push it back to the token list of predicate
		GetNextToken();
	}
	else{
		return NULL;
	}

	shared_ptr<Token> nametok = name();
	if (nametok != NULL){
		pred.pretok.push_back(nametok);
		GetNextToken();
		shared_ptr<Token> symboltok = symbol();
		while (symboltok != NULL){
			pred.pretok.push_back(symboltok);
			GetNextToken();
			symboltok = symbol();
		}

		// check the right paren
		if (tokvec.front()->type == Token::RIGHTPAREN){
			pred.pretok.push_back(tokvec.front());
			GetNextToken();
		}
		else{
			return NULL;
		}

		predlist.push_back(pred);
		child.push_back(NULL);

		// construct predicate node
		vector<Token> parenpair;
		shared_ptr<ASTNODE> predicatenode(new ASTNODE(ASTNODE::PREDICATE, parenpair, predlist, child));
		return predicatenode;
	}
	else{
		return NULL;
	}
}


shared_ptr<Token> ASTParser::name(){
	shared_ptr<Token> nametok;
	if (tokvec.front()->type == Token::LABEL){
		nametok = tokvec.front();
		return nametok;
	}
	else{
		return NULL;
	}
}


shared_ptr<Token> ASTParser::symbol(){
	shared_ptr<Token> symboltok;
	if (tokvec.front()->type == Token::UNBOUND)
		symboltok = tokvec.front();
	else if (tokvec.front()->type == Token::NUMBER)
		symboltok = tokvec.front();
	else if (tokvec.front()->type == Token::BOUND)
		symboltok = tokvec.front();
	else
		return NULL;

	return symboltok;
}


void ASTParser::GetNextToken(){
	if (!tokvec.empty()){
		curToken = tokvec.front();
		tokvec.erase(tokvec.begin());
	}
	else
		(*curToken).type = Token::UNKNOWN;		// handle EOF
}
