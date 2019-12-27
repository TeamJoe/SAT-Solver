#include "Variable.h"
#include "VariableState.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Debug.h"

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

//******************************
//------------------------------
//
// CONSTRUCTORS
//
//------------------------------
//******************************
Variable::Variable(const int Variable_Number, SAT * _parent)
{
	assert(Variable_Number != 0);
	this->Variable_Number = Variable_Number;

	if(this->Variable_Number < 0)
	{
		this->Variable_Number *= -1;
	}
	assert(this->Variable_Number > 0);

#ifdef KEEP_VARIABLE_HISTORY
	this->CurrentState = NULL;
#else
	this->CurrentState = new VariableState();
#endif
	this->isEvaluated = false;
	this->_parent = _parent;

}
Variable::~Variable()
{
	assert(this->Variable_Number != 0);
	while(this->Positives.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->Positives.size();
		#endif
		assert((*this->Positives.cbegin())->variable = this);
		assert((*this->Positives.cbegin())->IsActive());
		delete *this->Positives.cbegin();
		#ifdef _DEBUG
			assert(this->Positives.size() == previousSize - 1);
		#endif
	}
	while(this->Negatives.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->Negatives.size();
		#endif
		assert((*this->Negatives.cbegin())->variable = this);
		assert((*this->Negatives.cbegin())->IsActive());
		delete *this->Negatives.cbegin();
		#ifdef _DEBUG
			assert(this->Negatives.size() == previousSize - 1);
		#endif
	}
	while(this->InactivePositives.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->InactivePositives.size();
		#endif
		assert((*this->InactivePositives.cbegin())->variable = this);
		assert(!(*this->InactivePositives.cbegin())->IsActive());
		delete *this->InactivePositives.cbegin();
		#ifdef _DEBUG
			assert(this->InactivePositives.size() == previousSize - 1);
		#endif
	}
	while(this->InactiveNegatives.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->InactiveNegatives.size();
		#endif
		assert((*this->InactiveNegatives.cbegin())->variable = this);
		assert(!(*this->InactiveNegatives.cbegin())->IsActive());
		delete *this->InactiveNegatives.cbegin();
		#ifdef _DEBUG
			assert(this->InactiveNegatives.size() == previousSize - 1);
		#endif
	}

#ifdef KEEP_VARIABLE_HISTORY
	if (this->CurrentState != NULL && (*this->PreviousStates.crbegin()) != this->CurrentState) {
		delete this->CurrentState;
	}
	this->CurrentState = NULL;
	while(this->PreviousStates.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->PreviousStates.size();
		#endif
		delete *this->PreviousStates.cbegin();
		this->PreviousStates.pop_front();
		#ifdef _DEBUG
			assert(this->PreviousStates.size() == previousSize - 1);
		#endif
	}
	assert(this->PreviousStates.size() == 0);
#else
	delete this->CurrentState;
#endif
	assert(this->Positives.size() == 0);
	assert(this->Negatives.size() == 0);
	assert(this->InactivePositives.size() == 0);
	assert(this->InactiveNegatives.size() == 0);
	this->Variable_Number = 0;
}
Variable * Variable::copy(SAT * _parent) const
{
	return new Variable(this->Variable_Number, _parent);
}

//******************************
//------------------------------
//
// MODIFIERS
//
//------------------------------
//******************************

