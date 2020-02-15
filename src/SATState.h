class SATState;
class SplitState;

#ifndef SAT_STATE_H
#define SAT_STATE_H

#include <map>
#include <unordered_map>

#include "Variable.h"
#include "Clause.h"
#include "SAT.h"
#include "ClauseState.h"
#include "VariableState.h"
#include "SATSolverState.h"

#include "Constants.h"

using namespace std;

class SATState
{
private:
	const SAT * sat;

	map <unsigned int, VariableState *> * variables;
	map <unsigned int, ClauseState *> * clauses;

	unordered_map <unsigned int, VariableState*>* activeVariables;
	unordered_map <unsigned int, ClauseState*>* activeClauses;
	unsigned int activeVariableCount;
	unsigned int activeClauseCount;

	unsigned int variableAttempts;

	void deactivateClause(ClauseState* clause, unsigned int oldClauseCount);
	void updateClause(ClauseState* clause, unsigned int oldClauseCount, unsigned int newClauseCount);
	void reactivateClause(ClauseState* clause, unsigned int newClauseCount);

#ifdef _DEBUG
	void verify() const;
#endif

protected:
	ClauseState * _getState(const Clause * clause);
	VariableState * _getState(const Variable * variable);
public:
	SATState(const SAT * v);
	SATState(const SATState * oldState);
	~SATState();

	bool isActive(const Clause * clause) const;
	bool isActive(const Variable * variable) const;

	const SAT * getSAT() const;
	const ClauseState * getState(const Clause * clause) const;
	const VariableState * getState(const Variable * variable) const;

	void setVariable(const Variable * variable, const bool state);
	void unsetVariable(const Variable * variable);

	const map <unsigned int, const VariableState *> * getVariableMap() const;
	const map <unsigned int, const ClauseState *> * getClauseMap() const;

	unsigned int getRemainingClauseCount() const;
	unsigned int getRemainingVariableCount() const;
	bool hasSolution() const;
	bool canSolve() const;
	const int * getState() const; //return variable needs to be cleaned up
	const list<const list <int> *> & getRemainingClauses() const; //return variable needs to be cleaned up
	unsigned int getVariableAttempts() const;

	friend VariableState;
	friend ClauseState;
	friend SplitState;
};

#endif