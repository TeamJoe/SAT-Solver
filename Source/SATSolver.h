class SAT;
class SATSolver;
class SATSolverState;
struct ReturnValue;
struct Solution;

#include <ostream>
#include <fstream>

using namespace std;

typedef void * (*SolverCreator)(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables);
typedef ReturnValue * (*SolverFunction)(const SATSolver * solver, void * variables);
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
	list<const int *> * solutions;
	SATSolverState * state;
	SolvedStates solved;
	bool terminateRemaingThreads;
	void * variables;
};

struct Solution
{
	list<const int *> * solutions;
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
public:

	//Constructors
	SATSolver();
	~SATSolver();

	const SAT * getSAT() const;

	volatile bool isTerminatingAllThreads;
	void terminateAllThreads();
	void cleanSolution();
	Solution & analysisResults(ofstream & file, AnalysisFunction analysisFunction);
	void runSolver(const SAT * sat, void * variables, SolverFunction solverFunction);
	void runSolverParallel(const SAT * sat, const unsigned int threadCount, void * variables, SolverCreator solverCreator, SolverFunction solverFunction);
};
#endif