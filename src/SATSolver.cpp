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

#ifndef COMPLETE
	assert(this->isTerminatingAllThreads == true);
#endif
	assert(this->threads == NULL);
	assert(this->returnValues == NULL);
	assert(this->sat == NULL);
	assert(this->totalThreads == 0);
}

const SAT * SATSolver::getSAT() const
{
	return this->sat;
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

void cleanEachSolution(const ReturnValue * deleteValue)
{
	delete deleteValue->state;
	if (deleteValue->solutions != NULL)
	{
		list<const int *>::const_iterator end = deleteValue->solutions->cend();
		for (list<const int *>::const_iterator iter = deleteValue->solutions->cbegin(); iter != end; iter++)
		{
			delete [] *iter;
		}
		delete deleteValue->solutions;
	}

	delete deleteValue;
}

void SATSolver::cleanSolution()
{
	assert(this->threads == NULL);
	this->threads = new thread *[this->totalThreads];

	// Clean up the return variable
	for(unsigned int i = 0; i < this->totalThreads; i++)
	{
		this->threads[i] = NULL;
		assert(this->returnValues[i]->variables == NULL);
		if (i < this->totalThreads - 1)
		{
			this->threads[i] = new thread(&cleanEachSolution, this->returnValues[i]);
		}
		else
		{
			cleanEachSolution(this->returnValues[i]);
		}
	}

	//Clean up threads
	for(unsigned int i = 0; i < this->totalThreads - 1; i++)
	{
		this->threads[i]->join();
		delete this->threads[i];
	}
	delete [] this->threads;
	this->threads = NULL;

	// Clean up return values
	delete [] this->returnValues;
	this->returnValues = NULL;

	// Clean up remaining variables
	this->sat = NULL;
	this->totalThreads = 0;
}


Solution & SATSolver::analysisResults(ofstream & file, AnalysisFunction analysisFunction)
{
	assert(this->returnValues != NULL);

	Solution * solution = new Solution;
	solution->solved = SolvedStates::NOT_COMPLETED;
	solution->solutions = NULL;
	
	for(unsigned int i = 0; i < this->totalThreads; i++)
	{
		//Adjust the solution variable
		if (this->returnValues[i]->solved == SolvedStates::NOT_COMPLETED_SOLUTION)
		{
			assert(solution->solved != SolvedStates::COMPLETED_NO_SOLUTION);
			if (solution->solved != SolvedStates::COMPLETED_SOLUTION)
			{
				if (solution->solutions == NULL)
				{
					solution->solutions = this->returnValues[i]->solutions;
				}
				else
				{
					if (solution->solutions->size() < this->returnValues[i]->solutions->size())
					{
						solution->solutions = this->returnValues[i]->solutions;
					}
				}
				solution->solved = this->returnValues[i]->solved;
			}
		}
		else if (this->returnValues[i]->solved == SolvedStates::COMPLETED_SOLUTION)
		{
			assert(solution->solved != SolvedStates::COMPLETED_NO_SOLUTION);
			if (solution->solved == SolvedStates::COMPLETED_SOLUTION)
			{
				if (solution->solutions == NULL)
				{
					solution->solutions = this->returnValues[i]->solutions;
				}
				else
				{
					if (solution->solutions->size() > this->returnValues[i]->solutions->size())
					{
						solution->solutions = this->returnValues[i]->solutions;
					}
				}
			}
			else
			{
				solution->solutions = this->returnValues[i]->solutions;
			}
			solution->solved = this->returnValues[i]->solved;
		}
		else if (this->returnValues[i]->solved == SolvedStates::COMPLETED_NO_SOLUTION)
		{
			assert(solution->solved != SolvedStates::COMPLETED_SOLUTION && solution->solved != SolvedStates::NOT_COMPLETED_SOLUTION);
			assert(this->returnValues[i]->solutions == NULL || this->returnValues[i]->solutions->size() == 0);
			solution->solved = this->returnValues[i]->solved;
		}
		else if (this->returnValues[i]->solved == SolvedStates::NOT_COMPLETED_NO_SOLUTION || this->returnValues[i]->solved == SolvedStates::COMPLETED_UNKNOWN)
		{
			assert(this->returnValues[i]->solutions == NULL || this->returnValues[i]->solutions->size() == 0);
			if (solution->solved == SolvedStates::NOT_COMPLETED)
			{
				solution->solved = this->returnValues[i]->solved;
			}
		}
		else if (this->returnValues[i]->solved == SolvedStates::NOT_COMPLETED)
		{
			assert(this->returnValues[i]->solutions == NULL || this->returnValues[i]->solutions->size() == 0);
		}

		//Output other analysis to file
		file << static_cast<int>(this->returnValues[i]->solved);
		file << " (" << this->returnValues[i]->state->getState()->getVariableAttempts() << ")";
		if (this->returnValues[i]->solutions == NULL)
		{
			file << " (0)";
		}
		else
		{
			file << " (" << this->returnValues[i]->solutions->size() << ")";
		}
		if (analysisFunction != NULL)
		{
			analysisFunction(file, this->returnValues[i]);
		}
		file << ",";
	}

	return *solution;
}

void SATSolver::runSolver(const SAT * sat, void * variables, SolverFunction solverFunction)
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
	this->returnValues[0] = solverFunction(this, variables);
	delete variables;

	this->isTerminatingAllThreads = true;
}

void SATSolver::_runSolverParallel(SolverFunction solverFunction, const unsigned int currentThread, void * variables)
{
	ReturnValue * value = solverFunction(this, variables);
	if (value->terminateRemaingThreads)
	{
		this->isTerminatingAllThreads = true;
	}
	this->returnValues[currentThread] = value;
}

void SATSolver::runSolverParallel(const SAT * sat, const unsigned int threadCount, void * variables, SolverCreator solverCreator, SolverFunction solverFunction)
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
#ifndef COMPLETE
	assert(this->isTerminatingAllThreads == true);
#endif
	for(unsigned int i = 0; i < threadCount - 1; i++)
	{
		this->threads[i]->join();
		delete this->threads[i];
	}
	delete [] this->threads;
	this->threads = NULL;
}