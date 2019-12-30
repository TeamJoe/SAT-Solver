#include <list>
#include "Clause.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

//******************************
//------------------------------
//
// CONSTRUCTORS
//
//------------------------------
//******************************
Clause::Clause(const list <int> * clause, SAT * _parent)
{
	assert(clause != NULL);
	assert(_parent != NULL);

	this->_parent = _parent;
	this->_createValue(clause);
	this->clause = new Literal * [this->_size];
#ifdef _DEBUG
	assert(this->_size == this->_value.size());
#endif

	for (unsigned int i = 0; i < this->_size; i++)
	{
		this->clause[i] = this->createLiteral(this->value[i]);
	}

	_parent->clauses->push_back(this);
	this->listPointer = (--_parent->clauses->cend());
	assert(this == *this->listPointer);
}
Clause::~Clause()
{
	assert(this->clause != NULL);
	assert(this->_parent != NULL);
	for (unsigned int i = 0; i < this->_size; i++)
	{
		delete this->clause[i];
	}
	delete [] this->value;
	delete [] this->clause;
	this->_size = 0;
#ifdef _DEBUG
	this->_value.clear();
#endif

	this->clause = NULL;
	assert(this == *this->listPointer);
	this->_parent->clauses->erase(this->listPointer);
}
void Clause::_createValue(const list <int>* clause)
{
	assert(clause != NULL);
	assert(clause->size() > 0);

	if (clause->size() > 0)
	{
		this->value = new int[clause->size() + 1];
	}	

	this->_size = 0;
#ifdef _DEBUG
	this->_value.clear();
#endif
	//TODO beter sorting method
	for (list <int>::const_iterator iter = clause->cbegin(); iter != clause->cend(); iter++)
	{
		assert(*iter != 0);
		//check for duplicates and opposites
		//insert literals in numeric order
		unsigned int y = 0;
		for (; y < this->_size; y++)
		{
			//duplicate
			if (this->value[y] == *iter)
			{
				y = 0xFFFFFFFF;
				break;
			}
			//opposite (Clause will always be true, so remove it)
			else if (this->value[y] == -1 * *iter)
			{
				this->_size = 0;
#ifdef _DEBUG
				this->_value.clear();
#endif
				return;
			}

			int var1 = this->value[y] < 0
				? (-1 * this->value[y])
				: this->value[y];
			int var2 = *iter < 0
				? (-1 * *iter)
				: *iter;
			if (var1 > var2)
			{
				break;
			}
		}
		//if unique Variable
		if (y != 0xFFFFFFFF)
		{
			//if needs to be inserted at end
#ifdef _DEBUG
			this->_value.push_back(*iter);
#endif
			if (y == this->_size)
			{
				this->value[this->_size++] = *iter;
			}
			//if needs to be inserted inside of list
			else
			{
				for (unsigned int z = this->_size++; ; z--)
				{
					assert(z <= this->_size);
					this->value[z + 1] = this->value[z];
					if (z <= y)
					{
						break;
					}
					assert(z != 0);
				}
				this->value[y] = *iter;
			}
		}
	}
	assert(this->_size > 0);
}
Literal* Clause::createLiteral(const int& var)
{
	Variable* v = this->_parent->getOrCreateVariable(var);
	assert(v != NULL);
	return new Literal(v, this, var > 0);
}
Clause* Clause::copy(SAT* _parent) const
{
	list <int> clause;
	for (unsigned int i = 0; i < this->_size; i++)
	{
		clause.push_back(this->clause[i]->getValue());
	}
	return new Clause(&clause, _parent);
}
//******************************
//------------------------------
//
// OPERATIONS
//
//------------------------------
//******************************
unsigned int Clause::getIdentifier() const
{
	return (unsigned int)(void *)this;
}
unsigned int Clause::Size() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	return this->_size;
}
bool Clause::Contains(const Variable * Variable) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->getClause() == this);
		if(this->clause[i]->Contains(Variable))
		{
			return true;
		}
	}
	return false;
}
bool Clause::Contains(const Variable * Variable, const bool isPositive) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->getClause() == this);
		if(this->clause[i]->Contains(Variable, isPositive))
		{
			return true;
		}
	}
	return false;
}
bool Clause::Contains(const Literal * lit) const
{

	//TODO beter lookup method
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->getClause() == this);
		if(this->clause[i]->Contains(lit))
		{
			return true;
		}
	}
	return false;
}

