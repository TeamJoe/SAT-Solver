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

	Clause* getClause(const Clause* clause) const;
	Variable* getVariable(const int& variable) const;
	Variable* getVariable(const Variable* variable) const;

	void cleanVariables();
	void cleanClauses();
protected:
	Variable* getOrCreateVariable(const int& var);
	bool contains(const Clause* clause) const;
	bool contains(const int& variable) const;
	bool contains(const Variable* variable) const;
public:
	//Constructors
	SAT();
	~SAT();

	SAT* copy() const;

	bool addVariable(const int& variables);
	bool addClause(const list <int> * variables);

	//Checker Functions
	unsigned int getIdentifier() const;
	bool Evaluate(const int * variables) const;
	bool Evaluate(const list <int> * variables) const;

	//Solver Functions
	unsigned int ClauseCount() const;
	unsigned int VariableCount() const;

	bool operator==(const SAT& sat) const;
	bool operator!=(const SAT& sat) const;

	friend Variable;
	friend Clause;
	friend SATState;
};

#endif