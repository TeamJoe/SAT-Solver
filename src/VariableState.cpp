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
VariableState::VariableState(SATState * sat, const Variable * v)
{
	this->satState = sat;
	this->variable = v;
	this->Active = true;
	this->True = false;
	this->NegativesSize = 0;
	this->PositivesSize = 0;
	this->NegativeClauseSizes = NULL;
	this->PositiveClauseSizes = NULL;

	this->ActiveClauses = new map <unsigned int, ClauseState *>();
	this->InactiveClauses = new map <unsigned int, ClauseState *>();
	for(map <unsigned int, Clause *>::const_iterator iter = v->clauses->cbegin(); iter !=  v->clauses->cend(); iter++)
	{
		ClauseState * clauseState = this->satState->_getState(iter->second);
		assert(clauseState->getClause() == iter->second);
		assert(this->ActiveClauses->find(clauseState->getClause()->getIdentifier()) == this->ActiveClauses->cend());
		(*(this->ActiveClauses))[clauseState->getClause()->getIdentifier()] = clauseState;
		assert(this->ActiveClauses->find(clauseState->getClause()->getIdentifier()) != this->ActiveClauses->cend());
	}
	assert(this->ActiveClauses->size() == this->getVariable()->clauses->size());
	assert(this->InactiveClauses->size() == 0);

	assert(this->NegativesSize == 0);
	assert(this->PositivesSize == 0);
	assert(this->NegativeClauseSizes == NULL);
	assert(this->PositiveClauseSizes == NULL);

	this->NegativeClauseSizes = new vector <unsigned int>();
	this->PositiveClauseSizes = new vector <unsigned int>();

	const Variable * variable = this->getVariable();
	map <unsigned int, Clause *> * clauses = variable->clauses;
	map <unsigned int, Clause *>::const_iterator end = clauses->cend();
	for(map <unsigned int, Clause *>::const_iterator iter = clauses->cbegin(); iter != end; iter++)
	{
		Clause * clause = iter->second;
		assert(clause->Contains(this->variable));
		assert(satState->isActive(clause));
		unsigned int currentClauseSize = clause->Size();

		if (clause->Contains(this->variable, true)) {
			if(currentClauseSize >= this->PositiveClauseSizes->size())
			{
				unsigned int oldSize = this->PositiveClauseSizes->size();
				this->PositiveClauseSizes->resize(currentClauseSize + (unsigned int)1);
				(*this->PositiveClauseSizes)[currentClauseSize] = 1;
				for (oldSize; oldSize < this->PositiveClauseSizes->size() - 1; oldSize++)
				{
					(*this->PositiveClauseSizes)[oldSize] = 0;
				}
			}
			else
			{
				(*this->PositiveClauseSizes)[currentClauseSize]++;
			}
			assert((*this->PositiveClauseSizes)[currentClauseSize] > 0);
			this->PositivesSize++;
		} else {
			if(currentClauseSize >= this->NegativeClauseSizes->size())
			{
				unsigned int oldSize = this->NegativeClauseSizes->size();
				this->NegativeClauseSizes->resize(currentClauseSize + (unsigned int)1);
				(*this->NegativeClauseSizes)[currentClauseSize] = 1;
				for (oldSize; oldSize < this->NegativeClauseSizes->size() - 1; oldSize++)
				{
					(*this->NegativeClauseSizes)[oldSize] = 0;
				}
			}
			else
			{
				(*this->NegativeClauseSizes)[currentClauseSize]++;
			}
			assert((*this->NegativeClauseSizes)[currentClauseSize] > 0);
			this->NegativesSize++;
		}
	}

	assert(this->NegativesSize + this->PositivesSize == clauses->size());
	assert(this->NegativeClauseSizes != NULL);
	assert(this->PositiveClauseSizes != NULL);
}
VariableState::VariableState(SATState * sat, VariableState * v)
{
	this->satState = sat;
	this->variable = v->variable;
	this->Active = v->Active;
	this->True = v->True;
	this->NegativesSize = v->NegativesSize;
	this->PositivesSize = v->PositivesSize;
	this->NegativeClauseSizes = new vector <unsigned int>(v->NegativeClauseSizes->size());
	this->PositiveClauseSizes = new vector <unsigned int>(v->PositiveClauseSizes->size());

	this->ActiveClauses = new map <unsigned int, ClauseState *>();
	for(map <unsigned int, ClauseState *>::const_iterator iter = v->ActiveClauses->cbegin(); iter !=  v->ActiveClauses->cend(); iter++)
	{
		ClauseState * clauseState = this->satState->_getState(iter->second->getClause());
		assert(clauseState != iter->second);
		assert(clauseState->getClause() == iter->second->getClause());
		assert(this->ActiveClauses->find(clauseState->getClause()->getIdentifier()) == this->ActiveClauses->cend());
		(*(this->ActiveClauses))[clauseState->getClause()->getIdentifier()] = clauseState;
		assert(this->ActiveClauses->find(clauseState->getClause()->getIdentifier()) != this->ActiveClauses->cend());
	}
	assert(this->ActiveClauses->size() == v->ActiveClauses->size());

	this->InactiveClauses = new map <unsigned int, ClauseState *>();
	for(map <unsigned int, ClauseState *>::const_iterator iter = v->InactiveClauses->cbegin(); iter !=  v->InactiveClauses->cend(); iter++)
	{
		ClauseState * clauseState = this->satState->_getState(iter->second->getClause());
		assert(clauseState != iter->second);
		assert(clauseState->getClause() == iter->second->getClause());
		assert(this->InactiveClauses->find(clauseState->getClause()->getIdentifier()) == this->InactiveClauses->cend());
		(*(this->InactiveClauses))[clauseState->getClause()->getIdentifier()] = clauseState;
		assert(this->InactiveClauses->find(clauseState->getClause()->getIdentifier()) != this->InactiveClauses->cend());
	}
	assert(this->InactiveClauses->size() == v->InactiveClauses->size());
}
VariableState::~VariableState()
{
	if (this->PositiveClauseSizes != NULL) {
		delete this->PositiveClauseSizes;
		this->PositiveClauseSizes= NULL;
	}
	if (this->NegativeClauseSizes != NULL) {
		delete this->NegativeClauseSizes;
		this->NegativeClauseSizes= NULL;
	}
	if (this->ActiveClauses != NULL) {
		delete this->ActiveClauses;
		this->ActiveClauses= NULL;
	}
	if (this->InactiveClauses != NULL) {
		delete this->InactiveClauses;
		this->InactiveClauses= NULL;
	}
}

