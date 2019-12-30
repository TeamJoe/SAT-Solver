#include "Variable.h"
#include "Variable.h"
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
Variable::Variable(const int Variable_Number, SAT * _parent)
{
	assert(Variable_Number != 0);
	this->Variable_Number = Variable_Number;
	this->Negatives = new list <Literal *>();
	this->Positives = new list <Literal *>();
	this->clauses = new map <unsigned int, Clause *>();

	if(this->Variable_Number < 0)
	{
		this->Variable_Number *= -1;
	}
	assert(this->Variable_Number > 0);

	this->_parent = _parent;

	_parent->variables->push_back(this);
	this->listPointer = (--_parent->variables->cend());
	assert(this == *this->listPointer);
}
Variable* Variable::copy(SAT* _parent) const
{
	assert(this->Positives != NULL);
	assert(this->Negatives != NULL);
	assert(this->clauses != NULL);
	assert(this->Variable_Number != 0);
	return new Variable(this->GetVariable(), _parent);
}
Variable::~Variable()
{
	assert(this->Positives != NULL);
	assert(this->Negatives != NULL);
	assert(this->clauses != NULL);
	assert(this->_parent != NULL);
	assert(this->Variable_Number != 0);

	while(this->Positives->size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->Positives->size();
		#endif
		assert((*(this->Positives->cbegin()))->getVariable() == this);
		assert((*(this->Positives->cbegin()))->listPointer == this->Positives->cbegin());
		delete *this->Positives->cbegin();
		#ifdef _DEBUG
			assert(this->Positives->size() == previousSize - 1);
		#endif
	}
	assert(this->Positives->size() == 0);
	delete this->Positives;
	this->Positives = NULL;

	while(this->Negatives->size() > 0)
	{
		#ifdef _DEBUG
			unsigned int previousSize = this->Negatives->size();
		#endif
		assert((*(this->Negatives->cbegin()))->getVariable() == this);
		assert((*(this->Negatives->cbegin()))->listPointer == this->Negatives->cbegin());
		delete *this->Negatives->cbegin();
		#ifdef _DEBUG
			assert(this->Negatives->size() == previousSize - 1);
		#endif
	}
	assert(this->Negatives->size() == 0);
	delete this->Negatives;
	this->Negatives = NULL;

	assert(this->clauses->size() == 0);
	delete this->clauses;
	this->clauses = NULL;

	this->Variable_Number = 0;

	this->_parent->variables->erase(this->listPointer);
}
//a debugging sanity check function
bool doesListContain(Literal * lit, const list <Literal *> * search)
{
	for(list <Literal *>::const_iterator i = search->cbegin(); i != search->cend(); i++)
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
	assert(this->Positives != NULL);
	assert(this->Negatives != NULL);
	assert(this->clauses != NULL);

	assert(lit);
	assert(lit->getVariable() == this);
	assert(lit->Contains(this, true) || lit->Contains(this, false));

	assert(!doesListContain(lit, this->Positives));
	assert(!doesListContain(lit, this->Negatives));
	if(lit->GetType())
	{
		this->Positives->push_back(lit);
		lit->SetListPointer((--this->Positives->end()));

		assert(doesListContain(lit, this->Positives));
		assert(!doesListContain(lit, this->Negatives));
	}
	else
	{
		this->Negatives->push_back(lit);
		lit->SetListPointer((--this->Negatives->end()));

		assert(!doesListContain(lit, this->Positives));
		assert(doesListContain(lit, this->Negatives));
	}

	Clause * clause = lit->clause;
	assert(this->clauses->find(clause->getIdentifier()) == this->clauses->cend());
	(*(this->clauses))[clause->getIdentifier()] = clause;
	assert(this->clauses->find(clause->getIdentifier()) != this->clauses->cend());

	//assert(clause->clause[clause->_size - 1] == lit);
	if (clause->_size != 0) {
		for (unsigned int i = 0; i < clause->_size; i++)
		{
			Literal* l2 = clause->clause[i];
			assert(lit != l2);

			addSibling(lit, l2);
			addSibling(l2, lit);
		}
	}
}
void Variable::Remove(list <Literal *>::const_iterator& litIter)
{
	Literal * lit = *litIter;
	assert(lit);
	assert(lit->Contains(this));
	assert(lit->getClause());
	if(lit->GetType())
	{
		assert(this->Positives != NULL);
		assert(this->Negatives == NULL || !doesListContain(lit, this->Negatives));
		assert(doesListContain(lit, this->Positives));
		this->Positives->erase(litIter);
	}
	else
	{
		assert(this->Negatives != NULL);
		assert(this->Positives == NULL || !doesListContain(lit, this->Positives));
		assert(doesListContain(lit, this->Negatives));
		this->Negatives->erase(litIter);
	}

	assert(this->Positives == NULL || !doesListContain(lit, this->Positives));
	assert(this->Negatives == NULL || !doesListContain(lit, this->Negatives));

	assert(this->clauses != NULL);
	assert(this->clauses->find(lit->getClause()->getIdentifier()) != this->clauses->cend());
	this->clauses->erase(lit->getClause()->getIdentifier());
	assert(this->clauses->find(lit->getClause()->getIdentifier()) == this->clauses->cend());

	Clause* clause = lit->clause;
	assert(clause->_size != 0);
	assert(clause->clause[clause->_size - 1] == lit);
	for (unsigned int i = 0; i < clause->_size - 1; i++)
	{
		Literal* l2 = clause->clause[i];
		assert(lit != l2);

		removeSibling(lit, l2);
		removeSibling(l2, lit);
	}
}

