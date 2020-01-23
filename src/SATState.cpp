#include "VariableState.h"
#include "Variable.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

//-------------------------------
//
// Constructor
//
//------------------------------
SATState::SATState(const SAT * sat)
{
	this->sat = sat;
	this->variables = new map <unsigned int, VariableState *>();
	this->clauses = new map <unsigned int, ClauseState *>();
	this->variableAttempts = 0;
	this->activeClauseCount = this->sat->clauses->size();
	this->activeVariableCount = this->sat->variables->size();

	for(list <Clause *>::const_iterator iter = this->sat->clauses->cbegin(); iter != this->sat->clauses->cend(); iter++)
	{
		ClauseState * newClauseState = new ClauseState(this, *iter);
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) == this->clauses->cend());
		(*(this->clauses))[newClauseState->getClause()->getIdentifier()] = newClauseState;
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) != this->clauses->cend());
	}
	for(list <Variable *>::const_iterator iter = this->sat->variables->cbegin(); iter != this->sat->variables->cend(); iter++)
	{
		VariableState * newVariableState = new VariableState(this, *iter);
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) == this->variables->cend());
		(*(this->variables))[newVariableState->getVariable()->getIdentifier()] = newVariableState;
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) != this->variables->cend());
	}

	// TODO: Speed up by having this get set by VariableState
	for (map <unsigned int, ClauseState*>::const_iterator iter = this->clauses->begin(); iter != this->clauses->end(); iter++)
	{
		iter->second->update();
	}

	// TODO: Basic solving (resolving single size clauses, and variables without opposites
}

SATState::SATState(const SATState * oldState)
{
	this->sat = oldState->sat;
	this->variables = new map <unsigned int, VariableState *>();
	this->clauses = new map <unsigned int, ClauseState *>();
	this->variableAttempts = oldState->variableAttempts;
	this->activeClauseCount = oldState->activeClauseCount;
	this->activeVariableCount = oldState->activeVariableCount;

	assert(oldState->clauses != NULL);
	for(map <unsigned int, ClauseState *>::const_iterator iter = oldState->clauses->cbegin(); iter != oldState->clauses->cend(); iter++)
	{
		ClauseState * newClauseState = iter->second->copy(this);
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) == this->clauses->cend());
		(*(this->clauses))[newClauseState->getClause()->getIdentifier()] = newClauseState;
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) != this->clauses->cend());
	}

	assert(oldState->variables != NULL);
	for(map <unsigned int, VariableState *>::const_iterator iter = oldState->variables->cbegin(); iter != oldState->variables->cend(); iter++)
	{
		VariableState * newVariableState = new VariableState(this, iter->second);
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) == this->variables->cend());
		(*(this->variables))[newVariableState->getVariable()->getIdentifier()] = newVariableState;
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) != this->variables->cend());
	}
	for (map <unsigned int, ClauseState*>::const_iterator iter = this->clauses->begin(); iter != this->clauses->end(); iter++)
	{
		iter->second->update();
	}
}

SATState::~SATState() {
	assert(this->variables != NULL);
	for(map <unsigned int, VariableState *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		delete iter->second;
	}
	this->variables->clear();
	delete this->variables;
	this->variables = NULL;

	assert(this->clauses != NULL);
	for(map <unsigned int, ClauseState *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		delete iter->second;
	}
	this->clauses->clear();
	delete this->clauses;
	this->clauses = NULL;
}

bool SATState::isActive(const Clause * clause) const
{
	assert(this->clauses->find(clause->getIdentifier()) != this->clauses->cend());
	return this->clauses->find(clause->getIdentifier())->second->isActive();
}
bool SATState::isActive(const Variable * variable) const
{
	assert(this->variables->find(variable->getIdentifier()) != this->variables->cend());
	return this->variables->find(variable->getIdentifier())->second->isActive();
}

const SAT * SATState::getSAT() const
{
	return this->sat;
}
ClauseState * SATState::_getState(const Clause * clause)
{
	assert(this->clauses->find(clause->getIdentifier()) != this->clauses->cend());
	return this->clauses->find(clause->getIdentifier())->second;
}
VariableState * SATState::_getState(const Variable * variable)
{
	assert(this->variables->find(variable->getIdentifier()) != this->variables->cend());
	return this->variables->find(variable->getIdentifier())->second;
}

