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
Clause::Clause(SAT * _parent)
{
	this->clause = NULL;
	this->_size = 0;
	this->_parent = _parent;
}
Clause::Clause(const list <Literal *> * clause, SAT * _parent)
{
	assert(clause);
	this->clause = NULL;
	this->_parent = _parent;

	this->_size = (unsigned int)clause->size();
	if(this->_size > 0)
	{
		this->clause = new Literal * [this->_size + 1];
	}

	assert(this->_size > 0);
	assert(this->clause != NULL);

	unsigned int i = 0;
	for(list <Literal *>::const_iterator iter = clause->cbegin(); iter != clause->cend(); iter++)
	{
		assert(*iter != NULL);
		(*iter)->clause = this;

		//check for duplicates and opposites
		unsigned int y = 0;
		for(; y < i; y++)
		{
			//duplicate
			if(this->clause[y]->Contains(*iter))
			{
				y = 0xFFFFFFFF;
				delete *iter;
			}
			//opposite
			else if(this->clause[y]->Opposite(*iter))
			{
				this->_size = 0;
				// TODO: Could break, as it will delete here and in SAT
				// Can also cause a problem if the clause has a duplicate as well
				for(list <Literal *>::const_iterator iter = clause->cbegin(); iter != clause->cend(); iter++)
				{
					delete *iter;
				}
				return;
			}
			else if(*this->clause[y] > **iter)
			{
				break;
			}
		}
		//if unique Variable
		if(y != 0xFFFFFFFF)
		{
			assert(y < this->_size);
			//if needs to be inserted at end
			if(y == i)
			{
				this->clause[i++] = *iter;
				(*iter)->Add(this);
				assert(this->clause[i-1]->getClause() == this);
			}
			//if needs to be inserted inside of list
			else
			{
				for(unsigned int z = i; ; z--)
				{
					assert(z <= i);
					assert(z < this->_size);
					this->clause[z+1] = this->clause[z];
					if(z <= y)
					{
						break;
					}
					assert(z != 0);
				}
				(*iter)->Add(this);
				assert(this->clause[y]->getClause() == this);
				this->clause[y] = *iter;
				i++;
			}
		}
	}
	this->_size = i;
	assert(this->_size > 0);
}
Clause::~Clause()
{
	assert(this->clause != NULL);
	delete [] this->clause;
	this->clause = NULL;
	this->_size = 0;
}
void Clause::SetListPointer(list <Clause *>::const_iterator cla)
{
	assert(*cla == this);
	this->listPointer = cla;
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

bool Clause::Evaluate(const int * variables) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	if(this->_size < (unsigned int)variables[0])
	{
		assert(variables[variables[0] + 1] == NULL);
		for(int i = 1; i < (variables[0] + 1); i++)
		{
			for(unsigned int j = 0; j < this->_size; j++)
			{
				assert(this->clause[j]->getClause() == this);
				if(this->clause[j]->getValue() == variables[i])
				{
					return true;
				}
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
				if(this->clause[j]->getValue() == variables[i])
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
			for(unsigned int i = 0; i < this->_size; i++)
			{
				assert(this->clause[i]->getClause() == this);
				if(this->clause[i]->getValue() == *variable_iter)
				{
					return true;
				}
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
				if(this->clause[i]->getValue() == *variable_iter)
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