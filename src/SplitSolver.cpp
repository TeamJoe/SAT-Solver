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

struct SplitSatSolution
{
	SolverState state;
	list<list<int>*>* solutions;
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

list<list<int>*>* mergeSolutions(list<list<int>*>* l1, list<list<int>*>* l2);
SplitSatSolution* _solveSplitSatVerify(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
SplitSatSolution* _solveSplitSatNextVariable(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
SplitSatSolution* _solveSplitSatDepthSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const VariableState* var1, VariableSolutions solution, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
SplitSatSolution* _solveSplitSatSplitSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, list<SplitState*>* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth);
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
	SplitState* satState = new SplitState(value->state->getState());
	SplitSatSolution* i = _solveSplitSatVerify(value, solver, value->state, satState, depthSatVariables->SortFunctions, depthSatVariables->Decider, depthSatVariables->maxDepth);
	delete satState;
	if (i->state != SolverState::NO_SOLUTION_FOUND && i->state != SolverState::SOLUTION_FOUND)
	{
		if (i->solutions != NULL)
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
		if (i->solutions != NULL)
		{
			assert(i->state == SolverState::SOLUTION_FOUND);
			value->solved = SolvedStates::COMPLETED_SOLUTION;
		}
		else
		{
			assert(i->state != SolverState::SOLUTION_FOUND);
			value->solved = SolvedStates::COMPLETED_NO_SOLUTION;
		}
	}
	if (i->solutions != NULL)
	{
		for (list<list<int>*>::const_iterator iter = i->solutions->cbegin(); iter != i->solutions->cend(); iter++)
		{
			(*iter)->sort();
			int* solution = new int[(*iter)->size() + 2];
			solution[0] = (*iter)->size();
			int i = 1;
			for (list<int>::const_iterator valueIter1 = (*iter)->cbegin(); valueIter1 != (*iter)->cend(); valueIter1++)
			{
				solution[i++] = *valueIter1;
			}
			solution[i] = 0;
			value->solutions->push_back(solution);
			delete (*iter);
		}
		delete i->solutions;
	}
	delete i;
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

SplitSatSolution* _solveSplitSatVerify(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	if (satState->getRemainingClauseCount() == 0)
	{
		list<list<int>*>* value = new list<list<int>*>();
		value->push_back(satState->getState());
		return new SplitSatSolution{ SolverState::SOLUTION_FOUND, value };
	}
	else if (satState->getRemainingVariableCount() == 0)
	{
		return new SplitSatSolution{ SolverState::NO_SOLUTION_FOUND, NULL };
	}
	else if (!satState->canSolve())
	{
		return new SplitSatSolution{ SolverState::NO_SOLUTION_FOUND, NULL };
	}
	else if (0 < maxDepth && maxDepth < solverState->getState()->getVariableAttempts())
	{
		return new SplitSatSolution{ SolverState::MAX_DEPTH_REACHED, NULL };
	}
	else if (solver->isTerminatingAllThreads)
	{
		return new SplitSatSolution{ SolverState::TERMINATE_EARLY, NULL };
	}

	return _solveSplitSatNextVariable(value, solver, solverState, satState, SortFunctions, Decider, maxDepth);
}

SplitSatSolution* _solveSplitSatNextVariable(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	const VariableState* var1 = NextVariable(SortFunctions, Decider, satState->getVariableMap());
	assert(var1 != NULL);

	//Get best variable
	VariableSolutions solution = Decider(var1);
	assert(solution != VariableSolutions::VARIABLE_NO_SOLUTION);

	SplitSatSolution* i;
	if (solution == VariableSolutions::MUST_POSITIVE
		|| solution == VariableSolutions::MUST_NEGATIVE
		|| solution == VariableSolutions::VARIABLE_NO_SOLUTION)
	{
		i = _solveSplitSatDepthSolver(value, solver, solverState, satState, var1, solution, SortFunctions, Decider, maxDepth);
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

	return i;
}

bool compareSplitState(const SplitState* first, const SplitState* second)
{
	return first->getRemainingVariableCount() < second->getRemainingVariableCount();
}

SplitSatSolution* _solveSplitSatSplitSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, list<SplitState*>* satState, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	satState->sort(compareSplitState);
	SplitSatSolution* i = NULL;
	list<list<int>*>* solution = NULL;
	for (list<SplitState*>::const_iterator iter = satState->cbegin(); iter != satState->cend(); iter++)
	{
		i = _solveSplitSatVerify(value, solver, solverState, *iter, SortFunctions, Decider, maxDepth);
		if (i->state != SolverState::SOLUTION_FOUND) {
			break;
		}
		solution = mergeSolutions(solution, i->solutions);
		delete i;
		i = NULL;
	}
	for (list<SplitState*>::const_iterator iter = satState->cbegin(); iter != satState->cend(); iter++)
	{
		delete (*iter);
	}
	assert((i == NULL) != (solution == NULL));
	return i == NULL ? new SplitSatSolution{ SolverState::SOLUTION_FOUND, solution } : i;
}

SplitSatSolution* _solveSplitSatDepthSolver(ReturnValue* value, const SATSolver* solver, SATSolverState* solverState, SplitState* satState, const VariableState* var1, VariableSolutions solution, const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unsigned long long maxDepth)
{
	//Check if chain has a solution
	_setVariableSplitSat(solverState, var1, solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE);

	assert(var1->getValue() != 0);
	assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() > 0
		|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() < 0
		|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
		|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

	SplitSatSolution* i1 = _solveSplitSatVerify(value, solver, solverState, satState, SortFunctions, Decider, maxDepth);
#ifndef FIND_ALL_SOLUTIONS
	if (i1->state != SolverState::NO_SOLUTION_FOUND)
	{
#else
	if (i1->state != SolverState::NO_SOLUTION_FOUND && i1->state != SolverState::SOLUTION_FOUND)
	{
#endif
		solverState->unsetVariable(var1->getVariable());
		return i1;
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

		SplitSatSolution* i2 = _solveSplitSatVerify(value, solver, solverState, satState, SortFunctions, Decider, maxDepth);
		i1->solutions = mergeSolutions(i1->solutions, i2->solutions);
#ifndef FIND_ALL_SOLUTIONS
		if (i2->state != SolverState::NO_SOLUTION_FOUND)
		{
			assert(i1->state == SolverState::NO_SOLUTION_FOUND);
#else
		if (i2->state != SolverState::NO_SOLUTION_FOUND && i2 != SolverState::SOLUTION_FOUND)
		{
#endif
			assert(i1->state == SolverState::NO_SOLUTION_FOUND
				|| i1->state == SolverState::SOLUTION_FOUND);
			i1->state = i2->state;
		}
		else
		{
#ifdef FIND_ALL_SOLUTIONS
			assert(i1->state == SolverState::NO_SOLUTION_FOUND
				|| i1->state == SolverState::SOLUTION_FOUND);
			assert(i2->state == SolverState::NO_SOLUTION_FOUND
				|| i2->state == SolverState::SOLUTION_FOUND);
			i1->state = i1->state == SolverState::SOLUTION_FOUND
				|| i2->state == SolverState::SOLUTION_FOUND
				? SolverState::SOLUTION_FOUND
				: SolverState::NO_SOLUTION_FOUND;
#else
			assert(i1->state == SolverState::NO_SOLUTION_FOUND);
			assert(i2->state == SolverState::NO_SOLUTION_FOUND);
#endif
		}
		delete i2;
	}


	solverState->unsetVariable(var1->getVariable());
	return i1;
}

void _setVariableSplitSat(SATSolverState * solverState, const VariableState * var1, bool solution)
{
	solverState->setVariable(var1->getVariable(), solution);

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << var1->getValue() << "(" << solution << "): ";
	const list <const list <int>*>* currentClauses = &solverState->getState()->getRemainingClauses();
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

list<list<int>*>* mergeSolutions(list<list<int>*>* l1, list<list<int>*>* l2)
{
	if (l1 == NULL) 
	{
		return l2;
	} 
	else if (l2 == NULL) 
	{
		return l1;
	}
	else if (l1->size() < 1)
	{
		delete l1;
		return l2;
	}
	else if (l2->size() < 1)
	{
		delete l2;
		return l1;
	}
	else if (l2->size() == 1)
	{
		for (list<list<int>*>::const_iterator iter = l1->cbegin(); iter != l1->cend(); iter++)
		{
			for (list<int>::const_iterator valueIter = (*l2->cbegin())->cbegin(); valueIter != (*l2->cbegin())->cend(); valueIter++)
			{
				(*iter)->push_back(*valueIter);
			}
		}
		delete *(l2->cbegin());
		delete l2;
		return l1;
	}
	else if (l1->size() == 1)
	{
		for (list<list<int>*>::const_iterator iter = l2->cbegin(); iter != l2->cend(); iter++)
		{
			for (list<int>::const_iterator valueIter = (*l1->cbegin())->cbegin(); valueIter != (*l1->cbegin())->cend(); valueIter++)
			{
				(*iter)->push_back(*valueIter);
			}
		}
		delete* (l1->cbegin());
		delete l1;
		return l2;
	}
	else
	{
		list<list<int>*>* l3 = new list<list<int>*>();
		for (list<list<int>*>::const_iterator iter1 = l1->cbegin(); iter1 != l1->cend(); iter1++)
		{
			for (list<list<int>*>::const_iterator iter2 = l2->cbegin(); iter2 != l2->cend(); iter2++)
			{
				list<int>* solution = new list<int>();
				for (list<int>::const_iterator valueIter1 = (*iter1)->cbegin(); valueIter1 != (*iter1)->cend(); valueIter1++)
				{
					solution->push_back(*valueIter1);
				}
				for (list<int>::const_iterator valueIter2 = (*iter2)->cbegin(); valueIter2 != (*iter2)->cend(); valueIter2++)
				{
					solution->push_back(*valueIter2);
				}
				l3->push_back(solution);
			}
			delete (*iter1);
		}
		for (list<list<int>*>::const_iterator iter2 = l2->cbegin(); iter2 != l2->cend(); iter2++)
		{
			delete (*iter2);
		}
		delete l1;
		delete l2;
		return l3;
	}
}