const ClauseState * SATState::getState(const Clause * clause) const
{
	assert(this->clauses->find(clause->getIdentifier()) != this->clauses->cend());
	return this->clauses->find(clause->getIdentifier())->second;
}
const VariableState * SATState::getState(const Variable * variable) const
{
	assert(this->variables->find(variable->getIdentifier()) != this->variables->cend());
	return this->variables->find(variable->getIdentifier())->second;
}

void SATState::setVariable(const Variable * variable, const bool state)
{
	assert(this->variables->find(variable->getIdentifier()) != this->variables->cend());
	VariableState * variableState = this->_getState(variable);
	if (variableState->isActive())
	{
		variableState->Active = false;
		variableState->True = state;
		this->activeVariableCount--;

		map <unsigned int, ClauseState *> * clauses = variableState->ActiveClauses;
		map <unsigned int, ClauseState *>::const_iterator end = clauses->cend();
		for(map <unsigned int, ClauseState *>::const_iterator iter = clauses->cbegin(); iter != end;)
		{
			ClauseState * clauseState = iter->second;
			iter++;
			assert(clauseState->Active);
			const Clause * clause = clauseState->getClause();
			if(clause->Contains(variable, state))
			{
				assert(clauseState->getCurrentSize() < clause->Size());
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
				this->deactivateClause(clauseState);
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
			}
			else
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
				this->updateClause(clauseState, clauseState->getCurrentSize() + 1);
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
		}
	} 
	else if (variableState->True != state)
	{
		assert(!variableState->isActive());
		variableState->True = state;

		map <unsigned int, Clause *> * clauses = variableState->variable->clauses;
		map <unsigned int, Clause *>::const_iterator end = clauses->cend();
		for(map <unsigned int, Clause *>::const_iterator iter = clauses->cbegin(); iter != end;)
		{
			const Clause * clause = iter->second;
			ClauseState* clauseState = this->_getState(clause);
			iter++;
			assert(clause->Contains(variable));
			if (clause->Contains(variable, state))
			{
				if (clauseState->Active)
				{
					assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
					assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
					this->deactivateClause(clauseState);
				}
				assert(!clauseState->Active);
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
			}
			else
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
				this->reactivateClause(clauseState);
				assert(!clauseState->Active || variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(!clauseState->Active || variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
		}
	}

	this->variableAttempts++;
	assert(!variableState->isActive());
	assert(variableState->True == state);
}
void SATState::unsetVariable(const Variable * variable)
{
	assert(this->variables->find(variable->getIdentifier()) != this->variables->cend());
	VariableState * variableState = this->_getState(variable);
	if (!variableState->isActive())
	{
		variableState->Active = true;
		this->activeVariableCount++;

		map <unsigned int, Clause *> * clauses = variableState->variable->clauses;
		map <unsigned int, Clause *>::const_iterator end = clauses->cend();
		for(map <unsigned int, Clause *>::const_iterator iter = clauses->cbegin(); iter != end;)
		{
			const Clause * clause = iter->second;
			ClauseState* clauseState = this->_getState(clause);
			iter++;
			assert(clause->Contains(variable));
			if (clause->Contains(variable, variableState->True)) 
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
				this->reactivateClause(clauseState);
				assert(!clauseState->Active || variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(!clauseState->Active || variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
			else if (clauseState->Active)
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
				this->updateClause(clauseState, clauseState->getCurrentSize() - 1);
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
		}
	}
	assert(variableState->isActive());
}

void SATState::deactivateClause(ClauseState* clause)
{
	assert(clause->Active);
	assert(clause == this->_getState(clause->clause));
	unsigned int clauseCount = clause->getCurrentSize();
	assert(clauseCount <= clause->clause->Size());
	clause->Active = false;
	clause->True = true;
	this->activeClauseCount--;
	for (unsigned int i = 0; i < clause->clause->_size; i++)
	{
		VariableState* varState = clause->variables[i];
		assert(varState == this->_getState(varState->getVariable()));
		varState->removeClause(clause, clauseCount);
	}
	assert(!clause->Active);
}

void SATState::updateClause(ClauseState* clause, unsigned int oldClauseCount)
{
	assert(clause->Active);
	assert(clause == this->_getState(clause->clause));
	unsigned int clauseCount = clause->getCurrentSize();
	assert(clauseCount != 0);
	assert(oldClauseCount != clauseCount);
	for (unsigned int i = 0; i < clause->clause->_size; i++)
	{
		VariableState* varState = clause->variables[i];
		assert(varState == this->_getState(varState->getVariable()));
		assert(varState->ActiveClauses->find(clause->clause->getIdentifier()) != varState->ActiveClauses->cend());
		if (clause->clause->Contains(varState->getVariable(), true))
		{
			assert(clauseCount < varState->PositiveClauseSizes->size());
			assert((*(varState->PositiveClauseSizes))[oldClauseCount] != 0);
			(*(varState->PositiveClauseSizes))[clauseCount]++;
			(*(varState->PositiveClauseSizes))[oldClauseCount]--;
		}
		else
		{
			assert(clauseCount < varState->NegativeClauseSizes->size());
			assert((*(varState->NegativeClauseSizes))[oldClauseCount] != 0);
			(*(varState->NegativeClauseSizes))[clauseCount]++;
			(*(varState->NegativeClauseSizes))[oldClauseCount]--;
		}
	}
	assert(clause->Active);
}

void SATState::reactivateClause(ClauseState* clause)
{
	assert(!clause->Active);
	assert(clause == this->_getState(clause->clause));
	if (!clause->verifyTrue()) {
		unsigned int clauseCount = clause->getCurrentSize();
		assert(clauseCount <= clause->clause->_size);
		clause->Active = true;
		clause->True = false;
		this->activeClauseCount++;
		for (unsigned int i = 0; i < clause->clause->_size; i++)
		{
			VariableState* varState = clause->variables[i];
			assert(varState == this->_getState(varState->getVariable()));
			varState->addClause(clause, clauseCount);
		}
		assert(clause->Active);
	}
}

const map <unsigned int, const VariableState *> * SATState::getVariableMap() const
{
	return (map <unsigned int, const VariableState *> *)this->variables;
}
const map <unsigned int, const ClauseState *> * SATState::getClauseMap() const
{
	return (map <unsigned int, const ClauseState *> *)this->clauses;
}

unsigned int SATState::getRemainingClauseCount() const
{
#ifdef _DEBUG
	int actualCount = 0;
	for(map <unsigned int, ClauseState *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		if (iter->second->isActive())
		{
			actualCount++;
		}
	}
	assert(actualCount == this->activeClauseCount);
#endif
	return this->activeClauseCount;
}
unsigned int SATState::getRemainingVariableCount() const
{
#ifdef _DEBUG
	int actualCount = 0;
	for(map <unsigned int, VariableState *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if (iter->second->isActive())
		{
			actualCount++;
		}
	}
	assert(actualCount == this->activeVariableCount);
#endif
	return this->activeVariableCount;
}

bool SATState::hasSolution() const
{
	return this->getRemainingClauseCount() == 0;
}
bool SATState::canSolve() const
{
	assert(this->variables != NULL);
	for(map <unsigned int, VariableState *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if (!iter->second->hasSolution())
		{
			return false;
		}
	}

	//Loop not current required as the clauses do nothing on reset
	assert(this->clauses != NULL);
	/*for(map <unsigned int, ClauseState *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		if (!iter->second->hasSolution())
		{
			return false;
		}
	}*/

	return true;
}
const int * SATState::getState() const
{
	int count = this->variables->size() - this->activeVariableCount;
	if (count <= 0)
	{
		int * currentState = new int[2];
		currentState[0] = 0;
		currentState[1] = NULL;
		return currentState;
	}
	else
	{
		int * currentState = new int[count + 2];
		int i = 1;
		currentState[0] = count;
		for(map <unsigned int, VariableState *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
		{
			if (!iter->second->isActive())
			{
				assert(i < (count + 1));
				assert(iter->second->getValue() != 0);
				currentState[i++] = iter->second->getValue();
			}
		}
		assert(i == (count + 1));
		currentState[count + 1] = NULL;

		return currentState;
	}
}
const list<const list <int> *> & SATState::getRemainingClauses() const
{
	list<const list <int> *> * currentState = new list<const list <int> *> ();
	for(map <unsigned int, ClauseState *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		if (iter->second->isActive())
		{
			currentState->push_back(&iter->second->getCurrentState());
		}
	}

	return *currentState;
}

unsigned int SATState::getVariableAttempts() const
{
	return this->variableAttempts;
}

