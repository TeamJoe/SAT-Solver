#include <list>
#include <fstream>
#include <iostream>
#include <thread>
#include <windows.h>

#include "Variable.h"
#include "SAT.h"
#include "SATSolver.h"
#include "Sorters.h"

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
SATSolver::SATSolver(SAT * sat)
{
	this->sat = sat;
}
SATSolver::~SATSolver()
{
	assert(this->sat);
	this->sat->cleanVariables();
	this->sat->cleanClauses();
	this->sat = NULL;
}

//******************************
//------------------------------
//
// PRIVATE FUNCTIONS
//
//------------------------------
//******************************
/*list <string> & SATSolver::SlowSat()
{
	stringstream var;
	for(list <char>::const_iterator i = Variables.cbegin(); i != Variables.cend(); i++)
	{
		var << *i;
	}
	string variables = var.str();
	list <string> * Returns = new list <string>;

	unsigned int max = 1 << variables.size();
	bool match = false;
	for(unsigned int x = 0; x < max; x++)
	{
		for(unsigned int y = 0; y < variables.size(); y++)
		{
			if(((x >> y) % 2) == 1)
			{
				//variables[y] = ToUpper(variables[y]);
			}
			else
			{
				//variables[y] = ToLower(variables[y]);
			}
		}

		match = true;
		for(list <SAT>::const_iterator i = Clauses.cbegin(); i != Clauses.cend(); i++)
		{
			if(!i->evaluate(variables))
			{
				match = false;
				break;
			}
		}
		if(match)
		{
			Returns->push_back(variables);
		}
	}
	return (*Returns);
}*/

volatile static bool terminateAllThreads;

Pair * FlipSat(SAT * sat, int (Decider)(Variable *), const list <SortFunction *> * SortFunctions)
{
	//Occurs when the program cannot decide what value to give a variable
	list<unsigned int> * Equalility = new list<unsigned int>();

	//Occurs when the program cannot decide what variable to evaluate next 
	list<unsigned int> * Undecidable = new list<unsigned int>();
	list<Variable *> * VariableOrder = new list<Variable *>();
	unsigned int y = 0;

	while(sat->VariableCount() > 0)
	{
		if (terminateAllThreads) {
			break;
		}

		y++;
		Variable * var1 = sat->NextVariable(SortFunctions);
		Variable * var2 = sat->NextVariable(var1, SortFunctions);

		//if(sat->Sort(var1, var2, SortFunctions) != var1 || sat->Sort(var2, var1, SortFunctions) != var1)
		//{
		//	Undecidable->push_back(y);
		//}

		VariableOrder->push_back(var1);

		//Get best variable
		int solution = Decider(var1);

		if(solution == VARIABLE_UNKNOWN)
		{
			solution = VARIABLE_POSITIVE;
			Equalility->push_back(y);
		}

		sat->RemoveVariable(var1, solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE);
		assert(var1->GetValue() != 0);
	}

	if(sat->ClauseCount() != 0)
	{
		while(VariableOrder->size() > 0)
		{
			if (terminateAllThreads) {
				break;
			}

			Variable * var = VariableOrder->front();
			VariableOrder->pop_front();
			sat->FlipVariable(var);
			if(sat->ClauseCount() == 0)
			{
				break;
			}
			else if(sat->AllClausesHaveFalse())
			{
				sat->FlipAll();

				assert(sat->ClauseCount() == 0);
				assert(sat->AllClausesHaveTrue());
				break;
			}
			sat->FlipVariable(var);
		}
	}
	delete VariableOrder;

	Pair * a = new Pair;
	a->Result = sat->getResults();
	a->Equalility = Equalility;
	a->Undecidable = Undecidable;
	if(sat->ClauseCount() == 0)
	{
		assert(sat->AllClausesHaveTrue());
		a->Return = true;
	}
	else
	{
		a->Return = false;
	}
	return a;
}

bool hasRepeat(const list<list<int> *> * repeats, const list<int> * result)
{
	for (list<list<int> *>::const_iterator iter = repeats->cbegin(); iter != repeats->cend(); iter++)
	{
		if (result->size() != (*iter)->size()) {
			continue;
		}
		bool repeat = true;
		for (list<int>::const_iterator list1 = (*iter)->cbegin(); list1 != (*iter)->cend(); list1++)
		{
			bool hasLiteral = false;
			for (list<int>::const_iterator list2 = result->cbegin(); list2 != result->cend(); list2++)
			{
				if ((*list1) == (*list2)) {
					hasLiteral = true;
					break;
				}
			}
			if (hasLiteral == false) {
				repeat = false;
				break;
			}
		}
		if (repeat) {
			return true;
		}
	}
	return false;
}

