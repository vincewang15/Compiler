// ASTNODE.cpp: defines two constructors declared in ASTNODE.h,
// gives the specific implementation

#include "stdafx.h"
#include "ASTNODE.h"

ASTNODE::ASTNODE(){
	Type = UNKNOWN;
}


ASTNODE::ASTNODE(ASTNODEType typ, vector<Token> &v, list<Predicate> &headlist, list<shared_ptr<ASTNODE>> &ch){
	Type = typ;
	paren = v;
	predlist1 = headlist;
	child = ch;
}
