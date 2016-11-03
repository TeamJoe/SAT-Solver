class SAT;
class SATSolver;
struct ReturnValue;

#include <fstream>

void analyzeDepthSat(ofstream & file, const ReturnValue * value);
void * createDepthSatVariable(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables);
ReturnValue * solveDepthSat(const SATSolver * solver, void * variables);