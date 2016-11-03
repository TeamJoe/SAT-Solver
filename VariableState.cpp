#include "VariableState.h"
#include "Variable.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <vector>
#include "Debug.h"

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

VariableState::VariableState()
{
	this->NegativeClauseSizes = NULL;
	this->PositiveClauseSizes = NULL;
	this->Reset();
}

VariableState::VariableState(const Variable * v)
{
	this->NegativeClauseSizes = NULL;
	this->PositiveClauseSizes = NULL;
	this->Reset();
	this->Reset(v);
}
VariableState::~VariableState()
{
	if (this->PositiveClauseSizes != NULL) {
		delete this->PositiveClauseSizes;
	}
	if (this->NegativeClauseSizes != NULL) {
		delete this->NegativeClauseSizes;
	}
}
bool VariableState::IsSet() const {
	return this->NegativeClauseSizes != NULL && this->PositiveClauseSizes != NULL;
}
void VariableState::Reset()
{
	this->NegativesSize = 0;
	this->PositivesSize = 0;
	if (this->NegativeClauseSizes != NULL) {
		this->NegativeClauseSizes->clear();
	}
	if (this->PositiveClauseSizes != NULL) {
		this->PositiveClauseSizes->clear();
	}
}
void VariableState::Reset(const Variable * v)
{
	assert(!v->IsEvaluated());
	assert(v->IsActive());

	this->NegativesSize = v->Negatives.size();
	this->PositivesSize = v->Positives.size();

	if (this->NegativeClauseSizes == NULL) {
		this->NegativeClauseSizes = new vector <unsigned int> ();
	} else {
		this->NegativeClauseSizes->clear();
	}

	if (this->PositiveClauseSizes == NULL) {
		this->PositiveClauseSizes = new vector <unsigned int> ();
	}  else {
		this->PositiveClauseSizes->clear();
	}

	//Check the positive literals
	for(list <Literal *>::const_iterator iter = v->Positives.cbegin(); iter != v->Positives.cend(); iter++)
	{
		assert((*iter)->Contains(v));
		assert(!(*iter)->IsEvaluted());
		assert((*iter)->IsActive());
		assert((*iter)->clause->IsActive());
		if((*iter)->ClauseSize() >= this->NegativeClauseSizes->size())
		{
			unsigned int oldSize = this->NegativeClauseSizes->size();
			this->NegativeClauseSizes->resize((*iter)->ClauseSize() + 1);
			(*this->NegativeClauseSizes)[(*iter)->ClauseSize()] = 1;
			for (oldSize; oldSize < this->NegativeClauseSizes->size() - 1; oldSize++)
			{
				(*this->NegativeClauseSizes)[oldSize] = 0;
			}
		}
		else
		{
			(*this->NegativeClauseSizes)[(*iter)->ClauseSize()]++;
		}
		assert((*this->NegativeClauseSizes)[(*iter)->ClauseSize()] > 0);
	}

	//Check the negative literals
	for(list <Literal *>::const_iterator iter = v->Negatives.cbegin(); iter != v->Negatives.cend(); iter++)
	{
		assert((*iter)->Contains(v));
		assert(!(*iter)->IsEvaluted());
		assert((*iter)->IsActive());
		assert((*iter)->clause->IsActive());
		if((*iter)->ClauseSize() >= this->PositiveClauseSizes->size())
		{
			unsigned int oldSize = this->PositiveClauseSizes->size();
			this->PositiveClauseSizes->resize((*iter)->ClauseSize() + 1);
			(*this->PositiveClauseSizes)[(*iter)->ClauseSize()] = 1;
			for (oldSize; oldSize < this->PositiveClauseSizes->size() - 1; oldSize++)
			{
				(*this->PositiveClauseSizes)[oldSize] = 0;
			}
		}
		else
		{
			(*this->PositiveClauseSizes)[(*iter)->ClauseSize()]++;
		}
		assert((*this->PositiveClauseSizes)[(*iter)->ClauseSize()] > 0);
	}
}
unsigned int VariableState::getNegativesSize() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->NegativesSize;
}
unsigned int VariableState::getPositivesSize() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->PositivesSize;
}
unsigned int VariableState::getNegativeClauseCount() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	unsigned int count = 0;
	for (unsigned int i = 0; i < this->NegativeClauseSizes->size(); i++)
	{
		count += (*this->NegativeClauseSizes)[i];
	}
	return count;
}
unsigned int VariableState::getPositiveClauseCount() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	unsigned int count = 0;
	for (unsigned int i = 0; i < this->PositiveClauseSizes->size(); i++)
	{
		count += (*this->PositiveClauseSizes)[i];
	}
	return count;
}
unsigned int VariableState::getNegativeClauseCount(const unsigned int & ClauseSize) const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
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
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	if (ClauseSize >= this->PositiveClauseSizes->size())
	{
		return 0;
	} 
	else
	{
		return (*this->PositiveClauseSizes)[ClauseSize];
	}
}