#ifdef _DEBUG
void VariableState::checkState() const
{
	unsigned int negativeSize = 0;
	unsigned int positivesSize = 0;
	vector <unsigned int> * negativeClauseSizes = new vector <unsigned int>();
	vector <unsigned int> * positiveClauseSizes = new vector <unsigned int>();

	for(map <unsigned int, ClauseState *>::const_iterator iter = this->InactiveClauses->cbegin(); iter != this->InactiveClauses->cend(); iter++)
	{
		assert(iter->second->getClause()->Contains(this->variable));
		assert(!satState->isActive(iter->second->getClause()));
	}

	for(map <unsigned int, ClauseState *>::const_iterator iter = this->ActiveClauses->cbegin(); iter != this->ActiveClauses->cend(); iter++)
	{
		assert(iter->second->getClause()->Contains(this->variable));
		assert(satState->isActive(iter->second->getClause()));
		unsigned int currentClauseSize = iter->second->getCurrentSize();

		if (iter->second->getClause()->Contains(this->variable, true)) {
			if(currentClauseSize >= positiveClauseSizes->size())
			{
				unsigned int oldSize = positiveClauseSizes->size();
				positiveClauseSizes->resize(currentClauseSize + 1);
				(*positiveClauseSizes)[currentClauseSize] = 1;
				for (oldSize; oldSize < positiveClauseSizes->size() - 1; oldSize++)
				{
					(*positiveClauseSizes)[oldSize] = 0;
				}
			}
			else
			{
				(*positiveClauseSizes)[currentClauseSize]++;
			}
			assert((*positiveClauseSizes)[currentClauseSize] > 0);
			positivesSize++;
		} else {
			if(currentClauseSize >= negativeClauseSizes->size())
			{
				unsigned int oldSize = negativeClauseSizes->size();
				negativeClauseSizes->resize(currentClauseSize + 1);
				(*negativeClauseSizes)[currentClauseSize] = 1;
				for (oldSize; oldSize < negativeClauseSizes->size() - 1; oldSize++)
				{
					(*negativeClauseSizes)[oldSize] = 0;
				}
			}
			else
			{
				(*negativeClauseSizes)[currentClauseSize]++;
			}
			assert((*negativeClauseSizes)[currentClauseSize] > 0);
			negativeSize++;
		}
	}

	assert(this->NegativesSize + this->PositivesSize == this->ActiveClauses->size());
	assert(this->NegativeClauseSizes != NULL);
	assert(this->PositiveClauseSizes != NULL);

	assert(this->NegativesSize == negativeSize);
	assert(this->PositivesSize == positivesSize);

	for (unsigned int i = 0; i < this->PositiveClauseSizes->size(); i++) {
		if (i >= positiveClauseSizes->size()) {
			assert((*(this->PositiveClauseSizes))[i] == 0);
		} else {
			assert((*(this->PositiveClauseSizes))[i] == (*(positiveClauseSizes))[i]);
		}
	}

	for (unsigned int i = 0; i < this->NegativeClauseSizes->size(); i++) {
		if (i >= negativeClauseSizes->size()) {
			assert((*(this->NegativeClauseSizes))[i] == 0);
		} else {
			assert((*(this->NegativeClauseSizes))[i] == (*(negativeClauseSizes))[i]);
		}
	}

	delete negativeClauseSizes;
	delete positiveClauseSizes;
}
#endif

