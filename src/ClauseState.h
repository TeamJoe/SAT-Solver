class ClauseState;
class SplitState;

#ifndef CLAUSE_STATE_H
#define CLAUSE_STATE_H

#include "Clause.h"
#include "SATState.h"
#include "VariableState.h"

#include "Constants.h"

using namespace std;

class ClauseState
{
private:
	SATState * satState;
	const Clause * clause;
	VariableState** variables;

	bool Active;
	bool True;
	bool Duplicate;

	double * probabiltyPositive;
protected:
	ClauseState(SATState * sat, const Clause * clause);
	ClauseState * copy(SATState * sat);

	void init();
	bool verifyTrue() const;
#ifdef STATISTICS_STEPS
	void updateStatistics(int step);
#endif
public:
	~ClauseState();

	bool isActive() const;
	bool isTrue() const;
	bool isDuplicate(const ClauseState* clause);
	unsigned int getCurrentSize() const;

	const Clause * getClause() const;
	const list<int> & getCurrentState() const;

#ifdef STATISTICS_STEPS
	double getProbabiltyPositive(int step) const;
#endif

	friend SATState;
	friend SplitState;
};

#endif