bool hasRepeat(list<list<int> *> * repeats, const bool isRepeat, list<int> * result)
{
#ifdef _DEBUG
	if (isRepeat)
	{
		bool hh = hasRepeat(repeats, result);
		delete result;
		return hh == isRepeat;
	}
	else
	{
		bool hh = hasRepeat(repeats, result);
		repeats->push_back(result);
		return hh == isRepeat;
	}
#endif
}

int _DepthSat(SAT * sat, int (Decider)(Variable *), const list <SortFunction *> * SortFunctions, DepthPair * a, bool reversed, list<list<int> *> * repeats, unsigned long long maxDepth);
DepthPair * DepthSat(SAT * sat, int (Decider)(Variable *), const list <SortFunction *> * SortFunctions)
{
	list<list<int> *> * repeats = NULL;
#ifdef _DEBUG
	repeats = new list<list<int> *>();
	assert(repeats->size() == 0);
	repeats->push_back(new list<int>());
#endif

	DepthPair * a = new DepthPair;
	a->Result = new list<list<int> *>();
	a->EvalCount = 0;
	a->AttemptCount = 0;
	a->Return = 0;

	//No solution exists
	if (!sat->HasSolution()) {
		a->EvalCount = 1;
		a->Return = 0;
		sat->Revert();
		return a;
	}

#ifdef DEPTH_USES_FLIP
	Variable * var1 = sat->NextVariable(SortFunctions);

	//Get best variable
	int solution = Decider(var1);
	assert(solution != VARIABLE_NO_SOLUTION);

	//Check if chain has a solution
	sat->RemoveVariable(var1, solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE);
	
	assert(hasRepeat(repeats, false, sat->getResults()));
	assert(var1->GetValue() != 0);
	assert((solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE) && var1->GetValue() > 0 
		|| (solution == VARIABLE_NEGATIVE || solution == MUST_NEGATIVE) && var1->GetValue() < 0 
		|| (solution == VARIABLE_UNKNOWN && var1->GetValue() != 0));
#endif

	//Run the evaluation
	int i = _DepthSat(sat, Decider, SortFunctions, a, false, repeats, MAX_DEPTH_LIMIT);
	if (a->Result->size() > 0)
	{
		//assert(sat->ClauseCount() == 0);
		//assert(sat->AllClausesHaveTrue());
		if (MAX_DEPTH_LIMIT < a->EvalCount || terminateAllThreads) {
			a->Return = 0; 
		} else {
			a->Return = 1;
		}
	}
	else if (i < 1) 
	{
		assert(MAX_DEPTH_LIMIT < a->EvalCount || terminateAllThreads);
		assert(a->Result->size() == 0);
		assert(sat->ClauseCount() > 0);
		assert(!sat->AllClausesHaveTrue());
		assert(!sat->AllClausesHaveFalse());
		a->Return = 0;
	}
	else
	{
		assert(MAX_DEPTH_LIMIT >= a->EvalCount);
		assert(a->Result->size() == 0);
		assert(sat->ClauseCount() > 0);
		assert(!sat->AllClausesHaveTrue());
		assert(!sat->AllClausesHaveFalse());
		a->Return = -1;
	}

#ifdef _DEBUG
	for (list<list<int> *>::const_iterator iter = repeats->cbegin(); iter != repeats->cend(); iter++)
	{
		delete (*iter);
	}
	delete repeats;
#endif
	return a;
}

