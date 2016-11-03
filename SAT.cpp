#include <list>
#include <string>
#include <fstream>

#include "Variable.h"
#include "Clause.h"
#include "SAT.h"

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


//a debugging sanity check function
bool doesListContain(const Variable * var, const list <Variable *> search)
{
	for(list <Variable *>::const_iterator i = search.cbegin(); i != search.cend(); i++)
	{
		if(*i == var)
		{
			return true;
		}
	}
	return false;
}
bool doesListContain(const Clause * cla, const list <Clause *> search)
{
	for(list <Clause *>::const_iterator i = search.cbegin(); i != search.cend(); i++)
	{
		if(*i == cla)
		{
			return true;
		}
	}
	return false;
}

//******************************
//------------------------------
//
// CONSTRUCTORS
//
//------------------------------
//******************************
SAT::SAT()
{
}
SAT::SAT(ifstream & file, const bool isCNF)
{
	if (file.is_open())
	{
		if (isCNF)
		{
			if (!this->ReadCNF(file))
			{
				this->cleanVariables();
				this->cleanClauses();
				return;
			}
		}
		else
		{
			if (!this->ReadFrom(file))
			{
				this->cleanVariables();
				this->cleanClauses();
				return;
			}
		}
	}
}
SAT::SAT(const char * input, const bool isCNF)
{
	//Create intial information
	ifstream file;
	file.open(input);
	if(file.is_open())
	{
		if(isCNF)
		{
			if(!this->ReadCNF(file))
			{
				this->cleanVariables();
				this->cleanClauses();
				return;
			}
		}
		else
		{
			if(!this->ReadFrom(file))
			{
				this->cleanVariables();
				this->cleanClauses();
				return;
			}
		}
	}
	file.close();
}
SAT::~SAT()
{
}
SAT * SAT::copy() const
{
	SAT * sat = new SAT();
	for (list <Variable *>::const_iterator iter = this->previousVariables.cbegin(); iter != this->previousVariables.cend(); iter++)
	{
		sat->variables.push_back((*iter)->copy(sat));
		sat->variables.back()->SetListPointer((--sat->variables.cend()), true);
		assert(sat->variables.back() != (*iter));
		assert((*sat->variables.back()) == (**iter));
	}
	for (list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		sat->variables.push_back((*iter)->copy(sat));
		sat->variables.back()->SetListPointer((--sat->variables.cend()), true);
		assert(sat->variables.back() != (*iter));
		assert((*sat->variables.back()) == (**iter));
	}
	assert(sat->variables.size() == this->variables.size() + this->previousVariables.size());
	for (list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		sat->clauses.push_back((*iter)->copy(&sat->variables, sat));
		sat->clauses.back()->SetListPointer((--sat->clauses.cend()), true);
		assert(sat->clauses.back() != (*iter));
		assert((*sat->clauses.back()) == (**iter));
	}
	for (list <Clause *>::const_iterator iter = this->clauses.cbegin(); iter != this->clauses.cend(); iter++)
	{
		sat->clauses.push_back((*iter)->copy(&sat->variables, sat));
		sat->clauses.back()->SetListPointer((--sat->clauses.cend()), true);
		assert(sat->clauses.back() != (*iter));
		assert((*sat->clauses.back()) == (**iter));
	}
	assert(sat->clauses.size() == this->clauses.size() + this->previousClauses.size());
	sat->UpdateVariables();
	return sat;
}

void SAT::Revert()
{
	while(this->previousVariables.size() > 0)
	{
		Variable * var = *this->previousVariables.cbegin();
		this->variables.push_back(var);
		this->previousVariables.pop_front();
		var->SetListPointer((--this->variables.cend()), true);
	}
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		(*iter)->Revert();
		assert(!(*iter)->IsEvaluated());
	}
	while(this->previousClauses.size() > 0)
	{
		Clause * cla = *this->previousClauses.cbegin();
		this->clauses.push_back(cla);
		this->previousClauses.pop_front();
		cla->SetListPointer((--this->clauses.cend()), true);
	}
	assert(this->previousClauses.size() == 0);
	assert(this->previousVariables.size() == 0);

	this->UpdateVariables();
}

