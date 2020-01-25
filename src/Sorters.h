#include "VariableState.h"
#include "SAT.h"
#include "Constants.h"

using namespace std;

VariableSolutions StatisticSolver(const VariableState* v);
VariableSolutions DefaultSolver(const VariableState * v);
VariableSolutions FlipSatSolver(const VariableState * v);

//Naming Convention
//Least - Smallest number (So LeastSmallest is not the largest of the smallest, it is the smallest of the smallest numbers)
//Most - Largest number (So the opposite of least)
enum class Sorter {
	NoFunction = -1,
	MostLargeUsed = 0,
	LeastLargeUsed = 1,
	MostSmallUsed = 2,
	LeastSmallUsed = 3,
	MostDifference = 4,
	LeastDifference = 5,
	MostSmallestClauseSize = 6,
	LeastSmallestClauseSize = 7,
	MostSmallestClauseCount = 8,
	LeastSmallestClauseCount = 9,
	MostLargestClauseSize = 10,
	LeastLargestClauseSize = 11,
	MostLargestClauseCount = 12,
	LeastLargestClauseCount = 13,
	MostSmallestNegativeClauseSize = 14,
	LeastSmallestNegativeClauseSize = 15,
	MostSmallestNegativeClauseCount = 16,
	LeastSmallestNegativeClauseCount = 17,
	MostLargestNegativeClauseSize = 18,
	LeastLargestNegativeClauseSize = 19,
	MostLargestNegativeClauseCount = 20,
	LeastLargestNegativeClauseCount = 21,
	MostSmallestPositiveClauseSize = 22,
	LeastSmallestPositiveClauseSize = 23,
	MostSmallestPositiveClauseCount = 24,
	LeastSmallestPositiveClauseCount = 25,
	MostLargestPositiveClauseSize = 26,
	LeastLargestPositiveClauseSize = 27,
	MostLargestPositiveClauseCount = 28,
	LeastLargestPositiveClauseCount = 29,
	MostTotalUsed = 30,
	LeastTotalUsed = 31,
	MostClauseCountSmallestToLargest = 32,
	MostClauseCountLargestToSmallest = 33,
	LeastClauseCountSmallestToLargest = 34,
	LeastClauseCountLargestToSmallest = 35,
	HasNoSolution = 36,
	HasRequired = 37,
#ifdef STATISTICS_STEPS
	MostAbsoluteScore = 38,
	LeastAbsoluteScore = 39
#endif
};

SortFunction* getSortFunction(Sorter sorter);