int _DepthSat(SAT * sat, int (Decider)(Variable *), const list <SortFunction *> * SortFunctions, DepthPair * a, bool reversed, list<list<int> *> * repeats, unsigned long long maxDepth)
{
	//If all variable have a solution
	if(sat->VariableCount() < 1)
	{
		a->AttemptCount++;
		//If the solution is the opposite of what we currently have
		assert(hasRepeat(repeats, true, sat->getResults()));
#ifndef DEPTH_FULL_EVAL
		if(sat->ClauseCount() != 0 && sat->AllClausesHaveFalse())
		{
			sat->FlipAll();
			assert(hasRepeat(repeats, false, sat->getResults()));
		}
#endif
		if (sat->ClauseCount() == 0)
		{
			assert(sat->ClauseCount() == 0);
			assert(sat->AllClausesHaveTrue());
			assert(a->Result != NULL);
			a->Result->push_back(sat->getResults());
#ifdef _DEBUG
			for (list<list<int> *>::const_iterator iter = a->Result->cbegin(); iter != a->Result->cend(); iter++)
			{
				assert(sat->Evaluate(*iter));
			}
#endif
		}
		if (terminateAllThreads) {
			return -1;
		}
		return 1;
	}

	//exclude whole chains where every branch evaluates to true
	if (sat->ClauseCount() == 0)
	{
		assert(sat->ClauseCount() == 0);
		assert(sat->AllClausesHaveTrue());
		assert(a->Result != NULL);
		a->Result->push_back(sat->getResults());
#ifdef _DEBUG
		for (list<list<int> *>::const_iterator iter = a->Result->cbegin(); iter != a->Result->cend(); iter++)
		{
			assert(sat->Evaluate(*iter));
		}
#endif
		if (terminateAllThreads) {
			return -1;
		}
		return 1;
	}

	if (terminateAllThreads) {
		return -1;
	}

	//if no solution exists, just return
	if (!sat->HasSolution()) {
#ifdef DEPTH_USES_FLIP
		if (!reversed && sat->HasOppositeSolution())
		{
			assert(!reversed);
			assert(hasRepeat(repeats, true, sat->getResults()));
			sat->FlipAll();
			assert(hasRepeat(repeats, false, sat->getResults()));
			assert(sat->HasSolution());
			this->_DepthSat(sat, Decider, SortFunctions, a, true, repeats, maxDepth);
			if (a->Result != NULL) {
				assert(sat->AllClausesHaveTrue());
				assert(sat->ClauseCount() == 0);
				assert(hasRepeat(repeats, true, sat->getResults()));
			} else {
				assert(!sat->AllClausesHaveTrue());
				assert(sat->ClauseCount() > 0);
				assert(hasRepeat(repeats, true, sat->getResults()));
				sat->FlipAll();
				assert(hasRepeat(repeats, true, sat->getResults()));
			}
		}
#endif
		//If max limit was hit, return
		if (maxDepth < a->EvalCount)
		{
			assert(hasRepeat(repeats, true, sat->getResults()));
			return 0;
		}
		return 1;
	}

	Variable * var1 = sat->NextVariable(SortFunctions);

	//Get best variable
	int solution = Decider(var1);
	assert(solution != VARIABLE_NO_SOLUTION);

	//Check if chain has a solution
	sat->RemoveVariable(var1, solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE);

	assert(hasRepeat(repeats, false, sat->getResults()));
	assert(var1->GetValue() != 0);
	assert((solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE) && var1->GetValue() > 0 
		|| (solution == VARIABLE_NEGATIVE || solution == MUST_NEGATIVE) && var1->GetValue() < 0 
		|| (solution == VARIABLE_UNKNOWN && var1->GetValue() != 0)
		|| (solution == VARIABLE_NO_SOLUTION && var1->GetValue() != 0));

	a->EvalCount++;
	int i = _DepthSat(sat, Decider, SortFunctions, a, reversed, repeats, maxDepth);
	if (i < 0) {
		return i;
	}

	// if solution found return
#ifndef DEPTH_FULL_EVAL
	if (a->Result->size() > 0) {
		assert(sat->ClauseCount() == 0);
		assert(sat->AllClausesHaveTrue());
		assert(hasRepeat(repeats, true, sat->getResults()));
		return 1;
	}
	assert(sat->ClauseCount() > 0);
#endif

	//If max limit was hit, return
	if (maxDepth < a->EvalCount)
	{
		assert(hasRepeat(repeats, true, sat->getResults()));
		sat->UndoVariable(var1);
		assert(hasRepeat(repeats, true, sat->getResults()));
		return 0;
	}

	//Check if the opposite solution is plausible
	//if(solution != MUST_POSITIVE && solution != MUST_NEGATIVE)
	{
		assert(hasRepeat(repeats, true, sat->getResults()));
		sat->FlipVariable(var1);

		assert(hasRepeat(repeats, false, sat->getResults()));
		assert(var1->GetValue() != 0);
		assert((solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE) && var1->GetValue() < 0
			|| (solution == VARIABLE_NEGATIVE || solution == MUST_NEGATIVE) && var1->GetValue() > 0
			|| (solution == VARIABLE_UNKNOWN && var1->GetValue() != 0)
			|| (solution == VARIABLE_NO_SOLUTION && var1->GetValue() != 0));

		a->EvalCount++;
		int i = _DepthSat(sat, Decider, SortFunctions, a, reversed, repeats, maxDepth);
		if (i < 0) {
			return i;
		}

#ifndef DEPTH_FULL_EVAL
		// if solution found return
		if (a->Result->size() > 0) {
			assert(sat->ClauseCount() == 0);
			assert(sat->AllClausesHaveTrue());
			assert(hasRepeat(repeats, true, sat->getResults()));
			return 1;
		}
		assert(sat->ClauseCount() > 0);
#endif
	}

	//If max limit was hit, return
	if (maxDepth < a->EvalCount)
	{
		assert(hasRepeat(repeats, true, sat->getResults()));
		sat->UndoVariable(var1);
		assert(hasRepeat(repeats, true, sat->getResults()));
		return 0;
	}

	//Check if the reversed is possible as well
#ifdef DEPTH_USES_FLIP
	if (!reversed && sat->HasOppositeSolution())
	{
		assert(!reversed);
		assert(hasRepeat(repeats, true, sat->getResults()));
		sat->UndoVariable(var1);
		assert(hasRepeat(repeats, true, sat->getResults()));
		sat->FlipAll();
		assert(hasRepeat(repeats, false, sat->getResults()));
		assert(sat->HasSolution());
		int i = this->_DepthSat(sat, Decider, SortFunctions, a, true, repeats, maxDepth);
		if (i < 0) {
			return i;
		} else if (a->Result != NULL) {
			assert(sat->AllClausesHaveTrue());
			assert(sat->ClauseCount() == 0);
			assert(hasRepeat(repeats, true, sat->getResults()));
		} else {
			assert(!sat->AllClausesHaveTrue());
			assert(sat->ClauseCount() > 0);
			assert(hasRepeat(repeats, true, sat->getResults()));
			sat->FlipAll();
			assert(hasRepeat(repeats, true, sat->getResults()));
		}

		return 1;
	}
#endif

	//no solution found so step back and continue search
	assert(hasRepeat(repeats, true, sat->getResults()));
	sat->UndoVariable(var1);
	assert(hasRepeat(repeats, true, sat->getResults()));
	
	return 1;
}