void SAT::cleanVariables()
{
	for(list <Variable *>::const_iterator iter = this->previousVariables.cbegin(); iter != this->previousVariables.cend(); iter++)
	{
		assert((*iter)->IsEvaluated());
		assert(!(*iter)->IsActive());
		assert(!doesListContain(*iter, this->variables));
		delete *iter;
	}
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		delete *iter;
	}
	this->previousVariables.clear();
	this->variables.clear();
}
void SAT::cleanClauses()
{
	for(list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		assert(!(*iter)->IsActive());
		assert(!doesListContain(*iter, this->clauses));
		delete *iter;
	}
	for(list <Clause *>::const_iterator iter = this->clauses.cbegin(); iter != this->clauses.cend(); iter++)
	{
		assert((*iter)->IsActive());
		delete *iter;
	}
	this->previousClauses.clear();
	this->clauses.clear();
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
	for(list <Clause *>::const_iterator iter = this->clauses.cbegin(); iter != this->clauses.cend(); iter++)
	{
		assert((*iter)->IsActive());
		if(**iter == *clause)
		{
			return *iter;
		}
	}
	return NULL;
}
Variable * SAT::ContainsVariable(const Variable * variable) const
{
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		if(**iter == *variable)
		{
			return *iter;
		}
	}
	return NULL;
}
int SAT::CharToInt(const char c) const
{
	if(c >= 'A' && c <= 'Z')
	{
		return (int)(c - 64);
	}
	else if(c >= 'a' && c <= 'z')
	{
		return (int)(-1 * (c - 96));
	}
	else
	{
		return (int)c;
	}
}

Variable * SAT::Sort(Variable * var1, Variable * var2, const list <SortFunction *> * SortFunctions) const
{
	list <SortFunction *>::const_iterator iter = SortFunctions->cbegin();
	for(; iter != SortFunctions->cend(); iter++)
	{
		if(!((*iter)->Eqaulity)(var1, var2))
		{
			return ((*iter)->Sort)(var1, var2);
		}
	}
	return ((*(--iter))->Sort)(var1, var2);
}

//******************************
//------------------------------
//
// READ INPUT FUNCTIONS
//
//------------------------------
//******************************
bool SAT::ReadFrom(ifstream & file)
{
	if(file.eof())
	{
		return false;
	}

	//Read Input
	unsigned int length;
	file >> length;

	if (file.eof())
	{
		return false;
	}
	for(unsigned int i = 0; i < length; i++)
	{
		string s;
		file >> s;
		if(file.eof())
		{
			assert(false);
			return false;
		}
		if(s.size() > 0)
		{
			list <Literal *> clause;
			for(unsigned int i = 0; i < s.size(); i++)
			{
				Variable * var = new Variable(CharToInt(s[i]), this);
				Variable * v = ContainsVariable(var);
				if(!v)
				{
					clause.push_back(new Literal(var, NULL, CharToInt(s[i]) > 0));
					this->variables.push_back(var);
					var->SetListPointer((--this->variables.cend()), true);
				}
				else
				{
					clause.push_back(new Literal(v, NULL, CharToInt(s[i]) > 0));
					delete var;
				}
			}

			assert(clause.size());
			Clause * cla = new Clause(&clause, this);
			assert(cla);
			assert(cla->IntialSize());
			if(cla->isValid() && !ContainsClause(cla))
			{
				this->clauses.push_back(cla);
				cla->SetListPointer((--this->clauses.cend()), true);
			}
			else
			{
				while(clause.size() > 0)
				{
					delete *clause.cbegin();
					clause.pop_front();
				}
				delete cla;
			}
		}
	}

	this->UpdateVariables();

	return true;
}
bool SAT::ReadCNF(ifstream & file)
{
	if(file.eof())
	{
		return false;
	}

	while(true)
	{
		string s;
		file >> s;
		if(file.eof())
		{
			assert(false);
			return false;
		}
		if(s == "p")
		{
			file >> s;
			if(file.eof() || s != "cnf")
			{
				assert(false);
				return false;
			}
			break;
		}
	}

	unsigned int VarCount, ClauseCount;
	file >> VarCount >> ClauseCount;

	//Read inputs
	list <Literal *> clause;
	for(unsigned int i = 0; i < ClauseCount; i++)
	{
		clause.clear();
		int s;
		file >> s;
		if(file.eof())
		{
			//unexpected end of file
			assert(false);
			//make a new clause and delete it for clean up.
			Clause * cla = new Clause(&clause, this);
			delete cla;
			return false;
		}
		while(s != 0)
		{
			Variable * var = new Variable(s, this);
			assert(var);
			Variable * v = ContainsVariable(var);
			if(!v)
			{
				clause.push_back(new Literal(var, NULL, s > 0));
				this->variables.push_back(var);
				var->SetListPointer((--this->variables.cend()), true);
			}
			else
			{
				clause.push_back(new Literal(v, NULL, s > 0));
				delete var;
			}

			file >> s;
		}

		assert(clause.size());
		Clause * cla = new Clause(&clause, this);
		assert(cla);
		assert(cla->IntialSize());
		if(cla->isValid() && !ContainsClause(cla))
		{
			this->clauses.push_back(cla);
			cla->SetListPointer((--this->clauses.cend()), true);
		}
		else
		{
			while(clause.size() > 0)
			{
				delete *clause.cbegin();
				clause.pop_front();
			}
			delete cla;
		}
	}

	assert(VarCount == this->variables.size());

	//There are repeats in the files, do not know why
	//assert(ClauseCount == this->clauses.size());
	this->UpdateVariables();

	return true;
}