//-------------------------------
//
// Internal Functions
//
//------------------------------

void VariableState::reset()
{
}

void VariableState::addClause(const ClauseState * clauseState, const unsigned int clauseCount)
{
	assert(clauseState->isActive());
	assert(clauseCount == clauseState->getCurrentSize());

	const Clause * clause = clauseState->getClause();
	assert(this->InactiveClauses->find(clause->getIdentifier()) != this->InactiveClauses->cend());
	assert(this->ActiveClauses->find(clause->getIdentifier()) == this->ActiveClauses->cend());
	(*this->ActiveClauses)[clause->getIdentifier()] = this->InactiveClauses->find(clause->getIdentifier())->second;
	this->InactiveClauses->erase(clause->getIdentifier());
	assert(this->InactiveClauses->find(clause->getIdentifier()) == this->InactiveClauses->cend());
	assert(this->ActiveClauses->find(clause->getIdentifier()) != this->ActiveClauses->cend());
	assert(this->InactiveClauses->size() + this->ActiveClauses->size() == this->getVariable()->clauses->size()); 
	if (clause->Contains(this->getVariable(), true))
	{
		this->PositivesSize++;
		assert(clauseCount < this->PositiveClauseSizes->size());
		(*(this->PositiveClauseSizes))[clauseCount]++;
	}
	else
	{
		this->NegativesSize++;
		assert(clauseCount  < this->NegativeClauseSizes->size());
		(*(this->NegativeClauseSizes))[clauseCount]++;
	}
}
void VariableState::removeClause(const ClauseState * clauseState, const unsigned int clauseCount)
{
	assert(!clauseState->isActive());
	assert(clauseCount == clauseState->getCurrentSize() || clauseCount == clauseState->getCurrentSize() + 1);

	const Clause * clause = clauseState->getClause();
	assert(this->InactiveClauses->find(clause->getIdentifier()) == this->InactiveClauses->cend());
	assert(this->ActiveClauses->find(clause->getIdentifier()) != this->ActiveClauses->cend());
	(*this->InactiveClauses)[clause->getIdentifier()] = this->ActiveClauses->find(clause->getIdentifier())->second;
	this->ActiveClauses->erase(clause->getIdentifier());
	assert(this->InactiveClauses->find(clause->getIdentifier()) != this->InactiveClauses->cend());
	assert(this->ActiveClauses->find(clause->getIdentifier()) == this->ActiveClauses->cend());
	assert(this->InactiveClauses->size() + this->ActiveClauses->size() == this->getVariable()->clauses->size()); 
	if (clause->Contains(this->getVariable(), true))
	{
		assert(this->PositivesSize != 0);
		this->PositivesSize--;
		assert(clauseCount < this->PositiveClauseSizes->size());
		assert((*(this->PositiveClauseSizes))[clauseCount] != 0);
		(*(this->PositiveClauseSizes))[clauseCount]--;
	}
	else
	{
		assert(this->NegativesSize != 0);
		this->NegativesSize--;
		assert(clauseCount  < this->NegativeClauseSizes->size());
		assert((*(this->NegativeClauseSizes))[clauseCount] != 0);
		(*(this->NegativeClauseSizes))[clauseCount]--;
	}
}