Pair * FastSat(SAT * sat, int (Decider)(Variable *), const list <SortFunction *> * SortFunctions)
{
	//Occurs when the program cannot decide what value to give a variable
	list<unsigned int> * Equalility = new list<unsigned int>();

	//Occurs when the program cannot decide what variable to evaluate next 
	list<unsigned int> * Undecidable = new list<unsigned int>();
	unsigned int y = 0;

	while(sat->VariableCount() > 0)
	{
		if (terminateAllThreads) {
			break;
		}

		y++;
		Variable * var1 = sat->NextVariable(SortFunctions);
		Variable * var2 = sat->NextVariable(var1, SortFunctions);

		if(sat->Sort(var1, var2, SortFunctions) != var1 || sat->Sort(var2, var1, SortFunctions) != var1)
		{
			Undecidable->push_back(y);
		}

		//Get best variable
		int solution = Decider(var1);

		if(solution == VARIABLE_UNKNOWN)
		{
			solution = VARIABLE_POSITIVE;
			Equalility->push_back(y);
		}

		sat->RemoveVariable(var1, solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE);
		assert(var1->GetValue() != 0);
	}

	if(sat->ClauseCount() != 0 && sat->AllClausesHaveFalse())
	{
		sat->FlipAll();

		assert(sat->ClauseCount() == 0);
		assert(sat->AllClausesHaveTrue());
	}

	Pair * a = new Pair;
	a->Result = sat->getResults();
	a->Equalility = Equalility;
	a->Undecidable = Undecidable;
	if(sat->ClauseCount() == 0)
	{
		assert(sat->AllClausesHaveTrue());
		a->Return = true;
	}
	else
	{
		a->Return = false;
	}
	return a;
}

