#include <new>
#include <list>
#include <iostream>

#include "Sorters.h"
#include "SATState.h"
#include "SATSolverState.h"
#include "SATSolver.h"
#include "DepthSolver.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

struct DepthSatVariables
{
	list <SortFunction *> * SortFunctions;
	VariableSolutions (*Decider)(const VariableState *);
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

const VariableState * NextVariable(const list <SortFunction *> * SortFunctions, VariableSolutions (Decider)(const VariableState *), SATSolverState * solverState)
{
	const map <unsigned int, const VariableState *> * variables = solverState->getCurrentVariables();
	const VariableState * val = variables->cbegin()->second;
	
	VariableSolutions deciderInt = Decider(val);
	if (deciderInt == VariableSolutions::MUST_NEGATIVE || deciderInt == VariableSolutions::MUST_POSITIVE || deciderInt == VariableSolutions::VARIABLE_NO_SOLUTION) {
		return val;
	}

	map <unsigned int, const VariableState *>::const_iterator iter = variables->cbegin();
	map <unsigned int, const VariableState *>::const_iterator end = variables->cend();
	iter++;
	for(; iter != end; iter++)
	{
		const VariableState * val2 = iter->second;
		assert(val2->isActive());

		VariableSolutions deciderInt2 = Decider(val2);
		if (deciderInt2 == VariableSolutions::MUST_NEGATIVE || deciderInt2 == VariableSolutions::MUST_POSITIVE || deciderInt2 == VariableSolutions::VARIABLE_NO_SOLUTION) {
			return val2;
		}

		const VariableState * newVal = Sort(val, val2, SortFunctions);
		if (newVal != NULL) {
			val = newVal;
			deciderInt = deciderInt2;
		} else if (deciderInt2 != VariableSolutions::VARIABLE_UNKNOWN) {
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

int * getList(int v1, int v2, int v3) {
	int * var = new int [4];
	var[0] = v1;
	var[1] = v2;
	var[2] = v3;
	var[3] = -1;
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

int * getList(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8) {
	int * var = new int [9];
	var[0] = v1;
	var[1] = v2;
	var[2] = v3;
	var[3] = v4;
	var[4] = v5;
	var[5] = v6;
	var[6] = v7;
	var[7] = v8;
	var[8] = -1;
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
	//sortList->push_back(getList(36, 32, 4)); //HasSolution, MostClauseCountSmallestToLargest, MostDifference
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
	//******************************
	//------------------------------
	//
	// Fourth Set
	//
	//------------------------------
	//******************************
#ifdef FOURTH_SET
	sortList = new list<int *>();
	sortList->push_back(getList(36, 32, 4)); //HasSolution, MostClauseCountSmallestToLargest, MostDifference
	sortList->push_back(getList(36, 32, 5)); //HasSolution, MostClauseCountSmallestToLargest, LeastDifference
	sortList->push_back(getList(36, 33, 4)); //HasSolution, MostClauseCountLargestToSmallest, MostDifference
	/*
	sortList->push_back(getList(36, 33, 5)); //HasSolution, MostClauseCountLargestToSmallest, LeastDifference
	sortList->push_back(getList(36, 34, 4)); //HasSolution, LeastClauseCountSmallestToLargest, MostDifference
	sortList->push_back(getList(36, 34, 5)); //HasSolution, LeastClauseCountSmallestToLargest, LeastDifference
	sortList->push_back(getList(36, 35, 4)); //HasSolution, LeastClauseCountLargestToSmallest, MostDifference
	sortList->push_back(getList(36, 35, 5)); //HasSolution, LeastClauseCountLargestToSmallest, LeastDifference
	*/
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 4, 0)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostDifference, MostLargeUsed
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 4, 1)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostDifference, LeastLargeUsed
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 5, 0)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, LeastDifference, MostLargeUsed
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 5, 1)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, LeastDifference, LeastLargeUsed
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 0, 4)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostLargeUsed, MostDifference
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 1, 4)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, LeastLargeUsed, MostDifference
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 0, 5)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount, MostTotalUsed, LeastLargestClauseSize, LeastLargestClauseCount, MostLargeUsed, LeastDifference
	sortList->push_back(getList(36, 7, 8, 30, 11, 13, 1, 5)); //HasSolution, LeastSmallestClauseSize, MostSmallestClauseCount,  LeastLargestClauseSize, LeastLargestClauseCount, MostTotalUsed, LeastLargeUsed, LeastDifference
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