//-------------------------------
//
// Operators
//
//------------------------------
const Variable * VariableState::getVariable() const
{
	return this->variable;
}
bool VariableState::isActive() const
{
	return this->Active;
}
int VariableState::getValue() const
{
	if (!this->isActive())
	{
		return this->True ? this->variable->GetVariable() : (-1 * this->variable->GetVariable());
	}
	return 0;
}
bool VariableState::hasSolution() const
{
	/* Slower? */
	/*
	int smallestNegative = this->getSmallestNegativeClauseSize();
	int smallestPositive = this->getSmallestPositiveClauseSize();
	return (smallestNegative != 0 && smallestPositive != 0 && !(smallestNegative == 1 && smallestPositive == 1));
	*/
	return (getNegativeClauseCount(0) == 0 && getPositiveClauseCount(0) == 0 && (getNegativeClauseCount(1) == 0 || getPositiveClauseCount(1) == 0));
}

//-------------------------------
//
// State Helper Functions
//
//------------------------------

unsigned int VariableState::getNegativesSize() const
{
#if _DEBUG
	this->checkState();
#endif
	return this->NegativesSize;
}
unsigned int VariableState::getPositivesSize() const
{
#if _DEBUG
	this->checkState();
#endif
	return this->PositivesSize;
}
unsigned int VariableState::getNegativeClauseCount(const unsigned int & ClauseSize) const
{
#if _DEBUG
	this->checkState();
#endif
	if (ClauseSize >= this->NegativeClauseSizes->size()) 
	{
		return 0;
	}
	else 
	{
		return (*this->NegativeClauseSizes)[ClauseSize];
	}
}
unsigned int VariableState::getPositiveClauseCount(const unsigned int & ClauseSize) const
{
#if _DEBUG
	this->checkState();
#endif
	if (ClauseSize >= this->PositiveClauseSizes->size())
	{
		return 0;
	}
	else
	{
		return (*this->PositiveClauseSizes)[ClauseSize];
	}
}

unsigned int VariableState::getTotalNumber() const
{
	return this->getNegativesSize() + this->getPositivesSize();
}
unsigned int VariableState::getLargestNumber() const
{
	return this->getNegativesSize() > this->getPositivesSize()
		? this->getNegativesSize() 
		: this->getPositivesSize();
}
unsigned int VariableState::getSmallestNumber() const
{
	return this->getNegativesSize() < this->getPositivesSize()
		? this->getNegativesSize() 
		: this->getPositivesSize();
}
unsigned int VariableState::getDifference() const
{
	return this->getNegativesSize() > this->getPositivesSize()
		? this->getNegativesSize() - this->getPositivesSize()
		: this->getPositivesSize()- this->getNegativesSize();
}