//******************************
//------------------------------
//
// CHECKER FUNCTIONS
//
//------------------------------
//******************************
bool SAT::isValid() const
{
	return (this->clauses.size() > 0 || this->previousClauses.size() > 0) && (this->variables.size() > 0  || this->previousVariables.size() > 0);
}

bool SAT::Evaluate(const list <int> * variables) const
{
	for(list <Clause *>::const_iterator iter = this->clauses.cbegin(); iter != this->clauses.cend(); iter++)
	{
		assert((*iter)->IsActive());
		if(!(*iter)->Evaluate(variables))
		{
			return false;
		}
	}
	for(list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		assert(!(*iter)->IsActive());
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
void SAT::StepBackVariables()
{
#ifdef KEEP_VARIABLE_HISTORY
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		(*iter)->StepBack();
	}
#endif
}
void SAT::UpdateVariables()
{
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		(*iter)->Update();
	}
}
void SAT::SetVariables()
{
#ifdef KEEP_VARIABLE_HISTORY
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		(*iter)->Set();
	}
#endif
}

Variable * SAT::NextVariable(const list <SortFunction *> * SortFunctions) const
{
	Variable * val = this->variables.front();
	assert(val->getCurrentState() != NULL);
	assert(val->getCurrentState()->IsSet());
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert((*iter)->getCurrentState() != NULL);
		assert((*iter)->getCurrentState()->IsSet());
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		val = this->Sort(val, *iter, SortFunctions);
	}
	return val;
}

Variable * SAT::NextVariable(const Variable * Excluded_Variable, const list <SortFunction *> * SortFunctions) const
{
	Variable * val = this->variables.front();
	assert(val->getCurrentState() != NULL);
	assert(val->getCurrentState()->IsSet());
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert((*iter)->getCurrentState() != NULL);
		assert((*iter)->getCurrentState()->IsSet());
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		val = this->Sort(val, *iter, SortFunctions);
	}
	return val;
}
bool SAT::AllPreviousClausesHaveFalse() const
{
	for(list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		assert(!(*iter)->IsActive());
		assert((*iter)->AllEvaluted() || (*iter)->HasTrue());
		if(!(*iter)->HasFalse())
		{
			return false;
		}
	}
	return true;
}
bool SAT::AllPreviousClausesHaveTrue() const
{
	for(list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		assert(!(*iter)->IsActive());
		assert((*iter)->AllEvaluted() || (*iter)->HasTrue());
		if(!(*iter)->HasTrue())
		{
			return false;
		}
	}
	return true;
}

bool SAT::AllClausesHaveFalse() const
{
	for(list <Clause *>::const_iterator iter = this->clauses.cbegin(); iter != this->clauses.cend(); iter++)
	{
		assert((*iter)->IsActive());
		assert(!(*iter)->HasTrue());
		if(!(*iter)->HasFalse())
		{
			return false;
		}
	}
	for(list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		assert(!(*iter)->IsActive());
		assert((*iter)->HasTrue()); 
		if (!(*iter)->HasFalse())
		{
			return false;
		}
	}
	return true;
}
bool SAT::AllClausesHaveTrue() const
{
	for(list <Clause *>::const_iterator iter = this->clauses.cbegin(); iter != this->clauses.cend(); iter++)
	{
		assert((*iter)->IsActive());
		assert(!(*iter)->HasTrue());
		if(!(*iter)->HasTrue())
		{
			return false;
		}
	}
	for(list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		assert(!(*iter)->IsActive());
		assert((*iter)->HasTrue());
		if(!(*iter)->HasTrue())
		{
			return false;
		}
	}
	return true;
}
bool SAT::HasSolution() const
{
	for(list <Clause *>::const_iterator iter = this->clauses.cbegin(); iter != this->clauses.cend(); iter++)
	{
		assert((*iter)->IsActive());
		if(!(*iter)->HasSolution())
		{
			return false;
		}
	}
	for(list <Variable *>::const_iterator iter = this->variables.cbegin(); iter != this->variables.cend(); iter++)
	{
		assert((*iter)->getCurrentState() != NULL);
		assert((*iter)->getCurrentState()->IsSet());
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		if (!(*iter)->HasSolution())
		{
			return false;
		}
	}
	return true;
}
bool SAT::HasOppositeSolution() const
{
	for (list <Clause *>::const_iterator iter = this->previousClauses.cbegin(); iter != this->previousClauses.cend(); iter++)
	{
		assert(!(*iter)->IsActive());
		assert((*iter)->HasTrue());
		if (!(*iter)->HasOppositeSolution())
		{
			return false;
		}
	}
	/*for (list <Variable *>::const_iterator iter = this->previousVariables.cbegin(); iter != this->previousVariables.cend(); iter++)
	{
		assert((*iter)->getCurrentState() != NULL);
		assert((*iter)->getCurrentState()->IsSet());
		assert(!(*iter)->IsEvaluated());
		assert((*iter)->IsActive());
		if (!(*iter)->HasOppositeSolution())
		{
			return false;
		}
	}*/
	return true;
}
void SAT::FlipAll()
{
	for(list <Variable *>::const_iterator iter = this->previousVariables.cbegin(); iter != this->previousVariables.cend(); iter++)
	{
		assert((*iter)->IsEvaluated());
		assert(!(*iter)->IsActive());
		(*iter)->Flip();
	}

	this->UpdateVariables();
}

