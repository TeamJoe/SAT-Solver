#include <new>
#include <list>
#include <iostream>
#include <string.h>

#include "Sorters.h"
#include "SATState.h"
#include "SplitState.h"
#include "SATSolverState.h"
#include "SATSolver.h"
#include "SATSolver.h"
#include "SplitSolver.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

struct SplitSatVariables
{
	const char* name;
	list <SortFunction*>* SortFunctions;
	VariableSolutions(*Decider)(const VariableState*);
	unsigned long long maxDepth;
	unsigned int sortCount;
};

void analyzeSplitSat(ofstream& file, const ReturnValue* value)
{

}

void* createSplitSatVariable(const SAT* sat, const unsigned int currentCount, const unsigned int totalCount, const void* variables)
{
	SplitSatVariables* depthVariables = new SplitSatVariables;
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


SolverState _solveSplitSatVerify(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
SolverState _solveSplitSatNextVariable(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
SolverState _solveSplitSatDepthSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const VariableState* var1, VariableSolutions solution, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
SolverState _solveSplitSatSplitSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, list<SplitState*>* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
void _setVariableSplitSat(SATSolverState* solverState, const VariableState* var1, bool solution);
ReturnValue* solveSplitSat(const SATSolver* solver, void* variables)
{
	SplitSatVariables* depthSatVariables = (SplitSatVariables*)variables;

	ReturnValue* value = new ReturnValue;
	value->name = depthSatVariables->name;
	value->solutions = new list<const int*>();
	value->state = new SATSolverState(solver->getSAT());

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << "(Start): ";
	const list <const list <int>*>* currentClauses = &value->state->getState()->getRemainingClauses();
	for (list <const list <int>*>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
	{
		cout << "(";
		for (list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
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
	SolverState i = _solveSplitSatVerify(value, solver, value->state, new SplitState(value->state->getState()), depthSatVariables->SortFunctions, depthSatVariables->Decider, depthSatVariables->maxDepth);
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

SolverState _solveSplitSatVerify(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	if (satState->getRemainingClauseCount() == 0)
	{
		// TODO: Something to merge solutions;
		return SolverState::SOLUTION_FOUND;
	}
	else if (satState->getRemainingVariableCount() == 0)
	{
		return SolverState::NO_SOLUTION_FOUND;
	}
	else if (!satState->canSolve())
	{
		return  SolverState::NO_SOLUTION_FOUND;
	}
	else if (0 < maxDepth && maxDepth < solverState->getState()->getVariableAttempts())
	{
		return SolverState::MAX_DEPTH_REACHED;
	}
	else if (solver->isTerminatingAllThreads)
	{
		return SolverState::TERMINATE_EARLY;
	}

	return _solveSplitSatNextVariable(value, solver, solverState, satState, SortFunctions, Decider, maxDepth);
}

SolverState _solveSplitSatNextVariable(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	const VariableState* var1 = NextVariable(SortFunctions, Decider, satState->getVariableMap());
	assert(var1 != NULL);

	//Get best variable
	VariableSolutions solution = Decider(var1);
	assert(solution != VariableSolutions::VARIABLE_NO_SOLUTION);

	SolverState i;
	if (solution == VariableSolutions::MUST_POSITIVE
		|| solution == VariableSolutions::MUST_NEGATIVE
		|| solution == VariableSolutions::VARIABLE_NO_SOLUTION)
	{
		_setVariableSplitSat(solverState, var1, solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE);

		assert(var1->getValue() != 0);
		assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() > 0
			|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() < 0
			|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
			|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

		i = _solveSplitSatVerify(value, solver, solverState, satState, SortFunctions, Decider, maxDepth);
	}
	else
	{
		list<SplitState*>* split = satState->getSplit();
		if (split != NULL)
		{
			i = _solveSplitSatSplitSolver(value, solver, solverState, split, SortFunctions, Decider, maxDepth);
			delete split;
		}
		else 
		{
			i = _solveSplitSatDepthSolver(value, solver, solverState, satState, var1, solution, SortFunctions, Decider, maxDepth);
		}
	}

#ifndef FIND_ALL_SOLUTIONS
	if (i != SolverState::NO_SOLUTION_FOUND)
	{
#else
	if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND)
	{
#endif
		return i;
	}
	assert(!(0 < maxDepth && maxDepth < solverState->getState()->getVariableAttempts()));

	//no solution found so step back and continue search
	solverState->unsetVariable(var1->getVariable());

	return SolverState::NO_SOLUTION_FOUND;
}

bool compareSplitState(const SplitState* first, const SplitState* second)
{
	return first->getRemainingVariableCount() < second->getRemainingVariableCount();
}

SolverState _solveSplitSatSplitSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, list<SplitState*>* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	satState->sort(compareSplitState);
	SolverState i;
	for (list<SplitState*>::const_iterator iter = satState->cbegin(); iter != satState->cend(); iter++)
	{
		i = _solveSplitSatVerify(value, solver, solverState, *iter, SortFunctions, Decider, maxDepth);
		if (i != SolverState::SOLUTION_FOUND) {
			break;
		}
	}
	for (list<SplitState*>::const_iterator iter = satState->cbegin(); iter != satState->cend(); iter++)
	{
		delete (*iter);
	}
	return i;
}

SolverState _solveSplitSatDepthSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const VariableState* var1, VariableSolutions solution, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	//Check if chain has a solution
	_setVariableSplitSat(solverState, var1, solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE);

	assert(var1->getValue() != 0);
	assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() > 0
		|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() < 0
		|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
		|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

	SolverState i = _solveSplitSatVerify(value, solver, solverState, satState, SortFunctions, Decider, maxDepth);
#ifndef FIND_ALL_SOLUTIONS
	if (i != SolverState::NO_SOLUTION_FOUND)
	{
#else
	if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND)
	{
#endif
		return i;
	}
	assert(!(0 < maxDepth && maxDepth < solverState->getState()->getVariableAttempts()));

	//Check if the opposite solution is plausible
	if (solution != VariableSolutions::MUST_POSITIVE && solution != VariableSolutions::MUST_NEGATIVE)
	{
		_setVariableSplitSat(solverState, var1, !(solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE));

		assert(var1->getValue() != 0);
		assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() < 0
			|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() > 0
			|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
			|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

		i = _solveSplitSatVerify(value, solver, solverState, satState, SortFunctions, Decider, maxDepth);
	}

	return i;
}

void _setVariableSplitSat(SATSolverState * solverState, const VariableState * var1, bool solution)
{
	solverState->setVariable(var1->getVariable(), solution);

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << var1->getValue() << "(" << solution << "): ";
	const list <const list <int>*>* currentClauses = &satState->getRemainingClauses();
	for (list <const list <int>*>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
	{
		cout << "(";
		for (list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
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
}