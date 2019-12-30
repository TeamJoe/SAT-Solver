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
	this->clause = NULL;
	this->_parent = _parent;

	if(clause->size() > 0)
	{
		this->clause = new Literal * [clause->size() + 1];
	}

	assert(clause->size() > 0);
	assert(this->clause != NULL);

	this->_size = 0;
	//TODO beter sorting method
	for(list <int>::const_iterator iter = clause->cbegin(); iter != clause->cend(); iter++)
	{
		assert(*iter != 0);
		//check for duplicates and opposites
		//insert literals in numeric order
		unsigned int y = 0;
		for(; y < this->_size; y++)
		{
			//duplicate
			if(this->clause[y]->Contains(*iter))
			{
				y = 0xFFFFFFFF;
				break;
			}
			//opposite (Clause will always be true, so remove it)
			else if(this->clause[y]->Opposite(*iter))
			{
				for (; this->_size > 0; this->_size--)
				{
					delete this->clause[this->_size - 1];
				}
				assert(this->_size == 0);
				_parent->clauses->push_back(this);
				this->listPointer = (--_parent->clauses->cend());
				assert(this == *this->listPointer);
				return;
			}
			else if(*this->clause[y] > *iter)
			{
				break;
			}
		}
		//if unique Variable
		if(y != 0xFFFFFFFF)
		{
			//if needs to be inserted at end
			Literal * lit = createLiteral(*iter);
			if(y == this->_size)
			{
				this->clause[this->_size++] = lit;
				assert(this->clause[this->_size -1]->getClause() == this);
			}
			//if needs to be inserted inside of list
			else
			{
				for(unsigned int z = this->_size++; ; z--)
				{
					assert(z <= this->_size);
					this->clause[z+1] = this->clause[z];
					if(z <= y)
					{
						break;
					}
					assert(z != 0);
				}
				assert(this->clause[y]->getClause() == this);
				this->clause[y] = lit;
			}
		}
	}
	assert(this->_size > 0);

	if (_parent->contains(this)) {
		for (; this->_size > 0; this->_size--)
		{
			delete this->clause[this->_size - 1];
		}
		assert(this->_size == 0);
	}
	_parent->clauses->push_back(this);
	this->listPointer = (--_parent->clauses->cend());
	assert(this == *this->listPointer);
}
Clause::~Clause()
{
	assert(this->clause != NULL);
	assert(this->_parent != NULL);
	for (; this->_size > 0; this->_size--)
	{
		delete this->clause[this->_size - 1];
	}
	assert(this->_size == 0);
	delete [] this->clause;

	this->clause = NULL;
	this->_parent->clauses->erase(this->listPointer);
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