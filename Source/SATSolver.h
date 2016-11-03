class SAT;
class SATSolver;
class SATSolverState;
struct ReturnValue;
struct Solution;

#include <ostream>
#include <fstream>

using namespace std;

typedef void * (*SolverCreator)(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables);
typedef ReturnValue * (*SolverFunction)(SATSolver * solver, SATSolverState * solverState, void * variables);
typedef void (*AnalysisFunction)(ofstream & file, const ReturnValue * value);

#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

#include <thread>

#include "SAT.h"
#include "SATSolverState.h"

#include "Constants.h"

using namespace std;

struct ReturnValue
{
	list<const list <int> *> * solutions;
	SATSolverState * state;
	SolvedStates solved;
	bool terminateRemaingThreads;
	void * variables;
};

struct Solution
{
	list<const list <int> *> * solutions;
	SolvedStates solved;
};

class SATSolver
{
private:
	thread ** threads;
	const SAT * sat;
	ReturnValue ** returnValues;
	unsigned int totalThreads;
protected:
	void _runSolverParallel(SolverFunction solverFunction, const unsigned int currentThread, void * variables);
	Solution * analysisResults(ofstream & file, AnalysisFunction analysisFunction);
public:

	//Constructors
	SATSolver();
	~SATSolver();

	volatile bool isTerminatingAllThreads;
	void terminateAllThreads();
	Solution & runSolver(ofstream & file, const SAT * sat, void * variables, SolverFunction solverFunction, AnalysisFunction analysisFunction);
	Solution & runSolverParallel(ofstream & file, const SAT * sat, const unsigned int threadCount, void * variables, SolverCreator solverCreator, SolverFunction solverFunction, AnalysisFunction analysisFunction);
};
#endif