void Variable::Revert()
{
	this->isEvaluated = false;
	while(this->InactivePositives.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->InactivePositives.size();
		#endif
		assert((*this->InactivePositives.cbegin())->variable = this);
		assert(!(*this->InactivePositives.cbegin())->IsActive());
		this->Add(this->InactivePositives.cbegin());
		#ifdef _DEBUG
			assert(this->InactivePositives.size() == previousSize - 1);
		#endif
	}
	while(this->InactiveNegatives.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->InactiveNegatives.size();
		#endif
		assert((*this->InactiveNegatives.cbegin())->variable = this);
		assert(!(*this->InactiveNegatives.cbegin())->IsActive());
		this->Add(this->InactiveNegatives.cbegin());
		#ifdef _DEBUG
			assert(this->InactiveNegatives.size() == previousSize - 1);
		#endif
	}

#ifdef KEEP_VARIABLE_HISTORY
	if (this->CurrentState != NULL && (*this->PreviousStates.crbegin()) != this->CurrentState) {
		delete this->CurrentState;
	}
	this->CurrentState = NULL;
	while(this->PreviousStates.size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->PreviousStates.size();
		#endif
		delete *this->PreviousStates.cbegin();
		this->PreviousStates.pop_front();
		#ifdef _DEBUG
			assert(this->PreviousStates.size() == previousSize - 1);
		#endif
	}
	assert(this->PreviousStates.size() == 0);
#else
	this->CurrentState->Reset();
#endif
	assert(this->InactivePositives.size() == 0);
	assert(this->InactiveNegatives.size() == 0);
}

//a debugging sanity check function
bool doesListContain(Literal * lit, list <Literal *> search)
{
	for(list <Literal *>::const_iterator i = search.cbegin(); i != search.cend(); i++)
	{
		if(*i == lit)
		{
			return true;
		}
	}
	return false;
}

