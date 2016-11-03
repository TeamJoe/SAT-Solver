class Clause;

#ifndef CLAUSE_H
#define CLAUSE_H

#include <list>
#include "Literal.h"
#include "SAT.h"

using namespace std;

class Clause
{
private:
	Literal ** clause;
	unsigned int _size;

	SAT * _parent;
	bool isActive;
	list <Clause *>::const_iterator listPointer;
protected:
	Clause(SAT * _parent);
public:

	//Constructors
	Clause(const list <Literal *> * clause, SAT * _parent);
	~Clause();
	Clause * copy(const list<Variable *> * vars, SAT * _parent) const;

	//Operations
	unsigned int IntialSize() const;
	unsigned int Size() const;
	bool Contains(const Variable * variable) const;
	bool Contains(const Variable * variable, const bool isPositive) const;
	bool Contains(const Literal * lit) const;
	bool Evaluate(const list <int> * variables) const;
	bool isValid() const;

	bool IsActive() const;
	bool AllEvaluted() const; //true if all literals are evaluated
	bool AllTrue() const; //true if all literals are true
	bool AllFalse() const; //true if all literals are false
	bool HasEvaluted() const; //true if one literal is evaluated
	bool HasTrue() const; //true if one literal is true
	bool HasFalse() const; //true if one literal is false
	bool HasSolution() const;//Checks if solvable from the current state
	bool HasOppositeSolution() const;//Checks if solvable if all variables were reversed

	//Modifers
	void Undo();
	void Remove();
	void SetListPointer(list <Clause *>::const_iterator cla, bool isActive);

	//Operator Overloads
	bool operator==(const Clause & clause) const;
	bool operator!=(const Clause & clause) const;
	bool operator<(const Clause & clause) const;
	bool operator>(const Clause & clause) const;
	bool operator<=(const Clause & clause) const;
	bool operator>=(const Clause & clause) const;
};

#endif