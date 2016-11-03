#include "SATSolverState.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"


SATSolverState::SATSolverState(const SAT * sat)
{
	this->state = new SATState(sat);
	this->currentVariables = new map <unsigned int, const VariableState *>();

	const map <unsigned int, const VariableState *> * variables = this->state->getVariableMap();
	map <unsigned int, const VariableState *>::const_iterator variablesEnd = variables->cend();
	for (map <unsigned int, const VariableState *>::const_iterator iter = variables->cbegin(); iter != variablesEnd; iter++)
	{
		(*(this->currentVariables))[iter->first] = iter->second;
	}
}
SATSolverState::SATSolverState(const SATSolverState * oldState)
{
	this->state = new SATState(oldState->state);
	this->currentVariables = new map <unsigned int, const VariableState *>();

	const map <unsigned int, const VariableState *> * variables = this->state->getVariableMap();
	map <unsigned int, const VariableState *>::const_iterator variablesEnd = variables->cend();
	for (map <unsigned int, const VariableState *>::const_iterator iter = variables->cbegin(); iter != variablesEnd; iter++)
	{
		if (iter->second->isActive())
		{
			(*(this->currentVariables))[iter->first] = iter->second;
		}
	}
}
SATSolverState::~SATSolverState()
{
	assert(this->state != NULL);
	delete state;
	this->state = NULL;

	assert(this->currentVariables != NULL);
	delete currentVariables;
	this->currentVariables = NULL;
}

const SATState * SATSolverState::getState() const
{
	return this->state;
}
const map <unsigned int, const VariableState *> * SATSolverState::getCurrentVariables() const
{
	return (map <unsigned int, const VariableState *> *)this->currentVariables;
}

void SATSolverState::setVariable(const Variable * variable, const bool state)
{
	this->state->setVariable(variable, state);
	this->currentVariables->erase(variable->getIdentifier());
	assert(this->currentVariables->find(variable->getIdentifier()) == this->currentVariables->cend());
	assert(this->currentVariables->size() == this->state->getRemainingVariableCount());
}
void SATSolverState::unsetVariable(const Variable * variable)
{
	this->state->unsetVariable(variable);
	(*this->currentVariables)[variable->getIdentifier()] = this->state->getState(variable);
	assert(this->currentVariables->find(variable->getIdentifier()) != this->currentVariables->cend());
	assert(this->currentVariables->size() == this->state->getRemainingVariableCount());
}