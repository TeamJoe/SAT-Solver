class SAT;

#ifndef SAT_H
#define SAT_H

#include <fstream>
#include <list>

#include "Variable.h"
#include "Clause.h"
#include "SATState.h"

#include "Constants.h"

using namespace std;

class SAT
{
private:
	list <Variable *> * variables;
	list <Clause *> * clauses;

	Clause* ContainsClause(const Clause* clause) const;
	Variable* ContainsVariable(const int& variable) const;
	Variable* ContainsVariable(const Variable* variable) const;

	void cleanVariables();
	void cleanClauses();
protected:
	Variable* getOrCreateVariable(const int& var);
public:
	//Constructors
	SAT();
	~SAT();

	bool addVariable(Variable* variable);
	bool addClause(const list <int> * variables);

	//Checker Functions
	unsigned int getIdentifier() const;
	bool Evaluate(const int * variables) const;
	bool Evaluate(const list <int> * variables) const;

	//Solver Functions
	unsigned int ClauseCount() const;
	unsigned int VariableCount() const;

	friend Clause;
	friend SATState;
};

#endif