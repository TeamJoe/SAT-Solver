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
SAT::SAT(ifstream & file, const bool isCNF)
{
	this->variables = new list <Variable *>();
	this->clauses = new list <Clause *>();
	this->Valid = false;

	if (file.is_open())
	{
		if (isCNF)
		{
			if (this->ReadCNF(file))
			{
				this->Valid = true;
			}
		}
		else
		{
			if (this->ReadFrom(file))
			{
				this->Valid = true;
			}
		}
	}
}
SAT::SAT(const char * input, const bool isCNF)
{
	this->variables = new list <Variable *>();
	this->clauses = new list <Clause *>();
	this->Valid = false;

	//Create intial information
	ifstream file;
	file.open(input);
	if(file.is_open())
	{
		if(isCNF)
		{
			if (this->ReadCNF(file))
			{
				this->Valid = true;
			}
		}
		else
		{
			if (this->ReadFrom(file))
			{
				this->Valid = true;
			}
		}
	}
	file.close();
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

//******************************
//------------------------------
//
// READ INPUT FUNCTIONS
//
//------------------------------
//******************************
bool SAT::ReadFrom(ifstream & file)
{
	assert(this->variables != NULL);
	assert(this->clauses != NULL);

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
				int var = CharToInt(s[i]);
				Variable * v = ContainsVariable((var < 0) ? (-1 * var) : var);
				if(v == NULL)
				{
					v = new Variable(var, this);
					this->variables->push_back(v);
					v->SetListPointer((--this->variables->cend()));
				}
				clause.push_back(new Literal(v, NULL, var > 0));
			}

			assert(clause.size());
			Clause * cla = new Clause(&clause, this);
			assert(cla);
			assert(cla->Size());
			if(cla->isValid() && !ContainsClause(cla))
			{
				this->clauses->push_back(cla);
				cla->SetListPointer((--this->clauses->cend()));
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

	return true;
}
bool SAT::ReadCNF(ifstream & file)
{
	assert(this->variables != NULL);
	assert(this->clauses != NULL);

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
		int var;
		file >> var;
		if(file.eof())
		{
			//unexpected end of file
			assert(false);
			//make a new clause and delete it for clean up.
			Clause * cla = new Clause(&clause, this);
			delete cla;
			return false;
		}
		while(var != 0)
		{
			Variable * v = ContainsVariable((var < 0) ? (-1 * var) : var);
			if(v == NULL)
			{
				v = new Variable(var, this);
				this->variables->push_back(v);
				v->SetListPointer((--this->variables->cend()));
			}
			clause.push_back(new Literal(v, NULL, var > 0));

			file >> var;
		}

		assert(clause.size());
		Clause * cla = new Clause(&clause, this);
		assert(cla);
		assert(cla->Size());
		if(cla->isValid() && !ContainsClause(cla))
		{
			this->clauses->push_back(cla);
			cla->SetListPointer((--this->clauses->cend()));
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

	assert(VarCount == this->variables->size());

	//There are repeats in the files, do not know why
	//assert(ClauseCount == this->clauses.size());

	return true;
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
bool SAT::isValid() const
{
	return this->Valid;
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