unsigned int VariableState::getLargestNumber() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getNegativesSize() > this->getPositivesSize()
		? this->getNegativesSize() 
		: this->getPositivesSize();
}
unsigned int VariableState::getSmallestNumber() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getNegativesSize() < this->getPositivesSize()
		? this->getNegativesSize() 
		: this->getPositivesSize();
}
unsigned int VariableState::getDifference() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getNegativesSize() > this->getPositivesSize()
		? this->getNegativesSize() - this->getPositivesSize()
		: this->getPositivesSize()- this->getNegativesSize();
}

unsigned int VariableState::getLargestPositiveClauseSize() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	if (this->PositiveClauseSizes->size() == 0)
	{
		return 0;
	}
	return this->PositiveClauseSizes->size() - 1;
}
unsigned int VariableState::getLargestNegativeClauseSize() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	if (this->NegativeClauseSizes->size() == 0)
	{
		return 0;
	}
	return this->NegativeClauseSizes->size() - 1;
}
unsigned int VariableState::getSmallestPositiveClauseSize() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
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
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
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
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getPositiveClauseCount(this->getLargestPositiveClauseSize());
}
unsigned int VariableState::getLargestNegativeClauseCount() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getNegativeClauseCount(this->getLargestNegativeClauseSize());
}
unsigned int VariableState::getSmallestPositiveClauseCount() const 
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getPositiveClauseCount(this->getSmallestPositiveClauseSize());
}
unsigned int VariableState::getSmallestNegativeClauseCount() const 
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getNegativeClauseCount(this->getSmallestNegativeClauseSize());
}
unsigned int VariableState::getSmallestClauseSize() const 
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getSmallestNegativeClauseSize() < this->getSmallestPositiveClauseSize()
		? this->getSmallestNegativeClauseSize()
		: this->getSmallestPositiveClauseSize();
}
unsigned int VariableState::getLargestClauseSize() const 
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getLargestNegativeClauseSize() < this->getLargestPositiveClauseSize()
		? this->getLargestNegativeClauseSize()
		: this->getLargestPositiveClauseSize();
}
unsigned int VariableState::getSmallestClauseCount() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getSmallestNegativeClauseSize() == this->getSmallestPositiveClauseSize()
		? this->getSmallestNegativeClauseCount() + this->getSmallestPositiveClauseCount()
		: this->getSmallestNegativeClauseSize() < this->getSmallestPositiveClauseSize()
			? this->getSmallestNegativeClauseCount()
			: this->getSmallestPositiveClauseCount();
}
unsigned int VariableState::getLargestClauseCount() const
{
	assert(this->PositiveClauseSizes != NULL);
	assert(this->NegativeClauseSizes != NULL);
	return this->getLargestNegativeClauseSize() == this->getLargestPositiveClauseSize()
		? this->getLargestNegativeClauseCount() + this->getLargestPositiveClauseCount()
		: this->getLargestNegativeClauseSize() > this->getLargestPositiveClauseSize()
			? this->getLargestNegativeClauseCount()
			: this->getLargestPositiveClauseCount();
}


bool VariableState::operator==(const VariableState & variable) const
{
	if (this->NegativesSize != variable.NegativesSize) 
	{
		return false;
	}
	else if (this->PositivesSize != variable.PositivesSize) 
	{
		return false;
	}
	else if (this->NegativeClauseSizes->size() != variable.NegativeClauseSizes->size())
	{
		return false;
	}
	else if (this->PositiveClauseSizes->size() != variable.PositiveClauseSizes->size())
	{
		return false;
	}
	for (unsigned int i = 0; i < this->NegativeClauseSizes->size(); i++)
	{
		if ((*this->NegativeClauseSizes)[i] != (*variable.NegativeClauseSizes)[i])
		{
			return false;
		}
	}
	for (unsigned int i = 0; i < this->PositiveClauseSizes->size(); i++)
	{
		if ((*this->PositiveClauseSizes)[i] != (*variable.PositiveClauseSizes)[i])
		{
			return false;
		}
	}
	return true;
}