class Variable;

#ifndef VARIABLE_H
#define VARIABLE_H

#include <map>

#include "Literal.h"
#include "SAT.h"
#include "VariableState.h"
#include "SATState.h"

#include "Constants.h"

using namespace std;

class Variable
{
private:
	int Variable_Number;
	list <Literal *> * Negatives;
	list <Literal *> * Positives;

	SAT * _parent;
	map <unsigned int, Clause *> * clauses;
	list <Variable *>::const_iterator listPointer;
protected: 
	Variable(const int Variable_Number, SAT * _parent);

	void Add(Literal * lit);
	void Remove(list <Literal *>::const_iterator litIter);
	void SetListPointer(list <Variable *>::const_iterator var);
public:
	~Variable();

	//Operations
	unsigned int getIdentifier() const;
	int GetVariable() const;

	//Operator Overloads
	bool operator==(const Variable & variable) const;
	bool operator!=(const Variable & variable) const;
	bool operator<(const Variable & variable) const;
	bool operator>(const Variable & variable) const;
	bool operator<=(const Variable & variable) const;
	bool operator>=(const Variable & variable) const;

	friend Literal;
	friend SAT;
	friend VariableState;
	friend SATState;
};

#endif