bool Clause::Contains(const int lit) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for (unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->getClause() == this);
		if (this->clause[i]->Contains(lit))
		{
			return true;
		}
	}
	return false;
}

bool Clause::Evaluate(const int * variables) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	if(this->_size < (unsigned int)variables[0])
	{
		assert(variables[variables[0] + 1] == NULL);
		for(int i = 1; i < (variables[0] + 1); i++)
		{
			if (this->Contains(variables[i]))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		assert(variables[variables[0] + 1] == NULL);
		for(unsigned int j = 0; j < this->_size; j++)
		{
			for(int i = 1; i < (variables[0] + 1); i++)
			{
				assert(this->clause[j]->getClause() == this);
				if(this->clause[j]->Contains(variables[i]))
				{
					return true;
				}
			}
		}
		return false;
	}
}
bool Clause::Evaluate(const list <int> * variables) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	if(this->_size < variables->size())
	{
		for(list <int>::const_iterator variable_iter = variables->cbegin(); variable_iter != variables->cend(); variable_iter++)
		{
			if (this->Contains(*variable_iter))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		for(unsigned int i = 0; i < this->_size; i++)
		{
			for(list <int>::const_iterator variable_iter = variables->cbegin(); variable_iter != variables->cend(); variable_iter++)
			{
				assert(this->clause[i]->getClause() == this);
				if (this->clause[i]->Contains(*variable_iter))
				{
					return true;
				}
			}
		}
		return false;
	}
}
bool Clause::isValid() const
{
	return this->_size > 0;
}
//******************************
//------------------------------
//
// OPERATOR OVERLOADS
//
//------------------------------
//******************************
bool Clause::operator==(const Clause & clause) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	assert(clause._size > 0);
	assert(clause.clause != NULL);
	if(this->_size != clause._size)
	{
		return false;
	}
	for(unsigned int i = 0; i < this->_size; i++)
	{
		if(!this->Contains(clause.clause[i]))
		{
			assert(this->clause[i]->getClause() == this);
			return false;
		}
	}
	return true;
}
bool Clause::operator!=(const Clause & clause) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	assert(clause._size > 0);
	assert(clause.clause != NULL);
	return !(*this == clause);
}
bool Clause::operator<(const Clause & clause) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	assert(clause._size > 0);
	assert(clause.clause != NULL);
	if(this->_size != clause._size)
	{
		return this->_size < clause._size;
	}
	for(unsigned int i = 0; i < this->_size; i++)
	{
		if(!this->Contains(clause.clause[i]))
		{
			assert(this->clause[i]->getClause() == this);
			return *this->clause[i] < *clause.clause[i];
		}
	}
	return false;
}
bool Clause::operator>(const Clause & clause) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	assert(clause._size > 0);
	assert(clause.clause != NULL);
	if(this->_size != clause._size)
	{
		return this->_size > clause._size;
	}
	for(unsigned int i = 0; i < this->_size; i++)
	{
		if(!this->Contains(clause.clause[i]))
		{
			assert(this->clause[i]->getClause() == this);
			return *this->clause[i] > *clause.clause[i];
		}
	}
	return false;
}
bool Clause::operator<=(const Clause & clause) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	assert(clause._size > 0);
	assert(clause.clause != NULL);
	return !(*this > clause);
}
bool Clause::operator>=(const Clause & clause) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	assert(clause._size > 0);
	assert(clause.clause != NULL);
	return !(*this < clause);
}