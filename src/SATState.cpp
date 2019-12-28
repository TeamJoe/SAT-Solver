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

void SATState::reset()
{
	//Loop not current required as the variables do nothing on reset
	assert(this->variables != NULL);
	/*for(map <unsigned int, VariableState *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		iter->second->reset();
	}*/

	//Loop not current required as the clauses do nothing on reset
	assert(this->clauses != NULL);
	/*for(map <unsigned int, ClauseState *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		iter->second->reset();
	}*/
}

bool SATState::isActive(const Clause * clause) const
{
	return this->clauses->find(clause->getIdentifier())->second->isActive();
}
bool SATState::isActive(const Variable * variable) const
{
	return this->variables->find(variable->getIdentifier())->second->isActive();
}

const SAT * SATState::getSAT() const
{
	return this->sat;
}
ClauseState * SATState::_getState(const Clause * clause)
{
	return this->clauses->find(clause->getIdentifier())->second;
}
VariableState * SATState::_getState(const Variable * variable)
{
	return this->variables->find(variable->getIdentifier())->second;
}

const ClauseState * SATState::getState(const Clause * clause) const
{
	return this->clauses->find(clause->getIdentifier())->second;
}
const VariableState * SATState::getState(const Variable * variable) const
{
	return this->variables->find(variable->getIdentifier())->second;
}

