class Literal;

#ifndef LITERAL_H
#define LITERAL_H

#include <list>

#include "Variable.h"
#include "Clause.h"

#include "Constants.h"

using namespace std;

class Literal
{
private:
	int Value;
	Variable * variable;
	Clause * clause;
	list <Literal *>::const_iterator listPointer;
#ifdef _DEBUG
	bool isDestroying;
#endif
protected:
	void Add(Clause * clause);
	void SetListPointer(list <Literal *>::const_iterator lit);
public:

	//Constructors
	Literal(Variable * variable, Clause * clause, const bool isPositive);
	~Literal();

	//Comparors, compares variable and value
	bool Contains(const Variable * variable) const;
	bool Contains(const Variable * variable, const bool isPositive) const;
	bool Opposite(const Variable * variable, const bool isPositive) const;
	bool Contains(const Literal * lit) const;
	bool Opposite(const Literal * lit) const;

	//Operators
	unsigned int getIdentifier() const;
	int getValue() const;
	bool GetType() const;
	const Clause * getClause() const;
	const Variable * getVariable() const;
	unsigned int ClauseSize() const;


	//Operator overload (only comparors variable, not value)
	bool operator==(const Literal & lit) const;
	bool operator!=(const Literal & lit) const;
	bool operator<(const Literal & lit) const;
	bool operator>(const Literal & lit) const;
	bool operator<=(const Literal & lit) const;
	bool operator>=(const Literal & lit) const;

	friend Variable;
	friend Clause;
};

#endif