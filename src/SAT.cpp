#include <list>
#include <string>
#include <fstream>

#include "Variable.h"
#include "Clause.h"
#include "SAT.h"
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
SAT::SAT()
{
	this->variables = new list <Variable*>();
	this->clauses = new list <Clause*>();
}
SAT::~SAT()
{
	cleanClauses();
	cleanVariables();
	this->clauses = NULL;
	this->variables = NULL;
}
SAT* SAT::copy() const
{
	SAT* sat = new SAT();
	assert(this->variables != NULL);
	assert(this->clauses != NULL);
	assert(sat->variables != NULL);
	assert(sat->clauses != NULL);
	for (list <Variable*>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		assert((*iter)->isValid());
		Variable* variable = (*iter)->copy(sat);
		assert(variable != NULL);
		assert(variable->_parent == sat);
		assert(variable->isValid());
	}
	for (list <Clause*>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		assert((*iter)->isValid());
		Clause* clause = (*iter)->copy(sat);
		assert(clause != NULL);
		assert(clause->_parent == sat);
		assert(clause->isValid());
	}
	return sat;
}
void SAT::cleanVariables()
{
	assert(this->variables != NULL);
	while (this->variables->size() > 0)
	{
#ifdef _DEBUG
		unsigned int previousSize = this->variables->size();
#endif
		assert((*(this->variables->cbegin()))->_parent == this);
		assert((*(this->variables->cbegin()))->listPointer == this->variables->cbegin());
		delete* this->variables->cbegin();
#ifdef _DEBUG
		assert(this->variables->size() == previousSize - 1);
#endif
	}
	assert(this->variables->size() == 0);
	delete this->variables;
}
void SAT::cleanClauses()
{
	assert(this->clauses != NULL);
	while (this->clauses->size() > 0)
	{
#ifdef _DEBUG
		unsigned int previousSize = this->clauses->size();
#endif
		assert((*(this->clauses->cbegin()))->_parent == this);
		assert((*(this->clauses->cbegin()))->listPointer == this->clauses->cbegin());
		delete* this->clauses->cbegin();
#ifdef _DEBUG
		assert(this->clauses->size() == previousSize - 1);
#endif
	}
	assert(this->clauses->size() == 0);
	delete this->clauses;
}
bool SAT::add(Variable* variable)
{
	assert(variable != NULL);
	assert(this->variables != NULL);
	assert(variable->_parent == this);
	assert(!variable->isValid() || !this->contains(variable));
	this->variables->push_back(variable);
	variable->listPointer = (--this->variables->cend());
	assert(variable == *variable->listPointer);
	return true;
}
bool SAT::add(Clause* clause)
{
	assert(clause != NULL);
	assert(this->clauses != NULL);
	assert(clause->_parent == this);
	assert(!clause->isValid() || !this->contains(clause));
	this->clauses->push_back(clause);
	clause->listPointer = (--this->clauses->cend());
	assert(clause == *clause->listPointer);
	return true;
}
bool SAT::remove(Variable* variable)
{
	assert(variable != NULL);
	assert(this->variables != NULL);
	assert(variable->_parent == this);
	assert(!variable->isValid() || this->contains(variable));
	assert(variable == *variable->listPointer);
	this->variables->erase(variable->listPointer);
	return true;
}
bool SAT::remove(Clause* clause)
{
	assert(clause != NULL);
	assert(this->clauses != NULL);
	assert(clause->_parent == this);
	assert(!clause->isValid() || this->contains(clause));
	assert(clause == *clause->listPointer);
	this->clauses->erase(clause->listPointer);
	return true;
}
//******************************
//------------------------------
//
// PRIVATE FUNCTIONS
//
//------------------------------
//******************************
Clause * SAT::getClause(const Clause * clause) const
{
	assert(this->clauses != NULL);
	for(list <Clause *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		if(**iter == *clause)
		{
			return *iter;
		}
	}
	return NULL;
}
Variable * SAT::getVariable(const int& variable) const
{
	assert(this->variables != NULL);
	for(list <Variable *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if(**iter == variable)
		{
			return *iter;
		}
	}
	return NULL;
}
Variable * SAT::getVariable(const Variable * variable) const
{
	assert(this->variables != NULL);
	for(list <Variable *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if(**iter == *variable)
		{
			return *iter;
		}
	}
	return NULL;
}

