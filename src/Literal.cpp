#include "Variable.h"
#include "Literal.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"


//******************************
//------------------------------
//
// CONSTRUCTORS
//
//------------------------------
//******************************
Literal::Literal(Variable * Variable, Clause * clause, const bool isPositive)
{
#ifdef _DEBUG
	this->isDestroying = false;
#endif
	assert(Variable != NULL);
	assert(clause != NULL);

	this->variable = Variable;
	this->clause = clause;
	this->Value = isPositive
		? this->variable->GetVariable()
		: (-1 * this->variable->GetVariable());

	assert(this->Value != NULL);

	this->variable->Add(this);
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
void Literal::SetListPointer(list <Literal *>::const_iterator lit)
{
	assert(*lit == this);
	this->listPointer = lit;
}
//******************************
//------------------------------
//
// COMPARORS
//
//------------------------------
//******************************
bool Literal::Contains(const Variable * Variable) const
{
	assert(Variable != NULL);
	assert(this->variable != NULL);
	return Variable->GetVariable() == this->Value || (-1 * Variable->GetVariable()) == this->Value;
}
bool Literal::Contains(const Variable * Variable, const bool isPositive) const
{
	assert(Variable != NULL);
	assert(this->variable != NULL);
	return isPositive
		? Variable->GetVariable() == this->Value
		: (-1 * Variable->GetVariable()) == this->Value;
}
bool Literal::Opposite(const Variable * Variable, const bool isPositive) const
{
	assert(Variable != NULL);
	assert(this->variable != NULL);
	return !isPositive
		? Variable->GetVariable() == this->Value
		: (-1 * Variable->GetVariable()) == this->Value;
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

bool Literal::Contains(const int& lit) const
{
	assert(lit != 0);
	return this->Value == lit;
}
bool Literal::Opposite(const int& lit) const
{
	assert(lit != 0);
	return (-1 * this->Value) == lit;
}
//******************************
//------------------------------
//
// OPERATORS
//
//------------------------------
//******************************
unsigned int Literal::getIdentifier() const
{
	return (unsigned int)(void *)this;
}
int Literal::getValue() const
{
	return this->Value;
}
bool Literal::GetType() const
{
	return this->Value > 0;
}
const Clause * Literal::getClause() const
{
	return this->clause;
}
const Variable * Literal::getVariable() const
{
	return this->variable;
}
unsigned int Literal::ClauseSize() const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(this->clause->Size());
	return this->clause->Size();
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
	assert(lit.Value != 0);
	return *this == lit.Value;
}
bool Literal::operator!=(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	assert(lit.Value != 0);
	return *this != lit.Value;
}
bool Literal::operator<(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	assert(lit.Value != 0);
	return *this < lit.Value;
}
bool Literal::operator>(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	assert(lit.Value != 0);
	return *this > lit.Value;
}
bool Literal::operator<=(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	assert(lit.Value != 0);
	return *this <= lit.Value;
}
bool Literal::operator>=(const Literal & lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit.variable != NULL);
	assert(lit.clause != NULL);
	assert(lit.Value != 0);
	return *this >= lit.Value;
}
bool Literal::operator==(const int& lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit != NULL);
	int var1 = this->Value < 0
		? (-1 * this->Value)
		: this->Value;
	int var2 = lit < 0
		? (-1 * lit)
		: lit;
	return var1 == var2;
}
bool Literal::operator!=(const int& lit) const
{
	return !(*this == lit);
}
bool Literal::operator<(const int& lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit != NULL);
	int var1 = this->Value < 0
		? (-1 * this->Value)
		: this->Value;
	int var2 = lit < 0
		? (-1 * lit)
		: lit;
	return var1 < var2;
}
bool Literal::operator>(const int& lit) const
{
	assert(this->clause != NULL);
	assert(this->clause->Contains(this));
	assert(lit != NULL);
	int var1 = this->Value < 0
		? (-1 * this->Value)
		: this->Value;
	int var2 = lit < 0
		? (-1 * lit)
		: lit;
	return var1 > var2;
}
bool Literal::operator<=(const int& lit) const
{
	return !(*this > lit);
}
bool Literal::operator>=(const int& lit) const
{
	return !(*this < lit);
}