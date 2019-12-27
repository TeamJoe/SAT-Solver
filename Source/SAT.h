class SAT;

#ifndef SAT_H
#define SAT_H

#include <list>
#include <string>

#include "Variable.h"
#include "Clause.h"

using namespace std;

struct SortFunction
{
	bool (*Eqaulity)(const Variable *, const Variable *);
	Variable * (*Sort)(Variable *, Variable *);
};

class SAT
{
private:
	list <Variable *> previousVariables;
	list <Variable *> variables;
	list <Clause *> clauses;
	list <Clause *> previousClauses;

	//Private Functions
	Clause * ContainsClause(const Clause * clause) const;
	Variable * ContainsVariable(const Variable * variable) const;
	int CharToInt(const char c) const;

	//Read Input Functions
	bool ReadFrom(ifstream & file);
	bool ReadCNF(ifstream & file);

	void StepBackVariables();
	void UpdateVariables();
	void SetVariables();
protected:
	SAT();
public:

	//Constructors
	SAT(ifstream & file, const bool isCNF);
	SAT(const char * input, const bool isCNF);
	~SAT();
	SAT * copy() const;
	void Revert();
	void cleanVariables();
	void cleanClauses();

	//Checker Functions
	bool isValid() const;
	bool Evaluate(const list <int> * variables) const;
	bool AllPreviousClausesHaveFalse() const;
	bool AllPreviousClausesHaveTrue() const;
	bool AllClausesHaveFalse() const;
	bool AllClausesHaveTrue() const;
	bool HasSolution() const;//Checks if solvable from the current state
	bool HasOppositeSolution() const;//Checks if solvable if all variables were reversed

	//Solver Functions
	Variable * NextVariable(const list <SortFunction *> * SortFunctions) const;
	Variable * NextVariable(const Variable * Excluded_Variable, const list <SortFunction *> * SortFunctions) const;
	Variable * Sort(Variable * var1, Variable * var2, const list <SortFunction *> * SortFunctions) const;
	void FlipAll();
	void FlipVariable(Variable * var);
	void UndoVariable(Variable * var);
	void RemoveVariable(Variable * var, const bool isPositive);
	unsigned int ClauseCount() const;
	unsigned int VariableCount() const;

	void Remove(list <Clause *>::const_iterator listPointer, bool makeActive);
	void Remove(list <Variable *>::const_iterator listPointer, bool makeActive);

	list <int> * getResults() const;
};

#endif