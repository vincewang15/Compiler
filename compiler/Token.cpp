// Token.cpp: implements different constructors for different token types

#include "stdafx.h"
#include "Token.h"


Token::Token(){
	type = UNKNOWN;
	s = "";
}

Token::Token(token_type t, const string &str){
	type = t;
	s = str;
}

Token::Token(token_type t, const string &str, const int &n){
	type = t;
	s = str;
	num = n;
}