Variable* SAT::getOrCreateVariable(const int& var)
{
	assert(var != 0);
	Variable* v = getVariable((var < 0) ? (-1 * var) : var);
	if (v == NULL)
	{
		v = new Variable(var, this);
	}
	return v;
}

bool SAT::contains(const Clause* clause) const
{
	return getClause(clause) != NULL;
}

bool SAT::contains(const int& variable) const
{
	return getVariable(variable) != NULL;
}

bool SAT::contains(const Variable* variable) const
{
	return getVariable(variable) != NULL;
}

bool SAT::addVariable(const int& variable)
{
	assert(this->variables != NULL);
	assert(this->clauses != NULL);
	assert(variable != 0);
	Variable* var = new Variable(variable, this);
#ifdef _DEBUG
	this->variables->sort();
#endif
	assert(var->_parent == this);
	if (var->isValid())
	{
		return true;
	}
	else
	{
		delete var;
		return false;
	}
	return false;
}

bool SAT::addClause(const list <int>* clause)
{
	assert(this->variables != NULL);
	assert(this->clauses != NULL);
	assert(clause != NULL);
	if (clause->size() > 0) {
		Clause* cla = new Clause(clause, this);
#ifdef _DEBUG
		this->clauses->sort();
#endif
		assert(cla->_parent == this);
		if (cla->isValid())
		{
			return true;
		}
		else
		{
			delete cla;
			return false;
		}
	}
	return false;
}

//******************************
//------------------------------
//
// CHECKER FUNCTIONS
//
//------------------------------
//******************************
unsigned int SAT::getIdentifier() const
{
	return (unsigned int)(void *)this;
}
bool SAT::Evaluate(const int * variables) const
{
	assert(this->clauses != NULL);
	for(list <Clause *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		if(!(*iter)->Evaluate(variables))
		{
			return false;
		}
	}
	return true;
}
bool SAT::Evaluate(const list <int> * variables) const
{
	assert(this->clauses != NULL);
	for(list <Clause *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		if(!(*iter)->Evaluate(variables))
		{
			return false;
		}
	}
	return true;
}

//******************************
//------------------------------
//
// SOLVER FUNCTIONS
//
//------------------------------
//******************************
unsigned int SAT::ClauseCount() const
{
	assert(this->clauses != NULL);
	return this->clauses->size();
}

unsigned int SAT::VariableCount() const
{
	assert(this->variables != NULL);
	return this->variables->size();
}
//******************************
//------------------------------
//
// OPERATOR OVERLOADS
//
//------------------------------
//******************************
bool SAT::operator==(const SAT& sat) const
{
	assert(this->variables != NULL);
	assert(this->clauses != NULL);
	assert(sat.variables != NULL);
	assert(sat.clauses != NULL);
	if (this->variables->size() != sat.variables->size()
		|| this->clauses->size() != sat.clauses->size()) {
		return false;
	}
	for (list <Variable*>::const_iterator iter = sat.variables->cbegin(); iter != sat.variables->cend(); iter++)
	{
		assert((*iter)->isValid());
		if (!this->contains(*iter)) {
			return false;
		}
	}
	for (list <Clause*>::const_iterator iter = sat.clauses->cbegin(); iter != sat.clauses->cend(); iter++)
	{
		assert((*iter)->isValid());
		if (!this->contains(*iter)) {
			return false;
		}
	}
	return true;
}
bool SAT::operator!=(const SAT& sat) const
{
	return !(*this == sat);
}