#include "Variable.h"
#include "Literal.h"

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
Literal::Literal(Variable * variable, Clause * clause, const bool isPositive)
{
#ifdef _DEBUG
	this->isDestroying = false;
#endif
	assert(variable != NULL);

	this->variable = variable;
	this->clause = clause;
	this->Value = isPositive
		? this->variable->GetVariable()
		: (-1 * this->variable->GetVariable());
	this->variable->Add(this);

	assert(this->Value != NULL);
}
Literal::~Literal()
{
#ifdef _DEBUG
	this->isDestroying = true;
#endif
	assert(this->Value != 0);
	assert(this->clause != NULL);
	assert(this->variable != NULL);
	//assert(this->clause->Contains(this));
	Variable * var = this->variable;
	var->Remove(this->listPointer);
	this->variable = NULL;
	this->clause = NULL;
	this->Value = 0;
}
Literal * Literal::copy(const list<Variable *> * variables, Clause * clause) const
{
	Variable * var = NULL;
	for (list <Variable *>::const_iterator variable_iter = variables->cbegin(); variable_iter != variables->cend(); variable_iter++)
	{
		if ((*variable_iter)->GetVariable() == this->variable->GetVariable())
		{
			var = (*variable_iter);
			break;
		}
	}
	this->variable->GetVariable();
	return new Literal(var, clause, this->GetType());
}

//******************************
//------------------------------
//
// COMPARORS
//
//------------------------------
//******************************
bool Literal::Contains(const Variable * variable) const
{
	assert(variable != NULL);
	assert(this->variable != NULL);
	return variable->GetVariable() == this->Value || (-1 * variable->GetVariable()) == this->Value;
}
bool Literal::Contains(const Variable * variable, const bool isPositive) const
{
	assert(variable != NULL);
	assert(this->variable != NULL);
	return isPositive
		? variable->GetVariable() == this->Value
		: (-1 * variable->GetVariable()) == this->Value;
}
bool Literal::Opposite(const Variable * variable, const bool isPositive) const
{
	assert(variable != NULL);
	assert(this->variable != NULL);
	return !isPositive
		? variable->GetVariable() == this->Value
		: (-1 * variable->GetVariable()) == this->Value;
}
bool Literal::Contains(const Literal * lit) const
{
	assert(lit != NULL);
	return this->Value == lit->Value;
}
bool Literal::Opposite(const Literal * lit) const
{
	assert(lit != NULL);
	return (-1 * this->Value) == lit->Value;
}
//******************************
//------------------------------
//
// OPERATORS
//
//------------------------------
//******************************
bool Literal::GetType() const
{
	return this->Value > 0;
}
bool Literal::IsActive() const
{
	return this->isActive;
}
unsigned int Literal::IntialClauseSize() const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(this->clause->IntialSize());
	return this->clause->IntialSize();
}
unsigned int Literal::ClauseSize() const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(this->clause->Size());
	return this->clause->Size();
}

bool Literal::IsEvaluted() const
{
	assert(this->variable != NULL);
	assert(!this->variable->IsEvaluated() || this->variable->GetValue() == this->Value || this->variable->GetValue() == (-1 * this->Value));
	return this->variable->IsEvaluated();
}
bool Literal::IsTrue() const
{
	assert(this->variable != NULL);
	assert(!this->variable->IsEvaluated() || this->variable->GetValue() == this->Value || this->variable->GetValue() == (-1 * this->Value));
	return this->variable->IsEvaluated() && this->variable->GetValue() == this->Value;
}
bool Literal::IsFalse() const
{
	assert(this->variable != NULL);
	assert(!this->variable->IsEvaluated() || this->variable->GetValue() == this->Value || this->variable->GetValue() == (-1 * this->Value));
	return this->variable->IsEvaluated() && this->variable->GetValue() != this->Value;
}
//******************************
//------------------------------
//
// MODIFIERS
//
//------------------------------
//******************************
void Literal::Undo()
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	this->clause->Undo();
	assert(this->clause->IsActive() == this->IsActive());
}

void Literal::ReAddToVariable()
{
	assert(this->variable != NULL);
	if(!this->isActive)
	{
		this->variable->Add(this->listPointer);
	}
}

void Literal::RemoveFromClause()
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	this->clause->Remove();
	assert(!this->IsActive());
}

void Literal::RemoveFromVariable()
{
	assert(this->variable != NULL);
	if(this->isActive)
	{
		this->variable->Subtract(this->listPointer);
	}
	assert(!this->IsActive());
}

void Literal::SetListPointer(list <Literal *>::const_iterator lit, bool isActive)
{
	assert(*lit == this);
	this->listPointer = lit;
	this->isActive = isActive;
}

//******************************
//------------------------------
//
// OPERATOR OVERLOADS
//
//------------------------------
//******************************
bool Literal::operator==(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	int var1 = this->Value < 0 
		? (-1 * this->Value)
		: this->Value;
	int var2 = lit.Value < 0 
		? (-1 * lit.Value)
		: lit.Value;
	return var1 == var2;
}
bool Literal::operator!=(const Literal & lit) const
{
	return !(*this == lit);
}
bool Literal::operator<(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	int var1 = this->Value < 0 
		? (-1 * this->Value)
		: this->Value;
	int var2 = lit.Value < 0 
		? (-1 * lit.Value)
		: lit.Value;
	return var1 < var2;
}
bool Literal::operator>(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	int var1 = this->Value < 0 
		? (-1 * this->Value)
		: this->Value;
	int var2 = lit.Value < 0 
		? (-1 * lit.Value)
		: lit.Value;
	return var1 > var2;
}
bool Literal::operator<=(const Literal & lit) const
{
	return !(*this > lit);
}
bool Literal::operator>=(const Literal & lit) const
{
	return !(*this < lit);
}