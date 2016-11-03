#include <fstream>
#include <windows.h>

#include "SATSolver.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

SATSolver::SATSolver()
{
	this->isTerminatingAllThreads = true;
	this->threads = NULL;
	this->returnValues = NULL;
	this->sat = NULL;
	this->totalThreads = 0;
}
SATSolver::~SATSolver()
{
	if (this->totalThreads != 0)
	{
		this->terminateAllThreads();
	}

	assert(this->isTerminatingAllThreads == true);
	assert(this->threads == NULL);
	assert(this->returnValues == NULL);
	assert(this->sat == NULL);
	assert(this->totalThreads == 0);
}

void SATSolver::terminateAllThreads()
{
	this->isTerminatingAllThreads = true;

	// Wait for all thread to clean up
	volatile int * threads = (volatile int *)&this->totalThreads;
	while(true)
	{
		if (*threads == 0) 
		{
			break;
		}
		Sleep(100);
	}
}

Solution * SATSolver::analysisResults(ofstream & file, AnalysisFunction analysisFunction)
{
	assert(this->returnValues != NULL);

	Solution * solution = new Solution;
	solution->solved = NOT_COMPLETED;
	solution->solutions = NULL;
	list<const list <int> *> * deleteList = NULL;
	for(unsigned int i = 0; i < this->totalThreads; i++)
	{
		//Adjust the solution variable
		if (this->returnValues[i]->solved == NOT_COMPLETED_SOLUTION)
		{
			assert(solution->solved != COMPLETED_NO_SOLUTION);
			if (solution->solved != COMPLETED_SOLUTION)
			{
				if (solution->solutions == NULL)
				{
					solution->solutions = this->returnValues[i]->solutions;
				}
				else
				{
					if (solution->solutions->size() < this->returnValues[i]->solutions->size())
					{
						deleteList = solution->solutions;
						solution->solutions = this->returnValues[i]->solutions;
					}
					else
					{
						deleteList = this->returnValues[i]->solutions;
					}
				}
				solution->solved = this->returnValues[i]->solved;
			}
			else
			{
				deleteList = this->returnValues[i]->solutions;
			}
		}
		else if (this->returnValues[i]->solved == COMPLETED_SOLUTION)
		{
			assert(solution->solved != COMPLETED_NO_SOLUTION);
			if (solution->solved == COMPLETED_SOLUTION)
			{
				if (solution->solutions == NULL)
				{
					solution->solutions = this->returnValues[i]->solutions;
				}
				else
				{
					if (solution->solutions->size() > this->returnValues[i]->solutions->size())
					{
						deleteList = solution->solutions;
						solution->solutions = this->returnValues[i]->solutions;
					}
					else
					{
						deleteList = this->returnValues[i]->solutions;
					}
				}
			}
			else
			{
				deleteList = solution->solutions;
				solution->solutions = this->returnValues[i]->solutions;
			}
			solution->solved = this->returnValues[i]->solved;
		}
		else if (this->returnValues[i]->solved == COMPLETED_NO_SOLUTION)
		{
			assert(solution->solved != COMPLETED_SOLUTION && solution->solved != NOT_COMPLETED_SOLUTION);
			assert(this->returnValues[i]->solutions == NULL || this->returnValues[i]->solutions->size() == 0);
			deleteList = this->returnValues[i]->solutions;
			solution->solved = this->returnValues[i]->solved;
		}
		else if (this->returnValues[i]->solved == NOT_COMPLETED_NO_SOLUTION || this->returnValues[i]->solved == COMPLETED_UNKNOWN)
		{
			assert(this->returnValues[i]->solutions == NULL || this->returnValues[i]->solutions->size() == 0);
			deleteList = this->returnValues[i]->solutions;
			if (solution->solved == NOT_COMPLETED)
			{
				solution->solved = this->returnValues[i]->solved;
			}
		}
		else if (this->returnValues[i]->solved == NOT_COMPLETED)
		{
			assert(this->returnValues[i]->solutions == NULL || this->returnValues[i]->solutions->size() == 0);
			deleteList = this->returnValues[i]->solutions;
		}

		//Output other analysis to file
		file << this->returnValues[i]->solved;
		file << " (" << this->returnValues[i]->state->getState()->getVariableAttempts() << ")";
		if (analysisFunction != NULL)
		{
			analysisFunction(file, this->returnValues[i]);
		}
		file << ",";

		// Clean up the return variable
		assert(this->returnValues[i]->variables == NULL);
		delete this->returnValues[i]->state;
		if (deleteList != NULL)
		{
			list<const list <int> *>::const_iterator end = deleteList->cend();
			for (list<const list <int> *>::const_iterator iter = deleteList->cbegin(); iter != end; iter++)
			{
				delete *iter;
			}
			delete deleteList;
		}
		deleteList = NULL;

		delete this->returnValues[i];
	}

	// Clean up return values
	delete this->returnValues;
	this->returnValues = NULL;

	return solution;
}

