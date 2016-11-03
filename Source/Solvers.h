#include "SATSolver.h"
#include "SATSolverState.h"

void analyzeDepthSat(ofstream & file, const ReturnValue * value);
void * createDepthSatVariable(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables);
ReturnValue * DepthSat(SATSolver * solver, SATSolverState * solverState, void * variables);