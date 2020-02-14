#include <new>
#include <list>
#include <iostream>
#include <string.h>

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
	const char* name;
	list <SortFunction *> * SortFunctions;
	VariableSolutions (*Decider)(const VariableState *);
	unsigned long long maxDepth;
	unsigned int sortCount;
};

void analyzeDepthSat(ofstream & file, const ReturnValue * value)
{

}

void * createDepthSatVariable(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables)
{
	DepthSatVariables * depthVariables = new DepthSatVariables;
	depthVariables->name = getName(currentCount);
	depthVariables->SortFunctions = getSortFunctions(currentCount);
	depthVariables->maxDepth = MAX_DEPTH_LIMIT;

#ifdef STATISTICS_STEPS
	depthVariables->Decider = &StatisticSolver;
#else
	depthVariables->Decider = &DefaultSolver;
#endif

	return depthVariables;
}


SolverState _solveDepthSat(ReturnValue * value, const SATSolver * solver, SATSolverState * solverState, const list <SortFunction *> * SortFunctions, VariableSolutions (Decider)(const VariableState *), const unsigned long long maxDepth);
ReturnValue * solveDepthSat(const SATSolver * solver, void * variables)
{
	DepthSatVariables * depthSatVariables = (DepthSatVariables *)variables;

	ReturnValue * value = new ReturnValue;
	value->name = depthSatVariables->name;
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


#ifndef LET_ALL_SOLVERS_FINISH
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
	assert(solution != VariableSolutions::VARIABLE_NO_SOLUTION);

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
#ifndef FIND_ALL_SOLUTIONS
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
#ifndef FIND_ALL_SOLUTIONS
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