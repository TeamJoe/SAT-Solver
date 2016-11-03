class ClauseState;

#ifndef CLAUSE_STATE_H
#define CLAUSE_STATE_H

#include "Clause.h"
#include "SATState.h"

#include "Constants.h"

using namespace std;

class ClauseState
{
private:
	SATState * satState;
	const Clause * clause;

	bool Active;
	bool True;
protected:
	ClauseState(SATState * sat, const Clause * clause);
	ClauseState * copy(SATState * sat);

	void reset();
public:
	~ClauseState();

	bool isActive() const;
	bool isTrue() const;
	unsigned int getCurrentSize() const;

	const Clause * getClause() const;
	const list<int> & getCurrentState() const;

	friend SATState;
};

#endif