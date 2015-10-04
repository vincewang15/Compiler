// Scanner.h: extracts strings from input file stream, 
// classifies strings into different token types


#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"
#include <memory>

using namespace std;


class scanner{
public:
	ifstream &input;	
	scanner(ifstream &in);		// input from file
	scanner(string);			// for input from console
	operator bool() const;
	scanner &operator >> (shared_ptr<Token> &tok);

	// two scanner methods in order to verify whether the string is a LABEL or NUMBER
	bool Scan_NUMBER(string s);
	bool Scan_LABEL(string s);
	bool Scan_UNBOUND(string s);
	bool Scan_BOUND(string s);
};


#endif