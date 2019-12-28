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
	cleanVariables();
	cleanClauses();
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
Variable * SAT::ContainsVariable(const int variable) const
{
	assert(this->variables != NULL);
	for(list <Variable *>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if((*iter)->GetVariable() == variable)
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

Literal* SAT::createLiteral(const int var)
{
	Variable* v = ContainsVariable((var < 0) ? (-1 * var) : var);
	if (v == NULL)
	{
		v = new Variable(var, this);
		this->variables->push_back(v);
		v->SetListPointer((--this->variables->cend()));
	}
	return new Literal(v, NULL, var > 0);
}

bool SAT::addClause(const list <Literal*>* clause)
{
	assert(clause->size());
	Clause* cla = new Clause(clause, this);
	assert(cla);
	assert(cla->Size());
	if (cla->isValid() && !ContainsClause(cla))
	{
		this->clauses->push_back(cla);
		cla->SetListPointer((--this->clauses->cend()));
		return true;
	}
	else
	{
		for (list <Literal*>::const_iterator literal = clause->cbegin(); literal != clause->cend(); literal++) {
			delete* literal;
		}
		delete cla;
		return false;
	}
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
		list <Literal*> cla;
		for (list <int>::const_iterator literal = clause->cbegin(); literal != clause->cend(); literal++)
		{
			cla.push_back(this->createLiteral(*literal));
		}
		return this->addClause(&cla);
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