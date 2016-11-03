#include <list>
#include <iostream>

#include "Sorters.h"
#include "SATState.h"
#include "SATSolverState.h"
#include "SATSolver.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

struct DepthSatVariables
{
	list <SortFunction *> * SortFunctions;
	int (*Decider)(const VariableState *);
	unsigned long long maxDepth;
	unsigned int sortCount;
};

const VariableState * Sort(const VariableState * var1, const VariableState * var2, const list <SortFunction *> * SortFunctions)
{
	list <SortFunction *>::const_iterator iter = SortFunctions->cbegin();
	for(; iter != SortFunctions->cend(); iter++)
	{
		if(!((*iter)->Eqaulity)(var1, var2))
		{
			return ((*iter)->Sort)(var1, var2);
		}
	}
	return NULL;
}

const VariableState * NextVariable(const list <SortFunction *> * SortFunctions, int (Decider)(const VariableState *), SATSolverState * solverState)
{
	const map <unsigned int, const VariableState *> * variables = solverState->getCurrentVariables();
	const VariableState * val = variables->cbegin()->second;
	
	int deciderInt = Decider(val);
	if (deciderInt == MUST_NEGATIVE || deciderInt == MUST_POSITIVE || deciderInt == VARIABLE_NO_SOLUTION) {
		return val;
	}

	map <unsigned int, const VariableState *>::const_iterator iter = variables->cbegin();
	map <unsigned int, const VariableState *>::const_iterator end = variables->cend();
	iter++;
	for(; iter != end; iter++)
	{
		const VariableState * val2 = iter->second;
		assert(val2->isActive());

		int deciderInt2 = Decider(val2);
		if (deciderInt2 == MUST_NEGATIVE || deciderInt2 == MUST_POSITIVE || deciderInt2 == VARIABLE_NO_SOLUTION) {
			return val2;
		}

		const VariableState * newVal = Sort(val, val2, SortFunctions);
		if (newVal != NULL) {
			val = newVal;
			deciderInt = deciderInt2;
		} else if (deciderInt2 != VARIABLE_UNKNOWN) {
			val = val2;
			deciderInt = deciderInt2;
		}
	}
	return val;
}

int * getList(int v1, int v2) {
	int * var = new int [3];
	var[0] = v1;
	var[1] = v2;
	var[2] = -1;
	return var;
}

int * getList(int v1, int v2, int v3, int v4, int v5, int v6) {
	int * var = new int [7];
	var[0] = v1;
	var[1] = v2;
	var[2] = v3;
	var[3] = v4;
	var[4] = v5;
	var[5] = v6;
	var[6] = -1;
	return var;
}

int * getList(int v1, int v2, int v3, int v4, int v5, int v6, int v7) {
	int * var = new int [8];
	var[0] = v1;
	var[1] = v2;
	var[2] = v3;
	var[3] = v4;
	var[4] = v5;
	var[5] = v6;
	var[6] = v7;
	var[7] = -1;
	return var;
}