void Variable::addSibling(Literal* l1, Literal* l2)
{
	int value = l2->getValue() * (l1->GetType() ? 1 : -1);
	map<int, unsigned int>::iterator i1 = l1->variable->siblingCount.find(value);
	l1->variable->siblingCount.insert_or_assign(value, i1 == l1->variable->siblingCount.end() ? 1 : (i1->second + 1));
}
void Variable::removeSibling(Literal* l1, Literal* l2)
{
	int value = l2->getValue() * (l1->GetType() ? 1 : -1);
	map<int, unsigned int>::iterator i1 = l1->variable->siblingCount.find(value);
	assert(i1 != l1->variable->siblingCount.end());
	assert(i1->second > 0);
	l1->variable->siblingCount.insert_or_assign(value, i1->second - 1);
}

//******************************
//------------------------------
//
// OPERATIONS
//
//------------------------------
//******************************
unsigned int Variable::getIdentifier() const
{
	return (unsigned int)(void *)this;
}
int Variable::GetVariable() const
{
	assert(this->Variable_Number > 0);
	return this->Variable_Number;
}
bool Variable::isValid() const
{
	return this->Variable_Number > 0;
}

//******************************
//------------------------------
//
// OPERATOR OVERLOADS
//
//------------------------------
//******************************
bool Variable::operator==(const Variable & Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable.Variable_Number > 0);
	return this->Variable_Number == Variable.Variable_Number;
}
bool Variable::operator!=(const Variable & Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable.Variable_Number > 0);
	return this->Variable_Number != Variable.Variable_Number;
}
bool Variable::operator<(const Variable & Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable.Variable_Number > 0);
	return this->Variable_Number < Variable.Variable_Number;
}
bool Variable::operator>(const Variable & Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable.Variable_Number > 0);
	return this->Variable_Number > Variable.Variable_Number;
}
bool Variable::operator<=(const Variable & Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable.Variable_Number > 0);
	return this->Variable_Number <= Variable.Variable_Number;
}
bool Variable::operator>=(const Variable & Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable.Variable_Number > 0);
	return this->Variable_Number >= Variable.Variable_Number;
}
bool Variable::operator==(const int& Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable != 0);
	return this->Variable_Number == (Variable < 0 ? -1 * Variable : Variable);
}
bool Variable::operator!=(const int& Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable != 0);
	return this->Variable_Number != (Variable < 0 ? -1 * Variable : Variable);
}
bool Variable::operator<(const int& Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable != 0);
	return this->Variable_Number < (Variable < 0 ? -1 * Variable : Variable);
}
bool Variable::operator>(const int& Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable != 0);
	return this->Variable_Number > (Variable < 0 ? -1 * Variable : Variable);
}
bool Variable::operator<=(const int& Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable != 0);
	return this->Variable_Number <= (Variable < 0 ? -1 * Variable : Variable);
}
bool Variable::operator>=(const int& Variable) const
{
	assert(this->Variable_Number > 0);
	assert(Variable != 0);
	return this->Variable_Number >= (Variable < 0 ? -1 * Variable : Variable);
}