/*
A debug function which checks the results for duplicates
*/
bool hasDuplicates(const list<int> * Results) {
	vector <bool> variables;
	for(list <int>::const_iterator iter = Results->cbegin(); iter != Results->cend(); iter++)
	{
		int variable = 0;
		if ((*iter) < 0)
		{
			variable = -1 * (*iter);
		} 
		else
		{
			variable = (*iter);
		}
		assert(variable > 0);
		variable = variable - 1;
		if (variable >= (int)variables.size())
		{
			unsigned int oldSize = variables.size();
			variables.resize(variable + 1);
			variables[variable] = true;
			for (oldSize; oldSize < variables.size() - 1; oldSize++)
			{
				variables[oldSize] = false;
			}
		}
		else
		{
			if (variables[variable] == true)
			{
				return true;
			} 
			else
			{
				variables[variable] = true;
			}
		}
	}
	return false;
}

#ifdef DEPTH
#define SAT_FUNCTION DepthSat
#define SOLUTION_FUNCTION DefaultSolver
#endif
#ifdef FLIP
#define SAT_FUNCTION FlipSat
#define SOLUTION_FUNCTION FlipSatSolver
#endif
#ifdef FAST
#define SAT_FUNCTION FastSat
#define SOLUTION_FUNCTION DefaultSolver
#endif
#ifndef SAT_FUNCTION
#error Please define a SAT_FUNCTION
#endif

