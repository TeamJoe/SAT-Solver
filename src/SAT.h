class SAT;

#ifndef SAT_H
#define SAT_H

#include <fstream>
#include <list>

#include "Variable.h"
#include "Clause.h"
#include "SATState.h"

#include "Constants.h"

using namespace std;

class SAT
{
private:
	list <Variable *> * variables;
	list <Clause *> * clauses;
	bool Valid;

	//Private Functions
	Clause * ContainsClause(const Clause * clause) const;
	Variable * ContainsVariable(const int variable) const;
	Variable * ContainsVariable(const Variable * variable) const;
	int CharToInt(const char c) const;

	//Read Input Functions
	Literal* createLiteral(const int var);
	bool addClause(const list <Literal*>* var);
	bool ReadFrom(ifstream & file);
	bool ReadCNF(ifstream & file);
protected:
	void cleanVariables();
	void cleanClauses();
	SAT();
public:

	//Constructors
	SAT(ifstream & file, const bool isCNF);
	SAT(const char * input, const bool isCNF);
	~SAT();

	//Checker Functions
	unsigned int getIdentifier() const;
	bool isValid() const;
	bool Evaluate(const int * variables) const;
	bool Evaluate(const list <int> * variables) const;

	//Solver Functions
	unsigned int ClauseCount() const;
	unsigned int VariableCount() const;

	friend SATState;
};

#endif