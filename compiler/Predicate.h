// Predicate.h: a predicate contains a list of token

#ifndef PREDICATE_H
#define PREDICATE_H

#include <list>
#include <memory>
#include "Token.h"

using namespace std;

struct Predicate{
	list<shared_ptr<Token>> pretok;
	Predicate();
};

#endif