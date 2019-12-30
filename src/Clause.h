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
#ifdef _DEBUG
	list<int> _value;
#endif
	int* value;
	Literal ** clause;
	unsigned int _size;

	SAT * _parent;
	list <Clause *>::const_iterator listPointer;

	void _createValue(const list <int>* clause);
	Literal* createLiteral(const int& var);
protected:
	Clause(const list <int> * clause, SAT * _parent);

	Clause* copy(SAT* _parent) const;
public:
	~Clause();

	//Operations
	unsigned int getIdentifier() const;
	unsigned int Size() const;
	bool Contains(const Variable * variable) const;
	bool Contains(const Variable * variable, const bool isPositive) const;
	bool Contains(const Literal * lit) const;
	bool Contains(const int variables) const;
	bool Evaluate(const int * variables) const;
	bool Evaluate(const list <int> * variables) const;
	bool isValid() const;

	//Operator Overloads
	bool operator==(const Clause & clause) const;
	bool operator!=(const Clause & clause) const;
	bool operator<(const Clause & clause) const;
	bool operator>(const Clause & clause) const;
	bool operator<=(const Clause & clause) const;
	bool operator>=(const Clause & clause) const;

	friend Variable;
	friend Literal;
	friend SAT;
	friend ClauseState;
	friend SATState;
};

#endif