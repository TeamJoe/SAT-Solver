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
	this->Variable_Number = Variable_Number < 0 ? -1 * Variable_Number : Variable_Number;
	this->Negatives = new list <Literal *>();
	this->Positives = new list <Literal *>();
	this->clauses = new map <unsigned int, Clause *>();
	this->positiveSiblingCount = new map <int, map<unsigned int, Clause*>*>();
	this->negativeSiblingCount = new map <int, map<unsigned int, Clause*>*>();

	if(this->Variable_Number < 0)
	{
		this->Variable_Number *= -1;
	}
	assert(this->Variable_Number > 0);

	this->_parent = _parent;
	this->_parent->add(this);
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

	assert(this->negativeSiblingCount->size() == 0);
	delete this->negativeSiblingCount;
	this->negativeSiblingCount = NULL;

	assert(this->positiveSiblingCount->size() == 0);
	delete this->positiveSiblingCount;
	this->positiveSiblingCount = NULL;

	this->Variable_Number = 0;
	this->_parent->remove(this);
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

		addSiblings(lit, this->positiveSiblingCount);
	}
	else
	{
		this->Negatives->push_back(lit);
		lit->SetListPointer((--this->Negatives->end()));

		assert(!doesListContain(lit, this->Positives));
		assert(doesListContain(lit, this->Negatives));

		addSiblings(lit, this->negativeSiblingCount);
	}

	Clause * clause = lit->clause;
	assert(this->clauses->find(clause->getIdentifier()) == this->clauses->cend());
	(*(this->clauses))[clause->getIdentifier()] = clause;
	assert(this->clauses->find(clause->getIdentifier()) != this->clauses->cend());
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

		removeSiblings(lit, this->positiveSiblingCount);
	}
	else
	{
		assert(this->Negatives != NULL);
		assert(this->Positives == NULL || !doesListContain(lit, this->Positives));
		assert(doesListContain(lit, this->Negatives));
		this->Negatives->erase(litIter);

		removeSiblings(lit, this->negativeSiblingCount);
	}

	assert(this->Positives == NULL || !doesListContain(lit, this->Positives));
	assert(this->Negatives == NULL || !doesListContain(lit, this->Negatives));

	assert(this->clauses != NULL);
	assert(this->clauses->find(lit->getClause()->getIdentifier()) != this->clauses->cend());
	this->clauses->erase(lit->getClause()->getIdentifier());
	assert(this->clauses->find(lit->getClause()->getIdentifier()) == this->clauses->cend());


}

void Variable::addSiblings(Literal* lit, map <int, map<unsigned int, Clause*>*>* sibilings)
{
	Clause* clause = lit->clause;
	assert(clause->_size != 0);
	int value = lit->getValue();
	for (unsigned int i = 0; i < clause->_size; i++)
	{
		int sibling = clause->value[i];
		if (value != sibling) {
			assert(sibling != 0);
			assert(*this != sibling);
			map<int, map<unsigned int, Clause*>*>::iterator iter = sibilings->find(sibling);
			if (iter == sibilings->end())
			{
				map<unsigned int, Clause*>* val = new map<unsigned int, Clause*>();
				val->insert_or_assign(clause->getIdentifier(), clause);
				sibilings->insert_or_assign(sibling, val);
			}
			else
			{
				assert(iter->second->find(clause->getIdentifier()) == iter->second->end());
				iter->second->insert_or_assign(clause->getIdentifier(), clause);
			}
		}
	}
}

void Variable::removeSiblings(Literal* lit, map <int, map<unsigned int, Clause*>*>* sibilings)
{
	Clause* clause = lit->clause;
	assert(clause->_size != 0);
	int value = lit->getValue();
	for (unsigned int i = 0; i < clause->_size; i++)
	{
		int sibling = clause->value[i];
		if (value != sibling) {
			assert(sibling != 0);
			assert(*this != sibling);
			map<int, map<unsigned int, Clause*>*>::iterator iter = sibilings->find(sibling);
			assert(iter != sibilings->end());
			assert(iter->second->size() > 0);
			if (iter->second->size() == 1)
			{
				assert(iter->second->find(clause->getIdentifier()) != iter->second->end());
				delete iter->second;
				sibilings->erase(iter);
			}
			else
			{
				assert(iter->second->find(clause->getIdentifier()) != iter->second->end());
				iter->second->erase(clause->getIdentifier());
				assert(iter->second->find(clause->getIdentifier()) == iter->second->end());
			}
		}
	}
}

//******************************
//------------------------------
//
// OPERATIONS
//
//------------------------------
//******************************
unsigned long Variable::getIdentifier() const
{
	return (unsigned long)(void *)this;
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
unsigned int Variable::getSiblingCount(const Variable* variable) const
{
	int value = 0;
	if (this->positiveSiblingCount->find(variable->Variable_Number) != this->positiveSiblingCount->cend()) {
		value += this->positiveSiblingCount->find(variable->Variable_Number)->second->size();
	}
	if (this->positiveSiblingCount->find(-1 * variable->Variable_Number) != this->positiveSiblingCount->cend()) {
		value += this->positiveSiblingCount->find(-1 * variable->Variable_Number)->second->size();
	}
	if (this->negativeSiblingCount->find(variable->Variable_Number) != this->negativeSiblingCount->cend()) {
		value += this->negativeSiblingCount->find(variable->Variable_Number)->second->size();
	}
	if (this->negativeSiblingCount->find(-1 * variable->Variable_Number) != this->negativeSiblingCount->cend()) {
		value += this->negativeSiblingCount->find(-1 * variable->Variable_Number)->second->size();
	}
	return value;
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