//only used during construction
void Variable::Add(Literal * lit)
{
	assert(lit);
	assert(lit->Contains(this, true) || lit->Contains(this, false));
	if(lit->GetType())
	{
		this->Positives.push_back(lit);
		lit->SetListPointer((--this->Positives.end()), true);

		assert(!doesListContain(lit, this->InactivePositives));
		assert(doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));
	}
	else
	{
		this->Negatives.push_back(lit);
		lit->SetListPointer((--this->Negatives.end()), true);

		assert(!doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(doesListContain(lit, this->Negatives));
	}
	assert(lit->IsActive());
}
void Variable::Remove(list <Literal *>::const_iterator litIter)
{
	Literal * lit = *litIter;
	assert(lit);
	assert(lit->Contains(this));
	assert(lit->clause);
	if(lit->GetType())
	{
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));
		if(lit->IsActive())
		{
			assert(!doesListContain(lit, this->InactivePositives));
			assert(doesListContain(lit, this->Positives));
			this->Positives.erase(litIter);
		}
		else
		{
			assert(doesListContain(lit, this->InactivePositives));
			assert(!doesListContain(lit, this->Positives));
			this->InactivePositives.erase(litIter);
		}
	}
	else
	{
		assert(!doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		if(lit->IsActive())
		{
			assert(!doesListContain(lit, this->InactiveNegatives));
			assert(doesListContain(lit, this->Negatives));
			this->Negatives.erase(litIter);
		}
		else
		{
			assert(doesListContain(lit, this->InactiveNegatives));
			assert(!doesListContain(lit, this->Negatives));
			this->InactiveNegatives.erase(litIter);
		}
	}

	assert(!doesListContain(lit, this->InactivePositives));
	assert(!doesListContain(lit, this->Positives));
	assert(!doesListContain(lit, this->InactiveNegatives));
	assert(!doesListContain(lit, this->Negatives));
}
void Variable::Add(list <Literal *>::const_iterator litIter)
{
	Literal * lit = *litIter;
	assert(lit);
	assert(lit->Contains(this));
	assert(lit->clause);
	assert(!lit->IsActive());
	if(lit->GetType())
	{
		assert(doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));

		this->Positives.push_back(lit);
		lit->SetListPointer((--this->Positives.end()), true);
		this->InactivePositives.erase(litIter);

		assert(!doesListContain(lit, this->InactivePositives));
		assert(doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));
	}
	else
	{
		assert(!doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		assert(doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));

		this->Negatives.push_back(lit);
		lit->SetListPointer((--this->Negatives.end()), true);
		this->InactiveNegatives.erase(litIter);

		assert(!doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(doesListContain(lit, this->Negatives));
	}
	assert(lit->IsActive());
}
void Variable::Subtract(list <Literal *>::const_iterator litIter)
{
	Literal * lit = *litIter;
	assert(lit);
	assert(lit->Contains(this));
	assert(lit->clause);
	assert(lit->IsActive());
	if(lit->GetType())
	{
		assert(!doesListContain(lit, this->InactivePositives));
		assert(doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));

		this->InactivePositives.push_back(lit);
		lit->SetListPointer(--this->InactivePositives.end(), false);
		this->Positives.erase(litIter);

		assert(doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));
	}
	else
	{
		assert(!doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->InactiveNegatives));
		assert(doesListContain(lit, this->Negatives));

		this->InactiveNegatives.push_back(lit);
		lit->SetListPointer(--this->InactiveNegatives.end(), false);
		this->Negatives.erase(litIter);

		assert(!doesListContain(lit, this->InactivePositives));
		assert(!doesListContain(lit, this->Positives));
		assert(doesListContain(lit, this->InactiveNegatives));
		assert(!doesListContain(lit, this->Negatives));
	}
	assert(!lit->IsActive());
}
bool Variable::HasSolution() const
{
	if (this->getCurrentState()->getSmallestNegativeClauseSize() == 0 || this->getCurrentState()->getSmallestPositiveClauseSize() == 0)
	{
		return false;
	}
	else if (this->getCurrentState()->getSmallestNegativeClauseSize() == 1 && this->getCurrentState()->getSmallestPositiveClauseSize() == 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool Variable::HasOppositeSolution() const
{
	return true;
}
bool Variable::IsEvaluated() const
{
	//assert(this->isActive != this->isEvaluated);
	return this->isEvaluated;
}
/*
Removes the last state of the variable
*/
#ifdef KEEP_VARIABLE_HISTORY
void Variable::StepBack()
{
	assert(this->PreviousStates.size() > 0);
	#ifdef _DEBUG
		unsigned int previousSize = this->PreviousStates.size();
	#endif
	delete *this->PreviousStates.crbegin();
	this->PreviousStates.pop_back();
	#ifdef _DEBUG
		assert(this->PreviousStates.size() == previousSize - 1);
	#endif
}
#endif
/*
Flips the variable to the opposite value
*/
bool Variable::Flip()
{
	assert(this->isEvaluated == true);
	assert(this->isActive == false);

#ifdef _DEBUG
	bool isPositive = this->WentPositive;
#endif
	this->_Remove(!this->_Undo());
#ifdef _DEBUG
	assert(this->WentPositive != isPositive);
#endif
	
	assert(this->isEvaluated == true);
	assert(this->isActive == false);

	return this->WentPositive;
}
bool Variable::_Undo() 
{
	assert(this->isEvaluated == true);
	this->isEvaluated = false;

	for (list <Literal *>::const_iterator iter = this->InactivePositives.cbegin(); iter != this->InactivePositives.cend();)
	{
		Literal * lit = *iter;
		iter++;
		assert(lit->Contains(this));
		assert(!lit->IsActive());
		lit->Undo();
		assert(lit->IsActive() == lit->clause->IsActive());
	}
	for (list <Literal *>::const_iterator iter = this->InactiveNegatives.cbegin(); iter != this->InactiveNegatives.cend();)
	{
		Literal * lit = *iter;
		iter++;
		assert(lit->Contains(this));
		assert(!lit->IsActive());
		lit->Undo();
		assert(lit->IsActive() == lit->clause->IsActive());
	}

	assert(this->isEvaluated == false);
	return this->WentPositive;
}
void Variable::_Remove(bool isPositive) 
{
	assert(!this->isEvaluated);
	this->isEvaluated = true;

	if (isPositive)
	{
		while (this->Positives.size() > 0)
		{
			Literal * lit = *this->Positives.cbegin();
			assert(lit->Contains(this));
			assert(lit->IsActive());
			lit->RemoveFromClause();
			assert(!lit->IsActive());
			assert(!lit->clause->IsActive());
		}
		while (this->Negatives.size() > 0)
		{
			Literal * lit = *this->Negatives.cbegin();
#ifdef _DEBUG
			unsigned int clauseSize = lit->clause->Size();
#endif
			assert(lit->Contains(this));
			assert(lit->IsActive());
			lit->RemoveFromVariable();
			assert(!lit->IsActive());
#ifdef _DEBUG
			assert(lit->clause->Size() == clauseSize - 1);
#endif
		}

		assert(this->Positives.size() == 0);
		assert(this->Negatives.size() == 0);

		this->WentPositive = true;
	}
	else
	{
		while (this->Positives.size() > 0)
		{
			Literal * lit = *this->Positives.cbegin();
#ifdef _DEBUG
			unsigned int clauseSize = lit->clause->Size();
#endif
			assert(lit->Contains(this));
			assert(lit->IsActive());
			lit->RemoveFromVariable();
			assert(!lit->IsActive());
#ifdef _DEBUG
			assert(lit->clause->Size() == clauseSize - 1);
#endif
		}
		while (this->Negatives.size() > 0)
		{
			Literal * lit = *this->Negatives.cbegin();
			assert(lit->Contains(this));
			assert(lit->IsActive());
			lit->RemoveFromClause();
			assert(!lit->IsActive());
			assert(!lit->clause->IsActive());
		}

		assert(this->Positives.size() == 0);
		assert(this->Negatives.size() == 0);

		this->WentPositive = false;
	}

	assert(this->WentPositive == isPositive);
	assert(this->isEvaluated == true);
}
bool Variable::Undo()
{
	assert(this->isActive == false);

	this->_Undo();
	this->_parent->Remove(this->listPointer, true);

	assert(this->isActive == true);

	return this->WentPositive;
}
void Variable::Remove(bool isPositive)
{
	assert(this->isActive == true);
	
	this->_Remove(isPositive);
	this->_parent->Remove(this->listPointer, false);

	assert(this->isActive == false);
}
#ifdef KEEP_VARIABLE_HISTORY
void Variable::Set()
{
	if (this->CurrentState != NULL) {
		this->PreviousStates.push_back(this->CurrentState);
	}
}
#endif
void Variable::Update()
{
#ifdef KEEP_VARIABLE_HISTORY
	if (this->CurrentState != NULL && (*this->PreviousStates.crbegin()) != this->CurrentState) {
		delete this->CurrentState;
	}
	this->CurrentState = new VariableState(this);
#else
	this->CurrentState->Reset(this);
#endif
}


void Variable::SetListPointer(list <Variable *>::const_iterator var, bool isActive)
{
	assert(*var == this);
	this->listPointer = var;
	this->isActive = isActive;
}


//******************************
//------------------------------
//
// OPERATIONS
//
//------------------------------
//******************************

int Variable::GetVariable() const
{
	assert(this->Variable_Number > 0);
	return this->Variable_Number;
}

bool Variable::IsActive() const
{
	//assert(this->isActive != this->isEvaluated);
	return this->isActive;
}

int Variable::GetValue() const
{
	assert(this->Variable_Number > 0);
	if(this->isEvaluated)
	{
		return WentPositive ? this->Variable_Number : (-1 * this->Variable_Number);
	}
	else
	{
		return 0;
	}
}
VariableState * Variable::getCurrentState() const {
#ifdef _DEBUG
	assert((*this->CurrentState) == VariableState(this));
#endif
	return this->CurrentState;
}


//******************************
//------------------------------
//
// OPERATOR OVERLOADS
//
//------------------------------
//******************************
bool Variable::operator==(const Variable & variable) const
{
	return this->Variable_Number == variable.Variable_Number;
}
bool Variable::operator!=(const Variable & variable) const
{
	return this->Variable_Number != variable.Variable_Number;
}
bool Variable::operator<(const Variable & variable) const
{
	return this->Variable_Number < variable.Variable_Number;
}
bool Variable::operator>(const Variable & variable) const
{
	return this->Variable_Number > variable.Variable_Number;
}
bool Variable::operator<=(const Variable & variable) const
{
	return this->Variable_Number <= variable.Variable_Number;
}
bool Variable::operator>=(const Variable & variable) const
{
	return this->Variable_Number >= variable.Variable_Number;
}