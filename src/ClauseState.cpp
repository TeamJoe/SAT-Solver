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

	this->variables = new VariableState *[clause->_size];

#if _DEBUG
	for (unsigned int i = 0; i < this->clause->_size; i++)
	{
		this->variables[i] == NULL;
	}
#endif
}
ClauseState::~ClauseState()
{
	delete[] this->variables;
}
void ClauseState::init()
{
	for (unsigned int i = 0; i < this->clause->_size; i++)
	{
		this->variables[i] = this->satState->_getState(this->clause->clause[i]->getVariable());
	}
}
void ClauseState::update()
{
	assert(this->Active);
	assert(!this->True);
	double probabilityNegative = 1.0;
	for (unsigned int i = 0; i < this->clause->_size; i++)
	{
		if (this->variables[i]->isActive()) {
			probabilityNegative = probabilityNegative * (1.0 - this->variables[i]->getProbabiltyPositiveFirstStep());
		}
	}
	probabiltyPositiveFirstStep = 1.0 - probabilityNegative;
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
	if (*state == 0)
		assert(this->Active);
	else
		assert(this->Active != this->clause->Evaluate(state));
	delete [] state;
#endif
	return this->Active;
}
bool ClauseState::isTrue() const
{
#if _DEBUG
	const int * state = this->satState->getState();
	if (*state == 0)
		assert(this->Active);
	else
		assert(this->Active != this->clause->Evaluate(state));
	delete [] state;
#endif
	//assert(this->True == this->verifyTrue());
	return this->True;
}
unsigned int ClauseState::getCurrentSize() const
{
	unsigned int size = 0;
	for(unsigned int i = 0; i < this->clause->_size; i++)
	{
		if (this->variables[i]->isActive()) {
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
			if (this->variables[i]->isActive()) {
				state->push_back(lit->getValue());
			}
		}
	}
	return *state;
}

bool ClauseState::verifyTrue() const
{
	for (unsigned int i = 0; i < this->clause->_size; i++)
	{
		const Literal* lit = this->clause->clause[i];
		const VariableState* varState = this->variables[i];
		if (lit->getValue() == varState->getValue())
		{
			assert(!varState->isActive());
			return true;
		}
	}

	return false;
}