#include <list>
#include "Clause.h"

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
		//if unique variable
		if(y != 0xFFFFFFFF)
		{
			assert(y < this->_size);
			//if needs to be inserted at end
			if(y == i)
			{
				this->clause[i++] = *iter;
				assert(this->clause[i-1]->clause == this);
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
				assert(this->clause[y]->clause == this);
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
Clause * Clause::copy(const list<Variable *> * vars, SAT * _parent) const
{
	Clause * clause = new Clause(_parent);
	clause->_size = this->_size;
	if (clause->_size > 0)
	{
		clause->clause = new Literal * [clause->_size + 1];
	}
	for (unsigned int i = 0; i < clause->_size; i++)
	{
		clause->clause[i] = this->clause[i]->copy(vars, clause);
		assert(clause->clause[i] != this->clause[i]);
		assert((*clause->clause[i]) == (*this->clause[i]));
	}
	assert(clause->_size = this->_size);
	return clause;
}
//******************************
//------------------------------
//
// OPERATIONS
//
//------------------------------
//******************************
unsigned int Clause::IntialSize() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	return this->_size;
}
unsigned int Clause::Size() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	unsigned int size = 0;
	for(unsigned int i = 0; i < this->_size; i++)
	{
		if(this->clause[i]->IsActive())
		{
			size++;
		}
	}
	return size;
}
bool Clause::Contains(const Variable * variable) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(this->clause[i]->Contains(variable))
		{
			return true;
		}
	}
	return false;
}
bool Clause::Contains(const Variable * variable, const bool isPositive) const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(this->clause[i]->Contains(variable, isPositive))
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
		//assert(this->clause[i]->clause == this);
		if(this->clause[i]->Contains(lit))
		{
			return true;
		}
	}
	return false;
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
				assert(this->clause[i]->clause == this);
				if(this->clause[i]->Value == *variable_iter)
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
				assert(this->clause[i]->clause == this);
				if(this->clause[i]->Value == *variable_iter)
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

bool Clause::AllEvaluted() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(!this->clause[i]->IsEvaluted())
		{
			return false;
		}
	}
	return true;
}
bool Clause::HasEvaluted() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(this->clause[i]->IsEvaluted())
		{
			return true;
		}
	}
	return false;
}
bool Clause::AllTrue() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(!this->clause[i]->IsTrue())
		{
			return false;
		}
	}
	return true;
}
bool Clause::AllFalse() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(!this->clause[i]->IsFalse())
		{
			return false;
		}
	}
	return true;
}
bool Clause::HasTrue() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(this->clause[i]->IsTrue())
		{
			return true;
		}
	}
	return false;
}
bool Clause::HasFalse() const
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		if(this->clause[i]->IsFalse())
		{
			return true;
		}
	}
	return false;
}
bool Clause::HasSolution() const
{
	return !this->AllFalse();
}
bool Clause::HasOppositeSolution() const
{
	return !this->AllTrue();
}

bool Clause::IsActive() const
{
	return this->isActive;
}

//******************************
//------------------------------
//
// MODIFIERS
//
//------------------------------
//******************************
/*
Adds the clause back to the active set unless a literal in the clause evaluates to true
*/
void Clause::Undo()
{
	assert(this->_size > 0);
	assert(this->clause != NULL);

	//Check if clause is still true
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		assert(this->isActive || !this->clause[i]->IsActive());
		if(this->clause[i]->IsTrue())
		{
			assert(!this->isActive);
			return;
		}
	}

	//Reassign the variables if clause is no longer true
	for(unsigned int i = 0; i < this->_size; i++)
	{
		this->clause[i]->ReAddToVariable();
		assert(this->clause[i]->IsActive());
	}

	if(!this->isActive)
	{
		this->_parent->Remove(this->listPointer, true);
	}
	assert(this->isActive);
}
/*
Removes the clause from the active set
*/
void Clause::Remove()
{
	assert(this->_size > 0);
	assert(this->clause != NULL);
	for(unsigned int i = 0; i < this->_size; i++)
	{
		assert(this->clause[i]->clause == this);
		this->clause[i]->RemoveFromVariable();
		assert(!this->clause[i]->IsActive());
	}

	this->_parent->Remove(this->listPointer, false);
	assert(this->isActive == false);
}

void Clause::SetListPointer(list <Clause *>::const_iterator cla, bool isActive)
{
	assert(*cla == this);
	this->listPointer = cla;
	this->isActive = isActive;
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
			assert(this->clause[i]->clause == this);
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
			assert(this->clause[i]->clause == this);
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
			assert(this->clause[i]->clause == this);
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