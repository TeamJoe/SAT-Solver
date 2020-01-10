class Variable;
class CompositeVariable;

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
	map <int, map<unsigned int, Clause*>*>* positiveSiblingCount;
	map <int, map<unsigned int, Clause*>*>* negativeSiblingCount;
	map <unsigned int, Clause *> * clauses;
	list <Variable *>::const_iterator listPointer;

	void addSiblings(Literal* lit, map <int, map<unsigned int, Clause*>*>* sibilings);
	void removeSiblings(Literal* lit, map <int, map<unsigned int, Clause*>*>* sibilings);
protected: 
	Variable(const int Variable_Number, SAT * _parent);

	void Add(Literal * lit);
	void Remove(list <Literal *>::const_iterator& litIter);

	Variable* copy(SAT* _parent) const;
public:
	~Variable();

	//Operations
	unsigned int getIdentifier() const;
	int GetVariable() const;
	bool isValid() const;

	//Operator Overloads
	bool operator==(const Variable & variable) const;
	bool operator!=(const Variable & variable) const;
	bool operator<(const Variable & variable) const;
	bool operator>(const Variable & variable) const;
	bool operator<=(const Variable & variable) const;
	bool operator>=(const Variable & variable) const;
	bool operator==(const int& variable) const;
	bool operator!=(const int& variable) const;
	bool operator<(const int& variable) const;
	bool operator>(const int& variable) const;
	bool operator<=(const int& variable) const;
	bool operator>=(const int& variable) const;

	friend Literal;
	friend SAT;
	friend VariableState;
	friend SATState;
};

class CompositeVariable : protected Variable
{
private:
	Variable* v1;
	Variable* v2;
	bool isOpposite;
protected:
	CompositeVariable(Variable* v1, Variable* v2, bool isOpposite, SAT* _parent);

	CompositeVariable* copy(SAT* _parent) const;
public:
	friend Literal;
	friend SAT;
	friend VariableState;
	friend SATState;
};

#endif