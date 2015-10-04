// ASTParser.h: provides recursive descent parsing solutions to parse the syntax tree according to the input stream,
// declares six parsers for each non-terminal token 

#ifndef ASTPARSER_H
#define ASTPARSER_H

#include <memory>
#include <list>
#include "Predicate.h"
#include "ASTNODE.h"
#include "Token.h"
#include <vector>

using namespace std;


class ASTParser{
public:
	vector<shared_ptr<ASTNODE>> Parse(string ifname);

private:
	shared_ptr<Token> curToken;		// current Token
	vector<shared_ptr<Token>> tokvec;		// a vector hold all tokens

	// recursive parsing function
	shared_ptr<ASTNODE> horn();
	shared_ptr<ASTNODE> head();
	shared_ptr<ASTNODE> body();
	shared_ptr<ASTNODE> predicate();
	shared_ptr<Token> name();
	shared_ptr<Token> symbol();

	void GetNextToken();
	void match(Token::token_type expected);		// consume the unneeded matched token like COMMA AND SEPERATOR, otherwise, do nothing

};


#endif