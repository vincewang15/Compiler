// Number.h: derived class of Token, Number is the string consists entirely of decimal digit characters

#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include "Token.h"

using namespace std;

struct Number : Token {
	unsigned int con;
	Number(string &s);
};

#endif