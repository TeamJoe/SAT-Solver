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
	this->activeClauses = new unordered_map <unsigned int, ClauseState*>(this->sat->ClauseCount() * 2);
	this->activeVariables = new unordered_map <unsigned int, VariableState*>(this->sat->VariableCount() * 2);

	for(list <Clause *>::const_iterator iter = this->sat->clauses->cbegin(); iter != this->sat->clauses->cend(); iter++)
	{
		ClauseState * newClauseState = new ClauseState(this, *iter);
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) == this->clauses->cend());
		(*(this->clauses))[newClauseState->getClause()->getIdentifier()] = newClauseState;
		(*(this->activeClauses))[newClauseState->getClause()->getIdentifier()] = newClauseState;
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) != this->clauses->cend());
	}
	for(list <Variable *>::const_iterator iter = this->sat->variables->cbegin(); iter != this->sat->variables->cend(); iter++)
	{
		VariableState * newVariableState = new VariableState(this, *iter);
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) == this->variables->cend());
		(*(this->variables))[newVariableState->getVariable()->getIdentifier()] = newVariableState;
		(*(this->activeVariables))[newVariableState->getVariable()->getIdentifier()] = newVariableState;
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) != this->variables->cend());
	}

	// TODO: Speed up by having this get set by VariableState
	for (map <unsigned int, ClauseState*>::const_iterator iter = this->clauses->begin(); iter != this->clauses->end(); iter++)
	{
		iter->second->init();
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
	this->activeClauses = new unordered_map <unsigned int, ClauseState*>(this->sat->ClauseCount() * 2);
	this->activeVariables = new unordered_map <unsigned int, VariableState*>(this->sat->VariableCount() * 2);

	assert(oldState->clauses != NULL);
	for(map <unsigned int, ClauseState *>::const_iterator iter = oldState->clauses->cbegin(); iter != oldState->clauses->cend(); iter++)
	{
		ClauseState * newClauseState = iter->second->copy(this);
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) == this->clauses->cend());
		(*(this->clauses))[newClauseState->getClause()->getIdentifier()] = newClauseState;
		if (newClauseState->Active)
		{
			(*(this->activeClauses))[newClauseState->getClause()->getIdentifier()] = newClauseState;
		}
		assert(this->clauses->find(newClauseState->getClause()->getIdentifier()) != this->clauses->cend());
	}

	assert(oldState->variables != NULL);
	for(map <unsigned int, VariableState *>::const_iterator iter = oldState->variables->cbegin(); iter != oldState->variables->cend(); iter++)
	{
		VariableState * newVariableState = new VariableState(this, iter->second);
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) == this->variables->cend());
		(*(this->variables))[newVariableState->getVariable()->getIdentifier()] = newVariableState;
		if (newVariableState->Active)
		{
			(*(this->activeVariables))[newVariableState->getVariable()->getIdentifier()] = newVariableState;
		}
		assert(this->variables->find(newVariableState->getVariable()->getIdentifier()) != this->variables->cend());
	}

	// TODO: Speed up by having this get set by VariableState
	for (map <unsigned int, ClauseState*>::const_iterator iter = this->clauses->begin(); iter != this->clauses->end(); iter++)
	{
		iter->second->init();
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


	assert(this->activeClauses != NULL);
	delete this->activeClauses;
	this->activeClauses = NULL;

	assert(this->activeVariables != NULL);
	delete this->activeVariables;
	this->activeVariables = NULL;
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

void SATState::setVariable(const Variable* variable, const bool state)
{
	assert(this->variables->find(variable->getIdentifier()) != this->variables->cend());
	VariableState* variableState = this->_getState(variable);
	if (variableState->isActive())
	{
		variableState->Active = false;
		variableState->True = state;
		this->activeVariableCount--;
		assert(this->activeVariables->find(variableState->getVariable()->getIdentifier()) != this->activeVariables->cend());
		activeVariables->erase(variableState->getVariable()->getIdentifier());

		map <unsigned int, ClauseState*>* clauses = variableState->ActiveClauses;
		map <unsigned int, ClauseState*>::const_iterator end = clauses->cend();
		for (map <unsigned int, ClauseState*>::const_iterator iter = clauses->cbegin(); iter != end;)
		{
			ClauseState* clauseState = iter->second;
			iter++;
			assert(clauseState->Active);
			const Clause* clause = clauseState->getClause();
			if (clause->Contains(variable, state))
			{
				assert(clauseState->getCurrentSize() < clause->Size());
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
				this->deactivateClause(clauseState, clauseState->getCurrentSize() + 1);
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
			}
			else
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
				this->updateClause(clauseState, clauseState->getCurrentSize() + 1, clauseState->getCurrentSize());
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
		}
	}
	else if (variableState->True != state)
	{
		assert(!variableState->isActive());
		assert(this->activeVariables->find(variableState->getVariable()->getIdentifier()) == this->activeVariables->cend());
		variableState->True = state;

		map <unsigned int, Clause*>* clauses = variableState->variable->clauses;
		map <unsigned int, Clause*>::const_iterator end = clauses->cend();
		for (map <unsigned int, Clause*>::const_iterator iter = clauses->cbegin(); iter != end;)
		{
			const Clause* clause = iter->second;
			ClauseState* clauseState = this->_getState(clause);
			iter++;
			assert(clause->Contains(variable));
			if (clause->Contains(variable, state))
			{
				if (clauseState->Active)
				{
					assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
					assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
					this->deactivateClause(clauseState, clauseState->getCurrentSize());
				}
				assert(!clauseState->Active);
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
			}
			else
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
				this->reactivateClause(clauseState, clauseState->getCurrentSize());
				assert(!clauseState->Active || variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(!clauseState->Active || variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
		}
	}

	this->variableAttempts++;
	assert(!variableState->isActive());
	assert(variableState->True == state);

#ifdef STATISTICS_STEPS
	for (unsigned int stepCount = 1; stepCount < STATISTICS_STEPS; stepCount++) {
		map <unsigned int, ClauseState*>::const_iterator cend = this->clauses->cend();
		for (map <unsigned int, ClauseState*>::const_iterator iter = this->clauses->cbegin(); iter != cend; iter++)
		{
			if (iter->second->isActive()) {
				iter->second->updateStatistics(stepCount - 1);
			}
		}

		map <unsigned int, VariableState*>::const_iterator vend = this->variables->cend();
		for (map <unsigned int, VariableState*>::const_iterator iter = this->variables->cbegin(); iter != vend; iter++)
		{
			if (iter->second->isActive()) {
				iter->second->updateStatistics(stepCount);
			}
		}
	}
#endif
}
void SATState::unsetVariable(const Variable* variable)
{
	assert(this->variables->find(variable->getIdentifier()) != this->variables->cend());
	VariableState* variableState = this->_getState(variable);
	if (!variableState->isActive())
	{
		variableState->Active = true;
		this->activeVariableCount++;
		assert(this->activeVariables->find(variableState->getVariable()->getIdentifier()) == this->activeVariables->cend());
		(*activeVariables)[variableState->getVariable()->getIdentifier()] = variableState;

		map <unsigned int, Clause*>* clauses = variableState->variable->clauses;
		map <unsigned int, Clause*>::const_iterator end = clauses->cend();
		for (map <unsigned int, Clause*>::const_iterator iter = clauses->cbegin(); iter != end;)
		{
			const Clause* clause = iter->second;
			ClauseState* clauseState = this->_getState(clause);
			iter++;
			assert(clause->Contains(variable));
			if (clause->Contains(variable, variableState->True))
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) != variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) == variableState->ActiveClauses->cend());
				this->reactivateClause(clauseState, clauseState->getCurrentSize());
				assert(!clauseState->Active || variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(!clauseState->Active || variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
			else if (clauseState->Active)
			{
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
				this->updateClause(clauseState, clauseState->getCurrentSize() - 1, clauseState->getCurrentSize());
				assert(variableState->InactiveClauses->find(clause->getIdentifier()) == variableState->InactiveClauses->cend());
				assert(variableState->ActiveClauses->find(clause->getIdentifier()) != variableState->ActiveClauses->cend());
			}
		}
	}
	assert(variableState->isActive());

#ifdef STATISTICS_STEPS
	for (unsigned int stepCount = 1; stepCount < STATISTICS_STEPS; stepCount++) {
		map <unsigned int, ClauseState*>::const_iterator cend = this->clauses->cend();
		for (map <unsigned int, ClauseState*>::const_iterator iter = this->clauses->cbegin(); iter != cend; iter++)
		{
			if (iter->second->isActive()) {
				iter->second->updateStatistics(stepCount - 1);
			}
		}

		map <unsigned int, VariableState*>::const_iterator vend = this->variables->cend();
		for (map <unsigned int, VariableState*>::const_iterator iter = this->variables->cbegin(); iter != vend; iter++)
		{
			if (iter->second->isActive()) {
				iter->second->updateStatistics(stepCount);
			}
		}
	}
#endif
}

void SATState::deactivateClause(ClauseState* clause, unsigned int oldClauseCount)
{
	assert(clause->Active);
	assert(clause == this->_getState(clause->clause));
	assert(oldClauseCount <= clause->clause->Size());
	clause->Active = false;
	clause->True = true;
	this->activeClauseCount--;
	assert(activeClauses->find(clause->getClause()->getIdentifier()) != activeClauses->cend());
	activeClauses->erase(clause->getClause()->getIdentifier());
	for (unsigned int i = 0; i < clause->clause->_size; i++)
	{
		VariableState* varState = clause->variables[i];
		assert(varState == this->_getState(varState->getVariable()));
		varState->removeClause(clause, oldClauseCount);
	}
	assert(!clause->Active);
}

void SATState::updateClause(ClauseState* clause, unsigned int oldClauseCount, unsigned int newClauseCount)
{
	assert(clause->Active);
	assert(clause == this->_getState(clause->clause));
	assert(clause->getCurrentSize() == newClauseCount);
	assert(newClauseCount != 0);
	assert(oldClauseCount != newClauseCount);
	for (unsigned int i = 0; i < clause->clause->_size; i++)
	{
		VariableState* varState = clause->variables[i];
		assert(varState == this->_getState(varState->getVariable()));
		assert(varState->ActiveClauses->find(clause->clause->getIdentifier()) != varState->ActiveClauses->cend());
		if (clause->clause->Contains(varState->getVariable(), true))
		{
			assert(newClauseCount < varState->PositiveClauseSizes->size());
			assert((*(varState->PositiveClauseSizes))[oldClauseCount] != 0);
			(*(varState->PositiveClauseSizes))[newClauseCount]++;
			(*(varState->PositiveClauseSizes))[oldClauseCount]--;
		}
		else
		{
			assert(newClauseCount < varState->NegativeClauseSizes->size());
			assert((*(varState->NegativeClauseSizes))[oldClauseCount] != 0);
			(*(varState->NegativeClauseSizes))[newClauseCount]++;
			(*(varState->NegativeClauseSizes))[oldClauseCount]--;
		}
	}
	assert(clause->Active);
	assert(activeClauses->find(clause->getClause()->getIdentifier()) != activeClauses->cend());
}

void SATState::reactivateClause(ClauseState* clause, unsigned int newClauseCount)
{
	assert(!clause->Active);
	assert(clause == this->_getState(clause->clause));
	assert(clause->getCurrentSize() == newClauseCount);
	if (!clause->verifyTrue()) {
		assert(newClauseCount <= clause->clause->_size);
		clause->Active = true;
		clause->True = false;
		this->activeClauseCount++;
		assert(activeClauses->find(clause->getClause()->getIdentifier()) == activeClauses->cend());
		(*activeClauses)[clause->getClause()->getIdentifier()] = clause;
		for (unsigned int i = 0; i < clause->clause->_size; i++)
		{
			VariableState* varState = clause->variables[i];
			assert(varState == this->_getState(varState->getVariable()));
			varState->addClause(clause, newClauseCount);
		}
		assert(clause->Active);
	}
}

#ifdef _DEBUG
void SATState::verify() const
{
	for (map <unsigned int, ClauseState*>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		if (iter->second->isActive())
		{
			assert(this->activeClauses->find(iter->second->getClause()->getIdentifier()) != this->activeClauses->cend());
		}
		else
		{
			assert(this->activeClauses->find(iter->second->getClause()->getIdentifier()) == this->activeClauses->cend());
		}
	}
	assert(this->activeClauses->size() == this->activeClauseCount);

	for (map <unsigned int, VariableState*>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if (iter->second->isActive())
		{
			assert(this->activeVariables->find(iter->second->getVariable()->getIdentifier()) != this->activeVariables->cend());
		}
		else
		{
			assert(this->activeVariables->find(iter->second->getVariable()->getIdentifier()) == this->activeVariables->cend());
		}
	}
	assert(this->activeVariables->size() == this->activeVariableCount);
}
#endif

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
	this->verify();
#endif
	return this->activeClauseCount;
}
unsigned int SATState::getRemainingVariableCount() const
{
#ifdef _DEBUG
	this->verify();
#endif
	return this->activeVariableCount;
}

bool SATState::hasSolution() const
{
	return this->getRemainingClauseCount() == 0;
}
bool SATState::canSolve() const
{
	assert(this->activeVariables != NULL);
	for(unordered_map <unsigned int, VariableState *>::const_iterator iter = this->activeVariables->cbegin(); iter != this->activeVariables->cend(); iter++)
	{
		assert(iter->second->Active);
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
	for(unordered_map <unsigned int, ClauseState *>::const_iterator iter = this->activeClauses->cbegin(); iter != this->activeClauses->cend(); iter++)
	{
		assert(iter->second->isActive());
		currentState->push_back(&iter->second->getCurrentState());
	}

	return *currentState;
}

unsigned int SATState::getVariableAttempts() const
{
	return this->variableAttempts;
}

