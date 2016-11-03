class SATSolverState;

#ifndef SAT_SOLVER_STATE_H
#define SAT_SOLVER_STATE_H

#include "SAT.h"
#include "VariableState.h"
#include "SATState.h"

#include "Constants.h"

using namespace std;

class SATSolverState
{
private:
	SATState * state;
	map <unsigned int, const VariableState *> * currentVariables;
protected:
public:

	//Constructors
	SATSolverState(const SAT * sat);
	SATSolverState(const SATSolverState * oldState);
	~SATSolverState();

	const SATState * getState() const;
	const map <unsigned int, const VariableState *> * getCurrentVariables() const;

	void setVariable(const Variable * variable, const bool state);
	void unsetVariable(const Variable * variable);

	const list<const list <int> *> * getSolutions() const;
};
#endif