SolverState _solveDepthSat(ReturnValue * value, const SATSolver * solver, SATSolverState * solverState, const list <SortFunction *> * SortFunctions, VariableSolutions (Decider)(const VariableState *), const unsigned long long maxDepth);
ReturnValue * solveDepthSat(const SATSolver * solver, void * variables)
{
	DepthSatVariables * depthSatVariables = (DepthSatVariables *)variables;

	ReturnValue * value = new ReturnValue;
	value->solutions = new list<const int *>();
	value->state = new SATSolverState(solver->getSAT());

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << "(Start): ";
	const list <const list <int> *> * currentClauses = &value->state->getState()->getRemainingClauses();
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
	SolverState i = _solveDepthSat(value, solver, value->state, depthSatVariables->SortFunctions, depthSatVariables->Decider, depthSatVariables->maxDepth);
	if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND)
	{
		if (value->solutions->size() > 0)
		{
			value->solved = SolvedStates::NOT_COMPLETED_SOLUTION;
		}
		else
		{
			value->solved = SolvedStates::NOT_COMPLETED_NO_SOLUTION;
		}
	}
	else
	{
		if (value->solutions->size() > 0)
		{
			value->solved = SolvedStates::COMPLETED_SOLUTION;
		}
		else
		{
			value->solved = SolvedStates::COMPLETED_NO_SOLUTION;
		}
	}
	value->variables = NULL;


#ifndef COMPLETE
	value->terminateRemaingThreads = true;
#else
	value->terminateRemaingThreads = false;
#endif

	delete depthSatVariables->SortFunctions;
	delete depthSatVariables;

	return value;
}

SolverState _solveDepthSat(ReturnValue * value, const SATSolver * solver, SATSolverState * solverState, const list <SortFunction *> * SortFunctions, VariableSolutions (Decider)(const VariableState *), const unsigned long long maxDepth)
{
	//If all variable have a solution
	const SATState * satState = solverState->getState();
	if(satState->getRemainingVariableCount() == 0)
	{
		//If the solution is the opposite of what we currently have
		if (satState->getRemainingClauseCount() == 0)
		{
			try
			{
				value->solutions->push_back(satState->getState());
			}
			catch (std::bad_alloc& ba)
			{
				return SolverState::UNKNOWN_ERROR;
			}
#ifdef _DEBUG
			for (list<const int *>::const_iterator iter = value->solutions->cbegin(); iter != value->solutions->cend(); iter++)
			{
				assert(satState->getSAT()->Evaluate(*iter));
			}
#endif
			return SolverState::SOLUTION_FOUND;
		}
		if (solver->isTerminatingAllThreads) {
			return SolverState::TERMINATE_EARLY;
		}
		return SolverState::NO_SOLUTION_FOUND;
	}

	//exclude whole chains where every branch evaluates to true
	if (satState->getRemainingClauseCount() == 0)
	{
		try
		{
			value->solutions->push_back(satState->getState());
		}
		catch (std::bad_alloc& ba)
		{
			return SolverState::UNKNOWN_ERROR;
		}
#ifdef _DEBUG
		for (list<const int *>::const_iterator iter = value->solutions->cbegin(); iter != value->solutions->cend(); iter++)
		{
			assert(satState->getSAT()->Evaluate(*iter));
		}
#endif
		if (solver->isTerminatingAllThreads) {
			return SolverState::TERMINATE_EARLY;
		}
		return SolverState::SOLUTION_FOUND;
	}

	if (0 < maxDepth && maxDepth < satState->getVariableAttempts())
	{
		return SolverState::MAX_DEPTH_REACHED;
	}
	else if (solver->isTerminatingAllThreads)
	{
		return SolverState::TERMINATE_EARLY;
	}
	else if (!satState->canSolve()) 
	{
		return  SolverState::NO_SOLUTION_FOUND;
	}

	const VariableState * var1 = NextVariable(SortFunctions, Decider, solverState);
	assert(var1 != NULL);

	//Get best variable
	VariableSolutions solution = Decider(var1);
	assert(solution != VARIABLE_NO_SOLUTION);

	//Check if chain has a solution
	solverState->setVariable(var1->getVariable(), solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE);

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
	assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() > 0
		|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() < 0
		|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
		|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

	SolverState i = _solveDepthSat(value, solver, solverState, SortFunctions, Decider, maxDepth);
#ifdef END_ON_FIRST_SOLUTION
	if (i != SolverState::NO_SOLUTION_FOUND) {
#else
	if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND) {
#endif
		return i;
	}

	if (0 < maxDepth && maxDepth < satState->getVariableAttempts())
	{
		return SolverState::MAX_DEPTH_REACHED;
	}
	else if (solver->isTerminatingAllThreads)
	{
		return SolverState::TERMINATE_EARLY;
	}

	//Check if the opposite solution is plausible
	if(solution != VariableSolutions::MUST_POSITIVE && solution != VariableSolutions::MUST_NEGATIVE)
	{
		solverState->setVariable(var1->getVariable(), !(solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE));

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
		assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() < 0
			|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() > 0
			|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
			|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

		SolverState i = _solveDepthSat(value, solver, solverState, SortFunctions, Decider, maxDepth);
#ifdef END_ON_FIRST_SOLUTION
		if (i != SolverState::NO_SOLUTION_FOUND) {
#else
		if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND) {
#endif
			return i;
		}
	}

	//no solution found so step back and continue search
	solverState->unsetVariable(var1->getVariable());
	
	return SolverState::NO_SOLUTION_FOUND;
}