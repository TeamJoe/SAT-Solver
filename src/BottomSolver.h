class SAT;
class SATSolver;
struct ReturnValue;

#include <fstream>

void analyzeBottomSat(ofstream & file, const ReturnValue * value);
void * createBottomSatVariable(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables);
ReturnValue * solveBottomSat(const SATSolver * solver, void * variables);