unsigned int VariableState::getLargestPositiveClauseSize() const
{
#if _DEBUG
	this->checkState();
#endif
	if (0 < this->PositiveClauseSizes->size())
	{
		for (unsigned int i = this->PositiveClauseSizes->size() - 1; i != 0; i--)
		{
			if ((*this->PositiveClauseSizes)[i] > 0)
			{
				return i;
			}
		}
	}
	return 0;
}
unsigned int VariableState::getLargestNegativeClauseSize() const
{
#if _DEBUG
	this->checkState();
#endif
	if (0 < this->NegativeClauseSizes->size())
	{
		for (unsigned int i = this->NegativeClauseSizes->size() - 1; i != 0; i--)
		{
			if ((*this->NegativeClauseSizes)[i] > 0)
			{
				return i;
			}
		}
	}
	return 0;
}
unsigned int VariableState::getSmallestPositiveClauseSize() const
{
#if _DEBUG
	this->checkState();
#endif
	for (unsigned int i = 0; i < this->PositiveClauseSizes->size(); i++)
	{
		if ((*this->PositiveClauseSizes)[i] > 0)
		{
			return i;
		}
	}
	return 0xFFFFFFFF;
}
unsigned int VariableState::getSmallestNegativeClauseSize() const
{
#if _DEBUG
	this->checkState();
#endif
	for (unsigned int i = 0; i < this->NegativeClauseSizes->size(); i++)
	{
		if ((*this->NegativeClauseSizes)[i] > 0)
		{
			return i;
		}
	}
	return 0xFFFFFFFF;
}
unsigned int VariableState::getLargestPositiveClauseCount() const
{
	return this->getPositiveClauseCount(this->getLargestPositiveClauseSize());
}
unsigned int VariableState::getLargestNegativeClauseCount() const
{
	return this->getNegativeClauseCount(this->getLargestNegativeClauseSize());
}
unsigned int VariableState::getSmallestPositiveClauseCount() const
{
	return this->getPositiveClauseCount(this->getSmallestPositiveClauseSize());
}
unsigned int VariableState::getSmallestNegativeClauseCount() const
{
	return this->getNegativeClauseCount(this->getSmallestNegativeClauseSize());
}
unsigned int VariableState::getSmallestClauseSize() const
{
	return this->getSmallestNegativeClauseSize() < this->getSmallestPositiveClauseSize()
		? this->getSmallestNegativeClauseSize()
		: this->getSmallestPositiveClauseSize();
}
unsigned int VariableState::getLargestClauseSize() const
{
	return this->getLargestNegativeClauseSize() < this->getLargestPositiveClauseSize()
		? this->getLargestNegativeClauseSize()
		: this->getLargestPositiveClauseSize();
}
unsigned int VariableState::getSmallestClauseCount() const
{
	return this->getSmallestNegativeClauseSize() == this->getSmallestPositiveClauseSize()
		? this->getSmallestNegativeClauseCount() + this->getSmallestPositiveClauseCount()
		: this->getSmallestNegativeClauseSize() < this->getSmallestPositiveClauseSize()
			? this->getSmallestNegativeClauseCount()
			: this->getSmallestPositiveClauseCount();
}
unsigned int VariableState::getLargestClauseCount() const
{
	return this->getLargestNegativeClauseSize() == this->getLargestPositiveClauseSize()
		? this->getLargestNegativeClauseCount() + this->getLargestPositiveClauseCount()
		: this->getLargestNegativeClauseSize() > this->getLargestPositiveClauseSize()
			? this->getLargestNegativeClauseCount()
			: this->getLargestPositiveClauseCount();
}