Solution & SATSolver::runSolver(ofstream & file, const SAT * sat, void * variables, SolverFunction solverFunction, AnalysisFunction analysisFunction)
{
	//Check for legal start
	assert(sat != NULL);
	assert(solverFunction != NULL);
	assert(this->isTerminatingAllThreads == true);
	assert(this->threads == NULL);
	assert(this->returnValues == NULL);
	assert(this->sat == NULL);
	assert(this->totalThreads == 0);

	// initialize variables
	this->isTerminatingAllThreads = false;
	this->sat = sat;
	this->totalThreads = 1;
	this->returnValues = new ReturnValue *[1];

	// Run solution
	SATSolverState * startState = new SATSolverState(this->sat);
	this->returnValues[0] = solverFunction(this, startState, variables);
	delete variables;

	//Analyze Result
	Solution * finalState = this->analysisResults(file, analysisFunction);
	assert(this->returnValues == NULL);

	// Clean up remaining variables
	this->sat = NULL;
	this->totalThreads = 0;

	//Return result
	return *finalState;
}

void SATSolver::_runSolverParallel(SolverFunction solverFunction, const unsigned int currentThread, void * variables)
{
	SATSolverState * startState = new SATSolverState(this->sat);
	ReturnValue * value = solverFunction(this, startState, variables);
	if (value->terminateRemaingThreads)
	{
		this->isTerminatingAllThreads = true;
	}
	this->returnValues[currentThread] = value;
	delete variables;
}

Solution & SATSolver::runSolverParallel(ofstream & file, const SAT * sat, const unsigned int threadCount, void * variables, SolverCreator solverCreator, SolverFunction solverFunction, AnalysisFunction analysisFunction)
{
	//Check for legal start
	assert(sat != NULL);
	assert(threadCount > 0);
	assert(solverFunction != NULL);
	assert(this->isTerminatingAllThreads == true);
	assert(this->threads == NULL);
	assert(this->returnValues == NULL);
	assert(this->sat == NULL);
	assert(this->totalThreads == 0);

	// initialize variables
	this->isTerminatingAllThreads = false;
	this->sat = sat;
	this->totalThreads = threadCount;
	this->threads = new thread *[threadCount];
	this->returnValues = new ReturnValue *[threadCount];
	for(unsigned int i = 0; i < threadCount; i++)
	{
		this->threads[i] = NULL;
		this->returnValues[i] = NULL;
	}

	// Run solution
	this->isTerminatingAllThreads = false;
	for(unsigned int i = 0; i < threadCount - 1; i++)
	{
		this->threads[i] = new thread(&SATSolver::_runSolverParallel, this, solverFunction, i, variables);
		if (solverCreator != NULL)
		{
			variables = solverCreator(sat, i, threadCount, variables);
		}
	}
	this->_runSolverParallel(solverFunction, threadCount - 1, variables);


	// Clean up threads
	assert(this->isTerminatingAllThreads == true);
	for(unsigned int i = 0; i < threadCount - 1; i++)
	{
		this->threads[i]->join();
		delete this->threads[i];
	}
	delete this->threads;
	this->threads = NULL;

	//Analyze Result
	Solution * finalState = this->analysisResults(file, analysisFunction);
	assert(this->returnValues == NULL);

	// Clean up remaining variables
	this->sat = NULL;
	this->totalThreads = 0;

	//Return result
	return *finalState;
}