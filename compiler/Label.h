// Label.h: derived class of Token, Label is the string consists entirely of lowercase or uppercase alphabetic characters

#ifndef LABEL_H
#define LABEL_H

#include "Token.h"
using namespace std;

struct Label : Token {
	Label(string &s);
};

#endif