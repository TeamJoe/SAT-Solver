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

void analyzeBottomSat(ofstream & file, const ReturnValue * value)
{
}
void * createBottomSatVariable(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables)
{
	return NULL;
}
ReturnValue * solveBottomSat(const SATSolver * solver, void * variables)
{
	SATSolverState * solverState = new SATSolverState(solver->getSAT());

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

	const map <unsigned int, const VariableState *> * allVariables = solverState->getState()->getVariableMap();
	map <unsigned int, const VariableState *>::const_iterator end = allVariables->cend();
	for (map <unsigned int, const VariableState *>::const_iterator iter = allVariables->cbegin(); iter != end; iter++)
	{
		solverState->setVariable(iter->second->getVariable(), true);
	}

	bool hasSolution = false;
	bool isFinished = false;

	while (true)
	{
		isFinished = true;
		hasSolution = false;

		if (solverState->getState()->getRemainingClauseCount() == 0)
		{
			hasSolution = true;
		}
		for (map <unsigned int, const VariableState *>::const_iterator iter = allVariables->cbegin(); iter != end; iter++)
		{
			const Variable * currentVariable = iter->second->getVariable();
			int value = iter->second->getValue();
			solverState->unsetVariable(currentVariable);

			VariableSolutions solution = DefaultSolver(iter->second);
			if (solution == VariableSolutions::MUST_NEGATIVE || solution == VariableSolutions::MUST_POSITIVE)
			{
				if (value < 0 && (solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE))
				{
					isFinished = false;
				}
				else if (value > 0 && (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE))
				{
					isFinished = false;
				}
				solverState->setVariable(currentVariable, !(solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE));
			}
			else
			{
				solverState->setVariable(currentVariable, !(value < 0));
			}
			if (solution != VariableSolutions::VARIABLE_NO_SOLUTION && solution != VariableSolutions::MUST_NEGATIVE && solution != VariableSolutions::MUST_POSITIVE)
			{
				hasSolution = true;
			}

			
#ifdef OUTPUT_INTERMEDIATE_SOLUTION
			cout << value << "(" << solution << "): ";
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

			
		}

		if (isFinished)
		{
			break;
		}
	}	

	ReturnValue * value = new ReturnValue;
	value->solutions = NULL;
	value->state = solverState;
	value->solved = hasSolution ? SolvedStates::NOT_COMPLETED_SOLUTION : SolvedStates::NOT_COMPLETED_NO_SOLUTION;
	value->variables = NULL;
	value->terminateRemaingThreads = true;

	return value;
}