list<int *> * GetSortList()
{
	list<int *> * sortList = NULL;

	//******************************
	//------------------------------
	//
	// Debug Set
	//
	//------------------------------
	//******************************
#ifdef _DEBUG
	sortList = new list<int *>();
	sortList->push_back(getList(7, 8, 11, 12, 4, 0)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostDifference, MostLargeUsed
#else
	//******************************
	//------------------------------
	//
	// Orginal Set
	//
	//------------------------------
	//******************************
#ifdef ORGINAL_SET
	sortList = new list<int *>();
	sortList->push_back(getList(4, 0)); //MostDifference, MostLargeUsed
	sortList->push_back(getList(4, 1)); //MostDifference, LeastLargeUsed
	sortList->push_back(getList(5, 0)); //LeastDifference, MostLargeUsed
	sortList->push_back(getList(5, 1)); //LeastDifference, LeastLargeUsed
	sortList->push_back(getList(0, 4)); //MostLargeUsed, MostDifference
	sortList->push_back(getList(1, 4)); //LeastLargeUsed, MostDifference
	sortList->push_back(getList(0, 5)); //MostLargeUsed, LeastDifference
	sortList->push_back(getList(1, 5)); //LeastLargeUsed, LeastDifference
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
	sortList->push_back(getList(7, 8, 11, 12, 4, 0)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostDifference, MostLargeUsed
	sortList->push_back(getList(7, 8, 11, 12, 4, 1)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostDifference, LeastLargeUsed
	sortList->push_back(getList(7, 8, 11, 12, 5, 0)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastDifference, MostLargeUsed
	sortList->push_back(getList(7, 8, 11, 12, 5, 1)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastDifference, LeastLargeUsed
	sortList->push_back(getList(7, 8, 11, 12, 0, 4)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostLargeUsed, MostDifference
	sortList->push_back(getList(7, 8, 11, 12, 1, 4)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastLargeUsed, MostDifference
	sortList->push_back(getList(7, 8, 11, 12, 0, 5)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, MostLargeUsed, LeastDifference
	sortList->push_back(getList(7, 8, 11, 12, 1, 5)); //LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount, LeastLargeUsed, LeastDifference
	sortList->push_back(getList(4, 0, 7, 8, 11, 12)); //MostDifference, MostLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(getList(4, 1, 7, 8, 11, 12)); //MostDifference, LeastLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(getList(5, 0, 7, 8, 11, 12)); //LeastDifference, MostLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(getList(5, 1, 7, 8, 11, 12)); //LeastDifference, LeastLargeUsed, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(getList(0, 4, 7, 8, 11, 12)); //MostLargeUsed, MostDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(getList(1, 4, 7, 8, 11, 12)); //LeastLargeUsed, MostDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(getList(0, 5, 7, 8, 11, 12)); //MostLargeUsed, LeastDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
	sortList->push_back(getList(1, 5, 7, 8, 11, 12)); //LeastLargeUsed, LeastDifference, LeastSmallestClauseSize, MostSmallestClauseCount, LeastLargestClauseSize, MostLargestClauseCount
#endif
	//******************************
	//------------------------------
	//
	// Third Set
	//
	//------------------------------
	//******************************
#ifdef THIRD_SET
	sortList = new list<int *>();
	sortList->push_back(getList(7, 8, 30, 11, 13, 4, 0)); //LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostDifference, MostLargeUsed
	sortList->push_back(getList(7, 8, 30, 11, 13, 4, 1)); //LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostDifference, LeastLargeUsed
	sortList->push_back(getList(7, 8, 30, 11, 13, 5, 0)); //LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, LeastDifference, MostLargeUsed
	sortList->push_back(getList(7, 8, 30, 11, 13, 5, 1)); //LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, LeastDifference, LeastLargeUsed
	sortList->push_back(getList(7, 8, 30, 11, 13, 0, 4)); //LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostLargeUsed, MostDifference
	sortList->push_back(getList(7, 8, 30, 11, 13, 1, 4)); //LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, LeastLargeUsed, MostDifference
	sortList->push_back(getList(7, 8, 30, 11, 13, 0, 5)); //LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostLargeUsed, LeastDifference
	sortList->push_back(getList(7, 8, 30, 11, 13, 1, 5)); //LeastSmallestClauseSize, MostSmallestClauseCount,  LeastLargestClauseSize, LeastLargestClauseCount, MostTotalUsed, LeastLargeUsed, LeastDifference
#endif
#endif
	return sortList;
}

list <SortFunction *> * getSortFunctions(const unsigned int count)
{
	list<int *> * SortFunctions = GetSortList();
	list <SortFunction *> * ret = NULL;

	unsigned int i = 0;
	for (list <int *>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		if (i == count)
		{
			assert(ret == NULL);
			list <SortFunction *> * SortList = new list <SortFunction *>();
			for (unsigned int y = 0; (*iter)[y] != -1; y++)
			{
				SortList->push_back(&AllFunctions[(*iter)[y]]);
			}
			ret = SortList;
		}
		delete [] *iter;
		i++;
	}
	delete SortFunctions;
	return ret;
}

void analyzeDepthSat(ofstream & file, const ReturnValue * value)
{

}

void * createDepthSatVariable(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables)
{
	DepthSatVariables * depthVariables = new DepthSatVariables;
	depthVariables->SortFunctions = getSortFunctions(currentCount);
	depthVariables->maxDepth = MAX_DEPTH_LIMIT;
	depthVariables->Decider = &DefaultSolver;

	return depthVariables;
}


int _DepthSat(ReturnValue * value, SATSolver * solver, SATSolverState * solverState, const list <SortFunction *> * SortFunctions, int (Decider)(const VariableState *), const unsigned long long maxDepth);
ReturnValue * DepthSat(SATSolver * solver, SATSolverState * solverState, void * variables)
{
	DepthSatVariables * depthSatVariables = (DepthSatVariables *)variables;

	ReturnValue * value = new ReturnValue;
	value->solutions = new list<const list <int> *>();
	value->state = solverState;

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << "(Start): ";
	const list <const list <int> *> * currentClauses = &solverState->getState()->getRemainingClauses();
	for(list <const list <int> *>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
	{
		cout << "(";
		for(list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
		{
			if (iter2 != (*(iter))->cbegin()) {
				cout << ",";
			}
			cout << (*(iter2));
		}
		cout << ")";
		delete (*(iter));
	}
	cout << endl;
	delete currentClauses;
#endif

	//Run the evaluation
	int i = _DepthSat(value, solver, solverState, depthSatVariables->SortFunctions, depthSatVariables->Decider, depthSatVariables->maxDepth);
	if (i < 0)
	{
		if (value->solutions->size() > 0)
		{
			value->solved = NOT_COMPLETED_SOLUTION;
		}
		else
		{
			value->solved = NOT_COMPLETED_NO_SOLUTION;
		}
	}
	else
	{
		if (value->solutions->size() > 0)
		{
			value->solved = COMPLETED_SOLUTION;
		}
		else
		{
			value->solved = COMPLETED_NO_SOLUTION;
		}
	}
	value->variables = NULL;


#ifndef COMPLETE
	value->terminateRemaingThreads = true;
#else
	value->terminateRemaingThreads = false;
#endif

	delete depthSatVariables->SortFunctions;

	return value;
}

int _DepthSat(ReturnValue * value, SATSolver * solver, SATSolverState * solverState, const list <SortFunction *> * SortFunctions, int (Decider)(const VariableState *), const unsigned long long maxDepth)
{
	//If all variable have a solution
	const SATState * satState = solverState->getState();
	if(satState->getRemainingVariableCount() == 0)
	{
		//If the solution is the opposite of what we currently have
		if (satState->getRemainingClauseCount() == 0)
		{
			value->solutions->push_back(&satState->getState());
#ifdef _DEBUG
			for (list<const list<int> *>::const_iterator iter = value->solutions->cbegin(); iter != value->solutions->cend(); iter++)
			{
				assert(satState->getSAT()->Evaluate(*iter));
			}
#endif
			return 1;
		}
		if (solver->isTerminatingAllThreads) {
			return -2;
		}
		return 0;
	}

	//exclude whole chains where every branch evaluates to true
	if (satState->getRemainingClauseCount() == 0)
	{
		value->solutions->push_back(&satState->getState());
#ifdef _DEBUG
		for (list<const list<int> *>::const_iterator iter = value->solutions->cbegin(); iter != value->solutions->cend(); iter++)
		{
			assert(satState->getSAT()->Evaluate(*iter));
		}
#endif
		if (solver->isTerminatingAllThreads) {
			return -2;
		}
		return 1;
	}

	if (maxDepth < satState->getVariableAttempts())
	{
		return -1;
	}
	else if (solver->isTerminatingAllThreads)
	{
		return -2;
	}
	else if (!satState->canSolve()) 
	{
		return 0;
	}

	const VariableState * var1 = NextVariable(SortFunctions, Decider, solverState);
	assert(var1 != NULL);

	//Get best variable
	int solution = Decider(var1);
	assert(solution != VARIABLE_NO_SOLUTION);

	//Check if chain has a solution
	solverState->setVariable(var1->getVariable(), solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE);

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << var1->getValue() << "(" << solution << "): ";
	const list <const list <int> *> * currentClauses = &satState->getRemainingClauses();
	for(list <const list <int> *>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
	{
		cout << "(";
		for(list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
		{
			if (iter2 != (*(iter))->cbegin()) {
				cout << ",";
			}
			cout << (*(iter2));
		}
		cout << ")";
		delete (*(iter));
	}
	cout << endl;
	delete currentClauses;
#endif

	assert(var1->getValue() != 0);
	assert((solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE) && var1->getValue() > 0 
		|| (solution == VARIABLE_NEGATIVE || solution == MUST_NEGATIVE) && var1->getValue() < 0 
		|| (solution == VARIABLE_UNKNOWN && var1->getValue() != 0)
		|| (solution == VARIABLE_NO_SOLUTION && var1->getValue() != 0));

	int i = _DepthSat(value, solver, solverState, SortFunctions, Decider, maxDepth);
#ifdef END_ON_FIRST_SOLUTION
	if (i != 0) {
#else
	if (i < 0) {
#endif
		return i;
	}

	if (maxDepth < satState->getVariableAttempts())
	{
		return -1;
	}
	else if (solver->isTerminatingAllThreads)
	{
		return -2;
	}

	//Check if the opposite solution is plausible
	if(solution != MUST_POSITIVE && solution != MUST_NEGATIVE)
	{
		solverState->setVariable(var1->getVariable(), !(solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE));

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
		cout << var1->getValue() << "(" << solution << "): ";
		const list <const list <int> *> * currentClauses = &satState->getRemainingClauses();
		for(list <const list <int> *>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
		{
			cout << "(";
			for(list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
			{
				if (iter2 != (*(iter))->cbegin()) {
					cout << ",";
				}
				cout << (*(iter2));
			}
			cout << ")";
			delete (*(iter));
		}
		cout << endl;
		delete currentClauses;
#endif

		assert(var1->getValue() != 0);
		assert((solution == VARIABLE_POSITIVE || solution == MUST_POSITIVE) && var1->getValue() < 0 
			|| (solution == VARIABLE_NEGATIVE || solution == MUST_NEGATIVE) && var1->getValue() > 0 
			|| (solution == VARIABLE_UNKNOWN && var1->getValue() != 0)
			|| (solution == VARIABLE_NO_SOLUTION && var1->getValue() != 0));

		int i = _DepthSat(value, solver, solverState, SortFunctions, Decider, maxDepth);
#ifdef END_ON_FIRST_SOLUTION
		if (i != 0) {
#else
		if (i < 0) {
#endif
			return i;
		}
	}

	//no solution found so step back and continue search
	solverState->unsetVariable(var1->getVariable());
	
	return 0;
}