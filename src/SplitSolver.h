class SAT;
class SATSolver;
struct ReturnValue;

#include <fstream>

void analyzeSplitSat(ofstream& file, const ReturnValue* value);
void* createSplitSatVariable(const SAT* sat, const unsigned int currentCount, const unsigned int totalCount, const void* variables);
ReturnValue* solveSplitSat(const SATSolver* solver, void* variables);