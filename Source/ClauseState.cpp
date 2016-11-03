#include "ClauseState.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"


ClauseState::ClauseState(SATState * sat, const Clause * clause)
{
	this->satState = sat;
	this->clause = clause;
	this->Active = true;
	this->True = false;
}
ClauseState::~ClauseState()
{
}
void ClauseState::reset()
{
}
ClauseState * ClauseState::copy(SATState * sat)
{
	ClauseState * newClause = new ClauseState(sat, this->clause);
	newClause->Active = this->Active;
	newClause->True = this->True;
	return newClause;
}

bool ClauseState::isActive() const
{
#if _DEBUG
	const int * state = this->satState->getState();
	assert(this->Active != this->clause->Evaluate(state));
	delete [] state;
#endif
	return this->Active;
}
bool ClauseState::isTrue() const
{
#if _DEBUG
	const int * state = this->satState->getState();
	assert(this->Active == this->clause->Evaluate(state));
	delete [] state;
#endif
	return this->True;
}
unsigned int ClauseState::getCurrentSize() const
{
	unsigned int size = 0;
	for(unsigned int i = 0; i < this->clause->_size; i++)
	{
		if (this->satState->isActive(this->clause->clause[i]->getVariable())) {
			size++;
		}
	}
	return size;
}
const Clause * ClauseState::getClause() const
{
	return this->clause;
}
const list<int> & ClauseState::getCurrentState() const
{
	list<int> * state = new list<int>();
	if (this->Active)
	{
		for(unsigned int i = 0; i < this->clause->_size; i++)
		{
			const Literal * lit = this->clause->clause[i];
			if (this->satState->isActive(lit->getVariable())) {
				state->push_back(lit->getValue());
			}
		}
	}
	return *state;
}