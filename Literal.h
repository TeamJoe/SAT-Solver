class Literal;

#ifndef LITERAL_H
#define LITERAL_H

#include "VariableState.h"
#include "Variable.h"
#include "Clause.h"

class Literal
{
private:
	int Value;
	bool isActive;
	Variable * variable;
	Clause * clause;
	list <Literal *>::const_iterator listPointer;
#ifdef _DEBUG
	bool isDestroying;
#endif
public:

	//Constructors
	Literal(Variable * variable, Clause * clause, const bool isPositive);
	~Literal();
	Literal * copy(const list<Variable *> * vars, Clause * clause) const;

	//Comparors, compares variable and value
	bool Contains(const Variable * variable) const;
	bool Contains(const Variable * variable, const bool isPositive) const;
	bool Opposite(const Variable * variable, const bool isPositive) const;
	bool Contains(const Literal * lit) const;
	bool Opposite(const Literal * lit) const;

	//Operators
	bool GetType() const;
	bool IsActive() const;
	unsigned int IntialClauseSize() const;
	unsigned int ClauseSize() const;

	bool IsEvaluted() const;
	bool IsTrue() const;
	bool IsFalse() const;

	//Modifiers
	void Undo();//Used for undoing a variable
	void ReAddToVariable();
	void RemoveFromClause();//Removes the literal, its joining clauses, and all literal associated with that clause
	void RemoveFromVariable();//Removes a literal for a variable list
	void SetListPointer(list <Literal *>::const_iterator lit, bool isActive);

	//Operator overload (only comparors variable, not value)
	bool operator==(const Literal & lit) const;
	bool operator!=(const Literal & lit) const;
	bool operator<(const Literal & lit) const;
	bool operator>(const Literal & lit) const;
	bool operator<=(const Literal & lit) const;
	bool operator>=(const Literal & lit) const;

	friend VariableState;
	friend Variable;
	friend Clause;
};

#endif