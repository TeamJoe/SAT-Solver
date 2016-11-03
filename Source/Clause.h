class Clause;

#ifndef CLAUSE_H
#define CLAUSE_H

#include <list>

#include "Literal.h"
#include "SAT.h"
#include "ClauseState.h"
#include "SATState.h"

#include "Constants.h"

using namespace std;

class Clause
{
private:
	Literal ** clause;
	unsigned int _size;

	SAT * _parent;
	list <Clause *>::const_iterator listPointer;
protected:
	Clause(SAT * _parent);
	Clause(const list <Literal *> * clause, SAT * _parent);

	void SetListPointer(list <Clause *>::const_iterator var);
public:
	~Clause();

	//Operations
	unsigned int getIdentifier() const;
	unsigned int Size() const;
	bool Contains(const Variable * variable) const;
	bool Contains(const Variable * variable, const bool isPositive) const;
	bool Contains(const Literal * lit) const;
	bool Evaluate(const list <int> * variables) const;
	bool isValid() const;

	//Operator Overloads
	bool operator==(const Clause & clause) const;
	bool operator!=(const Clause & clause) const;
	bool operator<(const Clause & clause) const;
	bool operator>(const Clause & clause) const;
	bool operator<=(const Clause & clause) const;
	bool operator>=(const Clause & clause) const;

	friend Literal;
	friend SAT;
	friend ClauseState;
	friend SATState;
};

#endif