list<int *> * GetSortList()
{
	list<int *> * sortList = NULL;
	//******************************
	//------------------------------
	//
	// Orginal Set
	//
	//------------------------------
	//******************************
#ifdef ORGINAL_SET
	sortList = new list<int *>();
	sortList->push_back(new int[3] { 4, 0, -1 }); //MostDifference, MostLargeUsed
	sortList->push_back(new int[3] { 4, 1, -1 }); //MostDifference, LeastLargeUsed
	sortList->push_back(new int[3] { 5, 0, -1 }); //LeastDifference, MostLargeUsed
	sortList->push_back(new int[3] { 5, 1, -1 }); //LeastDifference, LeastLargeUsed
	sortList->push_back(new int[3] { 0, 4, -1 }); //MostLargeUsed, MostDifference
	sortList->push_back(new int[3] { 1, 4, -1 }); //LeastLargeUsed, MostDifference
	sortList->push_back(new int[3] { 0, 5, -1 }); //MostLargeUsed, LeastDifference
	sortList->push_back(new int[3] { 1, 5, -1 }); //LeastLargeUsed, LeastDifference
#endif
	//******************************
	//------------------------------
	//
	// Secondary Set
	//
	//------------------------------
	//******************************
#ifdef SECONDARY_SET
	sortList = new list<int *>();
	sortList->push_back(new int [7] { 7, 8, 11, 12, 4, 0, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostDifference, MostLargeUsed
	sortList->push_back(new int [7] { 7, 8, 11, 12, 4, 1, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostDifference, LeastLargeUsed
	sortList->push_back(new int [7] { 7, 8, 11, 12, 5, 0, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastDifference, MostLargeUsed
	sortList->push_back(new int [7] { 7, 8, 11, 12, 5, 1, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastDifference, LeastLargeUsed
	sortList->push_back(new int [7] { 7, 8, 11, 12, 0, 4, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostLargeUsed, MostDifference
	sortList->push_back(new int [7] { 7, 8, 11, 12, 1, 4, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastLargeUsed, MostDifference
	sortList->push_back(new int [7] { 7, 8, 11, 12, 0, 5, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostLargeUsed, LeastDifference
	sortList->push_back(new int [7] { 7, 8, 11, 12, 1, 5, -1 }); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastLargeUsed, LeastDifference
	sortList->push_back(new int [7] { 4, 0, 7, 8, 11, 12, -1 }); //MostDifference, MostLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(new int [7] { 4, 1, 7, 8, 11, 12, -1 }); //MostDifference, LeastLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(new int [7] { 5, 0, 7, 8, 11, 12, -1 }); //LeastDifference, MostLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(new int [7] { 5, 1, 7, 8, 11, 12, -1 }); //LeastDifference, LeastLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(new int [7] { 0, 4, 7, 8, 11, 12, -1 }); //MostLargeUsed, MostDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(new int [7] { 1, 4, 7, 8, 11, 12, -1 }); //LeastLargeUsed, MostDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(new int [7] { 0, 5, 7, 8, 11, 12, -1 }); //MostLargeUsed, LeastDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(new int [7] { 1, 5, 7, 8, 11, 12, -1 }); //LeastLargeUsed, LeastDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
#endif
	return sortList;
}

#ifdef DEPTH
ReturnValue AnalysisResults(ofstream & file, DepthPair ** Results, const SAT * sat)
#else
ReturnValue AnalysisResults(ofstream & file, Pair ** Results, const SAT * sat)
#endif
{
	//******************************
	//------------------------------
	//
	// Analysis
	//
	//------------------------------
	//******************************
#ifdef DEPTH
	ReturnValue output;
	output.Falses = 0;
	output.Trues = 0;
	output.Unknowns = 0;
	output.Result = new list<list<int> *>();

	for (unsigned int i = 0; Results[i] != NULL; i++)
	{
		if (Results[i]->Return > 0)
		{
			file << "1";
			assert(Results[i]->Result->size() > 0);
			output.Trues += (unsigned long long)1 << (unsigned long long)i;
			assert(output.Falses == 0);
			for (list<list<int> *>::const_iterator iter = Results[i]->Result->cbegin(); iter != Results[i]->Result->cend(); iter++)
			{
				assert(!hasDuplicates(*iter));
				assert(sat->Evaluate(*iter));
				if (!hasRepeat(output.Result, (*iter))) {
					output.Result->push_back(*iter);
				} else {
					delete (*iter);
				}
			}
			delete Results[i]->Result;
		}
		else if (Results[i]->Return < 0)
		{
			file << "-1";
			output.Falses += (unsigned long long)1 << (unsigned long long)i;
			assert(output.Trues == 0);
			assert(Results[i]->Result->size() == 0);
			//assert(!hasDuplicates(Results[i]->Result));
			//assert(!sat->Evaluate(Results[i]->Result));
			delete Results[i]->Result;
		}
		else
		{
			file << "0";
			output.Unknowns += (unsigned long long)1 << (unsigned long long)i;
			assert(Results[i]->Result->size() == 0);
			//assert(!hasDuplicates(Results[i]->Result));
			//assert(!sat->Evaluate(Results[i]->Result));
			delete Results[i]->Result;
		}

		list<unsigned int>::iterator y;

		file << "(";
		file << Results[i]->AttemptCount;
		file << ")";

		file << "(";
		file << Results[i]->EvalCount;
		file << ")";

		file << ",";
		delete Results[i];
	}
	delete [] Results;
	return output;
#else
	ReturnValue output;
	output.Falses = 0;
	output.Trues = 0;
	output.Unknowns = 0;
	output.Result = new list<list<int> *>();

	for (unsigned int i = 0; Results[i] != NULL; i++)
	{
		if (Results[i]->Return)
		{
			file << "1";
			output.Trues += (unsigned long long)1 << (unsigned long long)i;
			assert(!hasDuplicates(Results[i]->Result));
			assert(sat->Evaluate(Results[i]->Result));
			if (!hasRepeat(output.Result, Results[i]->Result)) {
				output.Result->push_back(Results[i]->Result);
			}
			else {
				delete Results[i]->Result;
			}
		}
		else if (Results[i]->Equalility->empty() && Results[i]->Undecidable->empty())
		{
			file << "-1";
			output.Falses += (unsigned long long)1 << (unsigned long long)i;
			assert(!hasDuplicates(Results[i]->Result));
			assert(!sat->Evaluate(Results[i]->Result));
			delete Results[i]->Result;
		}
		else
		{
			file << "0";
			output.Unknowns += (unsigned long long)1 << (unsigned long long)i;
			assert(!hasDuplicates(Results[i]->Result));
			assert(!sat->Evaluate(Results[i]->Result));
			delete Results[i]->Result;
		}

		list<unsigned int>::iterator y;

		//Output Undeciable list
		y = Results[i]->Undecidable->begin();
		file << "(";
		if (y != Results[i]->Undecidable->end())
		{
			file << *y;
			y++;
			for (; y != Results[i]->Undecidable->end(); y++)
			{
				file << "|" << *y;
			}
		}
		file << ")";

		//Output Equalility list
		y = Results[i]->Equalility->begin();
		file << "(";
		if (y != Results[i]->Equalility->end())
		{
			file << *y;
			y++;
			for (; y != Results[i]->Equalility->end(); y++)
			{
				file << "|" << *y;
			}
		}
		file << ")";

		file << ",";
		delete Results[i]->Equalility;
		delete Results[i]->Undecidable;
		delete Results[i];
	}
	delete [] Results;
	return output;
#endif
}

#ifdef DEPTH
list<list<int> *> * AnalysisResult(ofstream & file, DepthPair * Result, const SAT * sat)
#else
list <int> * AnalysisResult(ofstream & file, Pair * Result, const SAT * sat)
#endif
{
	//******************************
	//------------------------------
	//
	// Analysis
	//
	//------------------------------
	//******************************
#ifdef DEPTH
	list<list<int> *> * result = NULL;
	if (Result->Result->size() > 0)
	{
		file << dec << Result->Return;
		file << "(";
		file << Result->Result->size();
		file << ")";
		assert(Result->Result->size() > 0);
		result = Result->Result;
	}
	else if (Result->Return < 0)
	{
		file << "-1";
		assert(Result->Result->size() == 0);
		//assert(!hasDuplicates(Result->Result));
		//assert(!sat->Evaluate(Result->Result));
		result = Result->Result;
	}
	else
	{
		file << "0";
		assert(Result->Result->size() == 0);
		//assert(!hasDuplicates(Result->Result));
		//assert(!sat->Evaluate(Result->Result));
		delete Result->Result;
	}

	list<unsigned int>::iterator y;

	file << "(";
	file << Result->AttemptCount;
	file << ")";

	file << "(";
	file << Result->EvalCount;
	file << ")";

	file << ",";
	delete Result;
	return result;
#else
	list <int> * result = NULL;
	if (Result->Return)
	{
		file << "1";
		assert(!hasDuplicates(Result->Result));
		assert(sat->Evaluate(Result->Result));
		result = Result->Result;
	}
	else if (Result->Equalility->empty() && Result->Undecidable->empty())
	{
		file << "-1";
		assert(!hasDuplicates(Result->Result));
		assert(!sat->Evaluate(Result->Result));
		delete Result->Result;
		result = new list <int>();
	}
	else
	{
		file << "0";
		assert(!hasDuplicates(Result->Result));
		assert(!sat->Evaluate(Result->Result));
		delete Result->Result;
	}

	list<unsigned int>::iterator y;

	//Output Undeciable list
	y = Result->Undecidable->begin();
	file << "(";
	if (y != Result->Undecidable->end())
	{
		file << *y;
		y++;
		for (; y != Result->Undecidable->end(); y++)
		{
			file << "|" << *y;
		}
	}
	file << ")";

	//Output Equalility list
	y = Result->Equalility->begin();
	file << "(";
	if (y != Result->Equalility->end())
	{
		file << *y;
		y++;
		for (; y != Result->Equalility->end(); y++)
		{
			file << "|" << *y;
		}
	}
	file << ")";

	file << ",";
	delete Result->Equalility;
	delete Result->Undecidable;
	delete Result;
	return result;
#endif
}

#ifdef DEPTH
static DepthPair ** Results;
#else
static Pair ** Results;
#endif

void SolveFunction(const SAT * sat, const int * SortFunctions, SortFunction * AllFunctions, const unsigned int i)
{
	list <SortFunction *> SortList;
	for (int y = 0; SortFunctions[y] != -1; y++) {
		SortList.push_back(&AllFunctions[SortFunctions[y]]);
	}
	SAT * copy = sat->copy();
	Results[i] = SAT_FUNCTION(copy, SOLUTION_FUNCTION, &SortList);
	if (Results[i]->Result->size() < 0 && terminateAllThreads) {
		delete Results[i]->Result;
#ifndef DEPTH
		delete Results[i]->Equalility;
		delete Results[i]->Undecidable;
#endif
		delete Results[i];
		Results[i] = NULL;
	}
	copy->cleanVariables();
	copy->cleanClauses();
	delete copy;
}

ReturnValue SATSolver::SolveTruth(ofstream & file) const
{
	list<int *> * SortFunctions = GetSortList();

#ifdef DEPTH
	Results = new DepthPair * [SortFunctions->size() + 1];
#else
	Results = new Pair * [SortFunctions->size() + 1];
#endif
	Results[SortFunctions->size()] = NULL;

	unsigned int i = 0;
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		list <SortFunction *> SortList;
		for (unsigned int y = 0; (*iter)[y] != -1; y++)
		{
			SortList.push_back(&AllFunctions[(*iter)[y]]);
		}
		Results[i++] = SAT_FUNCTION(sat, SOLUTION_FUNCTION, &SortList);
		sat->Revert();
		delete [] (*iter);
	}
	delete SortFunctions;

	return AnalysisResults(file, Results, this->sat);
}

ReturnValue SATSolver::ParallelSolveTruth(ofstream & file) const
{
	list<int *> * SortFunctions = GetSortList();
	thread ** threads = new thread * [SortFunctions->size() + 1];
#ifdef DEPTH
	Results = new DepthPair * [SortFunctions->size() + 1];
#else
	Results = new Pair * [SortFunctions->size() + 1];
#endif
	threads[SortFunctions->size()] = NULL;
	Results[SortFunctions->size()] = NULL;

	unsigned int i = 0;
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		assert(i < SortFunctions->size());
		threads[i] = new thread(SolveFunction, this->sat, (*iter), AllFunctions, i);	
		i++;
	}

	i = 0;
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		assert(i < SortFunctions->size());
		threads[i]->join();
		delete [] (*iter);
		delete threads[i];
		i++;
	}
	delete SortFunctions;
	delete [] threads;

	return AnalysisResults(file, Results, this->sat);
}
list<list<int> *> * SATSolver::StopAtFirstTruth(ofstream & file) const
{
	list<int *> * SortFunctions = GetSortList();

#ifdef DEPTH
	DepthPair * Result = NULL;
#else
	Pair * Result = NULL;
#endif
	list<list<int> *> * solution = NULL;

	unsigned int i = 0;
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		list <SortFunction *> SortList;
		for (unsigned int y = 0; (*iter)[y] != -1; y++)
		{
			SortList.push_back(&AllFunctions[(*iter)[y]]);
		}
		Result = SAT_FUNCTION(sat, SOLUTION_FUNCTION, &SortList);
		solution = AnalysisResult(file, Result, this->sat);
		if (solution != NULL) {
			break;
		}
		sat->Revert();
	}

	//remove the sort functions
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++) 
	{
		delete[](*iter);
	}
	delete SortFunctions;

	return solution;
}
list<list<int> *> * SATSolver::ParallelStopAtFirstTruth(ofstream & file) const
{
	terminateAllThreads = false;
	list<int *> * SortFunctions = GetSortList();
	thread ** threads = new thread *[SortFunctions->size() + 1];
#ifdef DEPTH
	DepthPair * Result = NULL;
	Results = new DepthPair *[SortFunctions->size() + 1];
#else
	Pair * Result = NULL;
	Results = new Pair *[SortFunctions->size() + 1];
#endif
	list<list<int> *> * solution = NULL;
	threads[SortFunctions->size()] = NULL;
	unsigned int i = 0;
	for (i = 0; i <= SortFunctions->size(); i++) {
		Results[i] = NULL;
	}

	i = 0;
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		assert(i < SortFunctions->size());
		threads[i] = new thread(SolveFunction, this->sat, (*iter), AllFunctions, i);
		i++;
	}

	while (true) {
		unsigned int y = 0;
		for (i = 0; i < SortFunctions->size(); i++)
		{
			assert(y <= i);
			assert(i < SortFunctions->size());
			if (Results[i] != NULL) {
				if (Results[i]->Return == 1 || Results[i]->Return == -1) {
					Result = Results[i];
					break;
				}
				y++;
			}
		}
		if (Result != NULL || y == i) {
			break;
		}
		//sleep 1 second
		Sleep(1000);
	}

	terminateAllThreads = true;

	i = 0;
	//terminate threads
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		threads[i]->join();
		delete threads[i];
		i++;
	}
	delete[] threads;

	if (Result == NULL) {
		Result = Results[0];
	}

	unsigned int y = 0;
	i = 0;
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		if (Result != Results[i] && Results[i] != NULL) {
			list<list<int> *> * result = AnalysisResult(file, Results[i], this->sat);
			if (result != NULL) {
#ifdef _DEBUG
				unsigned int z = 0;
#endif
				for (list<list<int> *>::const_iterator iter2 = result->cbegin(); iter2 != result->cend(); iter2++)
				{
					delete (*iter2);
#ifdef _DEBUG
					z++;
#endif
				}
#ifdef _DEBUG
				assert(z == result->size());
#endif
				delete result;
			}
		} else if (Result == Results[i]) {
			assert(Results[i] != NULL);
			solution = AnalysisResult(file, Results[i], this->sat);
			y = i;
		} else {
			file << ",";
		}
		delete[](*iter);
		i++;
	}
	file << y << ",";
	delete [] Results;
	delete SortFunctions;

	return solution;
}