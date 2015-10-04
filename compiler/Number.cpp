// Number.cpp: initializes the member variable, converts the string into a unsigned int type

#include "stdafx.h"
#include "Number.h"
#include <sstream>


Number::Number(string &s) :Token(NUMBER, s){
	istringstream str(s);
	str >> con;
}