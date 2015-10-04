// Scanner.cpp: extracts strings from input file stream,
// create smart pointers that point to each token type


#include "stdafx.h"
#include "Scanner.h"
#include "Number.h"
#include "Label.h"
#include <fstream>

using namespace std;


scanner::scanner(ifstream &in) :input(in){}


scanner::operator bool() const{
	return !input.eof();		// if not the end of the file, return true
}

scanner &scanner::operator>> (shared_ptr<Token> &tok){
	string str;
	input >> str;			//put token from input file into token object
	// identify the token type
	if (str == "(")
		tok = shared_ptr<Token>(new Token(Token::LEFTPAREN, str));
	else if (str == ")")
		tok = shared_ptr<Token>(new Token(Token::RIGHTPAREN, str));
	else if (str == ",")
		tok = shared_ptr<Token>(new Token(Token::COMMA, str));
	else if (str == "^")
		tok = shared_ptr<Token>(new Token(Token::AND, str));
	else if (str == ":-")
		tok = shared_ptr<Token>(new Token(Token::SEPARATOR, str));
	else if (Scan_NUMBER(str))
		tok = shared_ptr<Token>(new Number(str));
	else if (Scan_UNBOUND(str))
		tok = shared_ptr<Token>(new Token(Token::UNBOUND, str, 0));
	else if (Scan_BOUND(str))
		tok = shared_ptr<Token>(new Token(Token::BOUND, str, 0));
	else if (Scan_LABEL(str))
		tok = shared_ptr<Token>(new Label(str));
	else
		tok = shared_ptr<Token>(new Token(Token::UNKNOWN, str));

	return *this;		// return the new scanner object after one token stream has been taken
}


bool scanner::Scan_NUMBER(string s){
	for (string::size_type i = 0; i < s.length(); ++i){
		if (!isdigit((s[i])))
			return false;
	}
	return true;
}

bool scanner::Scan_LABEL(string s){
	for (string::size_type i = 0; i < s.length(); ++i){
		if (!isalpha(s[i]))
			return false;
	}
	return true;
}

bool scanner::Scan_UNBOUND(string s){
	if (s.length()!=1||!isupper(s[0])) return false;
	else return true;
}

bool scanner::Scan_BOUND(string s){
	if (s.length() != 1 || !islower(s[0])) return false;
	else return true;
}