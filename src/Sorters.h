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
	MostLargeUsed,
	LeastLargeUsed,
	MostSmallUsed,
	LeastSmallUsed,
	MostDifference,
	LeastDifference,
	MostSmallestClauseSize,
	LeastSmallestClauseSize,
	MostSmallestClauseCount,
	LeastSmallestClauseCount,
	MostLargestClauseSize,
	LeastLargestClauseSize,
	MostLargestClauseCount,
	LeastLargestClauseCount,
	MostSmallestNegativeClauseSize,
	LeastSmallestNegativeClauseSize,
	MostSmallestNegativeClauseCount,
	LeastSmallestNegativeClauseCount,
	MostLargestNegativeClauseSize,
	LeastLargestNegativeClauseSize,
	MostLargestNegativeClauseCount,
	LeastLargestNegativeClauseCount,
	MostSmallestPositiveClauseSize,
	LeastSmallestPositiveClauseSize,
	MostSmallestPositiveClauseCount,
	LeastSmallestPositiveClauseCount,
	MostLargestPositiveClauseSize,
	LeastLargestPositiveClauseSize,
	MostLargestPositiveClauseCount,
	LeastLargestPositiveClauseCount,
	MostTotalUsed,
	LeastTotalUsed,
	MostClauseCountSmallestToLargest,
	MostClauseCountLargestToSmallest,
	LeastClauseCountSmallestToLargest,
	LeastClauseCountLargestToSmallest,
	HasNoSolution,
	HasRequired,
	HasQuickPath,
	MostAbsoluteScore,
	LeastAbsoluteScore,
	MostPercent1Score,
	LeastPercent1Score
};

SortFunction* getSortFunction(Sorter sorter);