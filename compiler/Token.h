// Token.h: Tokens make up a well-formed horn clause,
// this header file gives the defination of a token

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

struct Token {
public:
	enum token_type{
		AND,
		COMMA,
		LABEL,
		LEFTPAREN,
		NUMBER,
		RIGHTPAREN,
		SEPARATOR,
		UNBOUND,
		BOUND,
		UNKNOWN
	};

	token_type type;
	string s;
	int num;		// for BOUND and UNBOUND

	Token();
	Token(token_type t, const string &s);		// for others token
	Token(token_type t, const string &s, const int &n);		// for BOUND and UNBOUND
};
#endif