void SATState::setVariable(const Variable * variable, const bool state)
{
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
			unsigned int clauseCount = clauseState->getCurrentSize();
			assert(clauseCount < clause->Size());
			if(clause->Contains(variable, state))
			{
				assert(this->_getState(clause)->Active);
				assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
				assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
				clauseState->Active = false;
				clauseState->True = true;
				this->activeClauseCount--;
				for(unsigned int i = 0; i < clause->_size; i++)
				{
					const Variable * var = clause->clause[i]->getVariable();
					VariableState * varState = this->_getState(var);
					assert(var == varState->getVariable());
					varState->removeClause(clauseState, clauseCount + 1);
				}
				assert(!this->_getState(clause)->Active);
				assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->InactiveClauses->cend());
				assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->ActiveClauses->cend());
			}
			else
			{
				assert(this->_getState(clause)->Active);
				assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
				assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
				for(unsigned int i = 0; i < clause->_size; i++)
				{
					const Variable * var = clause->clause[i]->getVariable();
					VariableState * varState = this->_getState(var);
					assert(var == varState->getVariable());
					assert(varState->ActiveClauses->find(clause->getIdentifier()) != varState->ActiveClauses->cend());
					if (clause->Contains(var, true))
					{
						assert(clauseCount < varState->PositiveClauseSizes->size());
						assert((*(varState->PositiveClauseSizes))[clauseCount + 1] != 0);
						(*(varState->PositiveClauseSizes))[clauseCount]++;
						(*(varState->PositiveClauseSizes))[clauseCount + 1]--;
					}
					else
					{
						assert(clauseCount < varState->NegativeClauseSizes->size());
						assert((*(varState->NegativeClauseSizes))[clauseCount + 1] != 0);
						(*(varState->NegativeClauseSizes))[clauseCount]++;
						(*(varState->NegativeClauseSizes))[clauseCount + 1]--;
					}
				}
				assert(this->_getState(clause)->Active);
				assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
				assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
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
			iter++;
			assert(clause->Contains(variable));
			if (clause->Contains(variable, state))
			{
				if (this->_getState(clause)->Active)
				{
					assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
					assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
					ClauseState * clauseState = this->_getState(clause);
					unsigned int clauseCount = clauseState->getCurrentSize();
					assert(clauseCount < clause->Size());
					clauseState->Active = false;
					clauseState->True = true;
					this->activeClauseCount--;
					for(unsigned int i = 0; i < clause->_size; i++)
					{
						const Variable * var = clause->clause[i]->getVariable();
						VariableState * varState = this->_getState(var);
						assert(var == varState->getVariable());
						varState->removeClause(clauseState, clauseCount);
					}
				}
				assert(!this->_getState(clause)->Active);
				assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->InactiveClauses->cend());
				assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->ActiveClauses->cend());
			}
			else
			{
				assert(!this->_getState(clause)->Active);
				assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->InactiveClauses->cend());
				assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->ActiveClauses->cend());
				bool isTrue = false;
				for(unsigned int i = 0; i < clause->_size; i++)
				{
					const Literal * lit = clause->clause[i];
					VariableState * varState = this->_getState(lit->getVariable());
					if (!varState->isActive() && (varState->True == lit->GetType()))
					{
						isTrue = true;
						break;
					}
				}
				if (!isTrue)
				{
					ClauseState * clauseState = this->_getState(clause);
					unsigned int clauseCount = clauseState->getCurrentSize();
					assert(clauseCount < clause->Size());
					clauseState->Active = true;
					clauseState->True = false;
					this->activeClauseCount++;
					assert(clause == clauseState->clause);
					for(unsigned int i = 0; i < clause->_size; i++)
					{
						const Variable * var = clause->clause[i]->getVariable();
						VariableState * varState = this->_getState(var);
						assert(var == varState->getVariable());
						varState->addClause(clauseState, clauseCount);
					}
					assert(this->_getState(clause)->Active);
					assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
					assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
				}
			}
		}
	}

	this->variableAttempts++;
	assert(!variableState->isActive());
	assert(variableState->True == state);
}
void SATState::unsetVariable(const Variable * variable)
{
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
			iter++;
			assert(clause->Contains(variable));
			if (clause->Contains(variable, variableState->True)) 
			{
				assert(!this->_getState(clause)->Active);
				assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->InactiveClauses->cend());
				assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->ActiveClauses->cend());
				bool isTrue = false;
				for(unsigned int i = 0; i < clause->_size; i++)
				{
					const Literal * lit = clause->clause[i];
					VariableState * varState = this->_getState(lit->getVariable());
					if (!varState->isActive() && (varState->True == lit->GetType()))
					{
						isTrue = true;
						break;
					}
				}
				if (!isTrue)
				{
					ClauseState * clauseState = this->_getState(clause);
					unsigned int clauseCount = clauseState->getCurrentSize();
					assert(clauseCount != 0);
					clauseState->Active = true;
					clauseState->True = false;
					this->activeClauseCount++;
					assert(clause == clauseState->clause);
					for(unsigned int i = 0; i < clause->_size; i++)
					{
						const Variable * var = clause->clause[i]->getVariable();
						VariableState * varState = this->_getState(var);
						assert(var == varState->getVariable());
						varState->addClause(clauseState, clauseCount);
					}
					assert(this->_getState(clause)->Active);
					assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
					assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
				}
			}
			else
			{
				ClauseState * clauseState = this->_getState(clause);
				if (clauseState->Active)
				{
					unsigned int clauseCount = clauseState->getCurrentSize();
					assert(clauseCount != 0);
					assert(this->_getState(clause)->Active);
					assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
					assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
					for(unsigned int i = 0; i < clause->_size; i++)
					{
						const Variable * var = clause->clause[i]->getVariable();
						VariableState * varState = this->_getState(var);
						assert(var == varState->getVariable());
						assert(varState->ActiveClauses->find(clause->getIdentifier()) != varState->ActiveClauses->cend());
						if (clause->Contains(var, true))
						{
							assert(clauseCount < varState->PositiveClauseSizes->size());
							assert((*(varState->PositiveClauseSizes))[clauseCount - 1] != 0);
							(*(varState->PositiveClauseSizes))[clauseCount]++;
							(*(varState->PositiveClauseSizes))[clauseCount - 1]--;
						}
						else
						{
							assert(clauseCount < varState->NegativeClauseSizes->size());
							assert((*(varState->NegativeClauseSizes))[clauseCount - 1] != 0);
							(*(varState->NegativeClauseSizes))[clauseCount]++;
							(*(varState->NegativeClauseSizes))[clauseCount - 1]--;
						}
					}
					assert(this->_getState(clause)->Active);
					assert(this->_getState(variable)->InactiveClauses->find(clause->getIdentifier()) == this->_getState(variable)->InactiveClauses->cend());
					assert(this->_getState(variable)->ActiveClauses->find(clause->getIdentifier()) != this->_getState(variable)->ActiveClauses->cend());
				}
			}
		}
	}
	assert(variableState->isActive());
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

