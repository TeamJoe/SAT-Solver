class Variable;

#ifndef VARIABLE_H
#define VARIABLE_H

#include "VariableState.h"
#include "Literal.h"
#include "SAT.h"

using namespace std;

//#define KEEP_VARIABLE_HISTORY

class Variable
{
private:
	int Variable_Number;
	bool WentPositive;
	bool isEvaluated;
	list <Literal *> Negatives;
	list <Literal *> Positives;
	list <Literal *> InactiveNegatives;
	list <Literal *> InactivePositives;
#ifdef KEEP_VARIABLE_HISTORY
	list <VariableState *> PreviousStates;
#endif
	VariableState * CurrentState;

	SAT * _parent;
	bool isActive;
	list <Variable *>::const_iterator listPointer;
protected:
	bool _Undo();
	void _Remove(bool isPositive);
public:

	//Constructors
	Variable(const int Variable_Number, SAT * _parent);
	~Variable();
	Variable * copy(SAT * _parent) const;

	//Modifiers
	void Revert();
#ifdef KEEP_VARIABLE_HISTORY
	void StepBack();
#endif
	bool Flip();
	bool Undo();
	void Add(Literal * lit);
	void Remove(list <Literal *>::const_iterator litIter);
	void Add(list <Literal *>::const_iterator lit);
	void Subtract(list <Literal *>::const_iterator lit);
	void Remove(bool isPositive);
#ifdef KEEP_VARIABLE_HISTORY
	void Set();
#endif
	void Update();
	void SetListPointer(list <Variable *>::const_iterator var, bool isActive);

	//Operations
	bool HasSolution() const;//Checks if solvable from the current state
	bool HasOppositeSolution() const;//Checks if solvable if all variables were reversed
	bool IsEvaluated() const;
	int GetVariable() const;
	bool IsActive() const;
	int GetValue() const;
	VariableState * getCurrentState() const;

	//Operator Overloads
	bool operator==(const Variable & variable) const;
	bool operator!=(const Variable & variable) const;
	bool operator<(const Variable & variable) const;
	bool operator>(const Variable & variable) const;
	bool operator<=(const Variable & variable) const;
	bool operator>=(const Variable & variable) const;

	friend VariableState;
};

#endif