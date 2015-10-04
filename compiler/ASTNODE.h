// ASTNODE.h : gives the structure of the nodes of abstract syntax tree
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <memory>
#include <vector>
#include "Predicate.h"

using namespace std;


class ASTNODE{
public:
	enum ASTNODEType
	{
		ROOT,
		HEAD,
		BODY,
		PREDICATE,
		LEFTPAREN,
		RIGHTPAREN,
		UNKNOWN
	};

	ASTNODEType Type;		// node type
	vector<Token> paren;		// to keep the information of parenthesis
	list<Predicate> predlist1;
	list<shared_ptr<ASTNODE>> child;		// a list stored the pointer to each child node of current node

	ASTNODE();
	ASTNODE(ASTNODEType typ, vector<Token> &v, list<Predicate> &prelist, list<shared_ptr<ASTNODE>> &ch);		// constructor for head and body node	
};

#endif