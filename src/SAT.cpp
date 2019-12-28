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
}
void SAT::cleanVariables()
{
	assert(this->variables != NULL);
	for(list <Variable *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		delete *iter;
	}
	this->variables->clear();
	delete this->variables;
	this->variables = NULL;
}
void SAT::cleanClauses()
{
	assert(this->clauses != NULL);
	for(list <Clause *>::const_iterator iter = this->clauses->cbegin(); iter != this->clauses->cend(); iter++)
	{
		delete *iter;
	}
	this->clauses->clear();
	delete this->clauses;
	this->clauses = NULL;
}
//******************************
//------------------------------
//
// PRIVATE FUNCTIONS
//
//------------------------------
//******************************
Clause * SAT::ContainsClause(const Clause * clause) const
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
Variable * SAT::ContainsVariable(const int& variable) const
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
Variable * SAT::ContainsVariable(const Variable * variable) const
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
	Variable* v = ContainsVariable((var < 0) ? (-1 * var) : var);
	if (v == NULL)
	{
		v = new Variable(var, this);
		this->variables->push_back(v);
		v->SetListPointer((--this->variables->cend()));
	}
	return v;
}

bool SAT::addVariable(Variable* variable)
{
	assert(this->variables != NULL);
	assert(variable != NULL);
	assert(variable->GetVariable() != 0);
	assert(variable->GetVariable() > 0);
	if (ContainsVariable(variable->GetVariable() == NULL))
	{
		this->variables->push_back(variable);
		variable->SetListPointer((--this->variables->cend()));
		return true;
	}
	return false;
}

bool SAT::addClause(const list <int>* clause)
{
	assert(this->variables != NULL);
	assert(this->clauses != NULL);
	assert(clause != NULL);
	if (clause->size()) {
		Clause* cla = new Clause(clause, this);
		if (cla->isValid() && !ContainsClause(cla))
		{
			this->clauses->push_back(cla);
			cla->SetListPointer((--this->clauses->cend()));
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