void SAT::FlipVariable(Variable * var)
{
	assert(var->IsEvaluated());
	assert(!var->IsActive());
	assert(doesListContain(var, this->previousVariables));
	assert(!doesListContain(var, this->variables));

	var->Flip();

	assert(doesListContain(var, this->previousVariables));
	assert(!doesListContain(var, this->variables));

	assert(var->IsEvaluated());
	assert(!var->IsActive());

	this->UpdateVariables();
}

void SAT::UndoVariable(Variable * var)
{
	this->StepBackVariables();

	assert(var->IsEvaluated());
	assert(!var->IsActive());

	assert(doesListContain(var, this->previousVariables));
	assert(!doesListContain(var, this->variables));

	//Revert the variable
	var->Undo();

	assert(!doesListContain(var, this->previousVariables));
	assert(doesListContain(var, this->variables));

	assert(!var->IsEvaluated());
	assert(var->IsActive());

	this->UpdateVariables();
}

void SAT::RemoveVariable(Variable * var, const bool isPositive)
{
	this->SetVariables();

	assert(!var->IsEvaluated());
	assert(var->IsActive());

	assert(!doesListContain(var, this->previousVariables));
	assert(doesListContain(var, this->variables));

	//Remove the variable
	var->Remove(isPositive);

	assert(doesListContain(var, this->previousVariables));
	assert(!doesListContain(var, this->variables));

	assert(var->IsEvaluated());
	assert(!var->IsActive());

	this->UpdateVariables();
}

unsigned int SAT::ClauseCount() const
{
	return this->clauses.size();
}

unsigned int SAT::VariableCount() const
{
	return this->variables.size();
}


void SAT::Remove(list <Clause *>::const_iterator listPointer, bool makeActive)
{
	Clause * cla = *listPointer;
	if(makeActive)
	{
		assert(!cla->IsActive());
		assert(doesListContain(cla, this->previousClauses));
		assert(!doesListContain(cla, this->clauses));

		this->previousClauses.erase(listPointer);
		this->clauses.push_back(cla);
		cla->SetListPointer((--this->clauses.cend()), true);

		assert(!doesListContain(cla, this->previousClauses));
		assert(doesListContain(cla, this->clauses));
	}
	else
	{
		assert(cla->IsActive());
		assert(!doesListContain(cla, this->previousClauses));
		assert(doesListContain(cla, this->clauses));

		this->clauses.erase(listPointer);
		this->previousClauses.push_back(cla);
		cla->SetListPointer((--this->previousClauses.cend()), false);

		assert(doesListContain(cla, this->previousClauses));
		assert(!doesListContain(cla, this->clauses));
	}
}
void SAT::Remove(list <Variable *>::const_iterator listPointer, bool makeActive)
{
	Variable * var = *listPointer;
	if(makeActive)
	{
		assert(!var->IsActive());
		assert(doesListContain(var, this->previousVariables));
		assert(!doesListContain(var, this->variables));

		this->previousVariables.erase(listPointer);
		this->variables.push_back(var);
		var->SetListPointer((--this->variables.cend()), true);

		assert(!doesListContain(var, this->previousVariables));
		assert(doesListContain(var, this->variables));
	}
	else
	{
		assert(var->IsActive());
		assert(!doesListContain(var, this->previousVariables));
		assert(doesListContain(var, this->variables));

		this->variables.erase(listPointer);
		this->previousVariables.push_back(var);
		var->SetListPointer((--this->previousVariables.cend()), false);

		assert(doesListContain(var, this->previousVariables));
		assert(!doesListContain(var, this->variables));
	}
}

list <int> * SAT::getResults() const
{
	list <int> * result = new list <int>();
	for (list <Variable *>::const_iterator iter = this->previousVariables.cbegin(); iter != this->previousVariables.cend(); iter++)
	{
		assert((*iter)->IsEvaluated());
		assert(!(*iter)->IsActive());
		result->push_back((*iter)->GetValue());
	}
	return result;
}