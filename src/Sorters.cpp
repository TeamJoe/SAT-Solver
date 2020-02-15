#include "Sorters.h"
#include "VariableState.h"
#include "VariableState.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"


//******************************
//------------------------------
//
// Sorting Functions
//
//------------------------------
//******************************
const VariableState * HasNoSolution(const VariableState * v1, const VariableState * v2)
{
	return v1->hasSolution() ? v2 : v1;
}

const VariableState* HasRequired(const VariableState* v1, const VariableState* v2)
{
	return v1->getSmallestClauseSize() < 2
		|| v1->getNegativesSize() < 1
		|| v1->getPositivesSize() < 1
		? v1 : v2;
}

const VariableState* HasQuickPath(const VariableState* v1, const VariableState* v2)
{
	return v1->getSmallestClauseSize() < 3
		? v1 : v2;
}

const VariableState * LeastTotalUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getTotalNumber() < v2->getTotalNumber() ? v1 : v2;
}

const VariableState * MostTotalUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getTotalNumber() > v2->getTotalNumber() ? v1 : v2;
}

const VariableState * LeastDifference(const VariableState * v1, const VariableState * v2)
{
	if (v1->getDifference() > 0 && v2->getDifference() > 0)
	{
		return v1->getDifference() < v2->getDifference() ? v1 : v2;
	}
	return v1->getDifference() > 0 ? v1 : v2;
}

const VariableState * LeastSmallUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNumber() < v2->getSmallestNumber() ? v1 : v2;
}

const VariableState * LeastLargeUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNumber() < v2->getLargestNumber() ? v1 : v2;
}

const VariableState * MostDifference(const VariableState * v1, const VariableState * v2)
{
	return v1->getDifference() > v2->getDifference() ? v1 : v2;
}

const VariableState * MostSmallUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNumber() > v2->getSmallestNumber() ? v1 : v2;
}

const VariableState * MostLargeUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNumber() > v2->getLargestNumber() ? v1 : v2;
}

const VariableState * LeastSmallestClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestClauseSize() < v2->getSmallestClauseSize() ? v1 : v2;
}

const VariableState * LeastSmallestClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestClauseCount() < v2->getSmallestClauseCount() ? v1 : v2;
}

const VariableState * LeastLargestClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestClauseSize() < v2->getLargestClauseSize() ? v1 : v2;
}

const VariableState * LeastLargestClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestClauseCount() < v2->getLargestClauseCount() ? v1 : v2;
}

const VariableState * MostSmallestClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestClauseSize() > v2->getSmallestClauseSize() ? v1 : v2;
}

const VariableState * MostSmallestClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestClauseCount() > v2->getSmallestClauseCount() ? v1 : v2;
}

const VariableState * MostLargestClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestClauseSize() > v2->getLargestClauseSize() ? v1 : v2;
}

const VariableState * MostLargestClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestClauseCount() > v2->getLargestClauseCount() ? v1 : v2;
}

const VariableState * MostSmallestNegativeClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNegativeClauseSize() > v2->getSmallestNegativeClauseSize() ? v1 : v2;
}

const VariableState * LeastSmallestNegativeClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNegativeClauseSize() < v2->getSmallestNegativeClauseSize() ? v1 : v2;
}

const VariableState * MostSmallestNegativeClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNegativeClauseCount() > v2->getSmallestNegativeClauseCount() ? v1 : v2;
}

const VariableState * LeastSmallestNegativeClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNegativeClauseCount() < v2->getSmallestNegativeClauseCount() ? v1 : v2;
}

const VariableState * MostLargestNegativeClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNegativeClauseSize() > v2->getLargestNegativeClauseSize() ? v1 : v2;
}

const VariableState * LeastLargestNegativeClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNegativeClauseSize() < v2->getLargestNegativeClauseSize() ? v1 : v2;
}

const VariableState * MostLargestNegativeClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNegativeClauseCount() > v2->getLargestNegativeClauseCount() ? v1 : v2;
}

const VariableState * LeastLargestNegativeClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNegativeClauseCount() < v2->getLargestNegativeClauseCount() ? v1 : v2;
}

const VariableState * MostSmallestPositiveClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestPositiveClauseSize() > v2->getSmallestPositiveClauseSize() ? v1 : v2;
}

const VariableState * LeastSmallestPositiveClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestPositiveClauseSize() < v2->getSmallestPositiveClauseSize() ? v1 : v2;
}

const VariableState * MostSmallestPositiveClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestPositiveClauseCount() > v2->getSmallestPositiveClauseCount() ? v1 : v2;
}

const VariableState * LeastSmallestPositiveClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestPositiveClauseCount() < v2->getSmallestPositiveClauseCount() ? v1 : v2;
}

const VariableState * MostLargestPositiveClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestPositiveClauseSize() > v2->getLargestPositiveClauseSize() ? v1 : v2;
}

const VariableState * LeastLargestPositiveClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestPositiveClauseSize() < v2->getLargestPositiveClauseSize() ? v1 : v2;
}

const VariableState * MostLargestPositiveClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestPositiveClauseCount() > v2->getLargestPositiveClauseCount() ? v1 : v2;
}

const VariableState * LeastLargestPositiveClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestPositiveClauseCount() < v2->getLargestPositiveClauseCount() ? v1 : v2;
}

#ifdef  STATISTICS_STEPS
const VariableState* MostAbsoluteScore(const VariableState* v1, const VariableState* v2)
{
	return v1->getAbsoluteScore(STATISTICS_STEPS - 1) > v2->getAbsoluteScore(STATISTICS_STEPS - 1) ? v1 : v2;
}
const VariableState* LeastAbsoluteScore(const VariableState* v1, const VariableState* v2)
{
	return v1->getAbsoluteScore(STATISTICS_STEPS - 1) < v2->getAbsoluteScore(STATISTICS_STEPS - 1) ? v1 : v2;
}
#endif

//******************************
//------------------------------
//
// More Complex Functions
//
//------------------------------
//******************************
const VariableState * MostClauseCountSmallestToLargest(const VariableState * v1, const VariableState * v2)
{
	unsigned int size = v1->getLargestClauseSize() < v2->getLargestClauseSize() ? v2->getLargestClauseSize() : v1->getLargestClauseSize();
	for (unsigned int i = 1; i < size; i++)
	{
		unsigned int c1 = v1->getNegativeClauseCount(i) + v1->getPositiveClauseCount(i);
		unsigned int c2 = v2->getNegativeClauseCount(i) + v2->getPositiveClauseCount(i);
		if (c1 < c2) {
			return v2;
		} else if (c1 > c2) {
			return v1;
		}
	}
	// TODO: Should say here that they are equal
	return v1;
}

const VariableState * MostClauseCountLargestToSmallest(const VariableState * v1, const VariableState * v2)
{
	unsigned int size = v1->getLargestClauseSize() < v2->getLargestClauseSize() ? v2->getLargestClauseSize() : v1->getLargestClauseSize();
	for (unsigned int i = size; i > 0; i--)
	{
		unsigned int c1 = v1->getNegativeClauseCount(i) + v1->getPositiveClauseCount(i);
		unsigned int c2 = v2->getNegativeClauseCount(i) + v2->getPositiveClauseCount(i);
		if (c1 < c2) {
			return v2;
		} else if (c1 > c2) {
			return v1;
		}
	}
	// TODO: Should say here that they are equal
	return v1;
}

const VariableState * LeastClauseCountSmallestToLargest(const VariableState * v1, const VariableState * v2)
{
	const VariableState * v = MostClauseCountSmallestToLargest(v1, v2);
	if (v == v1) {
		return v2;
	} else {
		return v1;
	}
}

const VariableState * LeastClauseCountLargestToSmallest(const VariableState * v1, const VariableState * v2)
{
	const VariableState * v = MostClauseCountLargestToSmallest(v1, v2);
	if (v == v1) {
		return v2;
	} else {
		return v1;
	}
}

//******************************
//------------------------------
//
// Equaility Functions
//
//------------------------------
//******************************
bool HasNoSolutionCompare(const VariableState * v1, const VariableState * v2)
{
	return v1->hasSolution() && v2->hasSolution();
}

bool HasRequiredCompare(const VariableState* v1, const VariableState* v2)
{
	return v1->getSmallestClauseSize() > 1 && v2->getSmallestClauseSize() > 1
		&& v1->getNegativesSize() > 0 && v1->getPositivesSize() > 0
		&& v2->getNegativesSize() > 0 && v2->getPositivesSize() > 0;
}

bool HasQuickPathCompare(const VariableState* v1, const VariableState* v2)
{
	return (v1->getSmallestClauseSize() > 2 && v2->getSmallestClauseSize() > 2)
		|| (v1->getSmallestClauseSize() < 3 && v2->getSmallestClauseSize() < 3);
}

bool TotalUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getTotalNumber() == v2->getTotalNumber();
}

bool LargeUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNumber() == v2->getLargestNumber();
}

bool Difference(const VariableState * v1, const VariableState * v2)
{
	return v1->getDifference() == v2->getDifference();
}

bool SmallUsed(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNumber() == v2->getSmallestNumber();
}

bool SmallestClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestClauseSize() == v2->getSmallestClauseSize();
}

bool SmallestClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestClauseCount() == v2->getSmallestClauseCount();
}

bool LargestClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestClauseSize() == v2->getLargestClauseSize();
}

bool LargestClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestClauseCount() == v2->getLargestClauseCount();
}

bool SmallestNegativeClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNegativeClauseSize() == v2->getSmallestNegativeClauseSize();
}

bool SmallestNegativeClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestNegativeClauseCount() == v2->getSmallestNegativeClauseCount();
}

bool LargestNegativeClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNegativeClauseSize() == v2->getLargestNegativeClauseSize();
}

bool LargestNegativeClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestNegativeClauseCount() == v2->getLargestNegativeClauseCount();
}

bool SmallestPositiveClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestPositiveClauseSize() == v2->getSmallestPositiveClauseSize();
}

bool SmallestPositiveClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getSmallestPositiveClauseCount() == v2->getSmallestPositiveClauseCount();
}

bool LargestPositiveClauseSize(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestPositiveClauseSize() == v2->getLargestPositiveClauseSize();
}

bool LargestPositiveClauseCount(const VariableState * v1, const VariableState * v2)
{
	return v1->getLargestPositiveClauseCount() == v2->getLargestPositiveClauseCount();
}

#ifdef STATISTICS_STEPS
bool AbsoluteScore(const VariableState* v1, const VariableState* v2)
{
	return v1->getAbsoluteScore(STATISTICS_STEPS - 1) == v2->getAbsoluteScore(STATISTICS_STEPS - 1);
}
bool Percent1Score(const VariableState* v1, const VariableState* v2)
{
	double d1 = v1->getAbsoluteScore(STATISTICS_STEPS - 1);
	double d2 = v2->getAbsoluteScore(STATISTICS_STEPS - 1);
	return ((d1 * 0.99) <= d2) && (d2 <= (d1 * 1.01));
}
#endif

//******************************
//------------------------------
//
// More Complex Functions
//
//------------------------------
//******************************
bool AllClauseCounts(const VariableState * v1, const VariableState * v2)
{
	unsigned int size = v1->getLargestClauseSize() < v2->getLargestClauseSize() ? v2->getLargestClauseSize() : v1->getLargestClauseSize();
	for (unsigned int i = 1; i < size; i++)
	{
		unsigned int c1 = v1->getNegativeClauseCount(i) + v1->getPositiveClauseCount(i);
		unsigned int c2 = v2->getNegativeClauseCount(i) + v2->getPositiveClauseCount(i);
		if (c1 != c2) {
			return false;
		}
	}
	return true;
}


//******************************
//------------------------------
//
// Find const VariableState Value
//
//------------------------------
//******************************
#ifdef  STATISTICS_STEPS
VariableSolutions StatisticSolver(const VariableState* v)
{
	if (!v->hasSolution())
	{
		return VariableSolutions::VARIABLE_NO_SOLUTION;
	}
	else if (v->getSmallestNegativeClauseSize() == 1)
	{
		return VariableSolutions::MUST_NEGATIVE;
	}
	else if (v->getSmallestPositiveClauseSize() == 1)
	{
		return VariableSolutions::MUST_POSITIVE;
	}
	else if (v->getNegativesSize() == 0)
	{
		return VariableSolutions::MUST_POSITIVE;
	}
	else if (v->getPositivesSize() == 0)
	{
		return VariableSolutions::MUST_NEGATIVE;
	}
	else if (v->getScore(STATISTICS_STEPS - 1) != 0.0)
	{
		return v->getScore(STATISTICS_STEPS - 1) > 0.0
			? VariableSolutions::VARIABLE_POSITIVE
			: VariableSolutions::VARIABLE_NEGATIVE;
	}
	else
	{
		return VariableSolutions::VARIABLE_UNKNOWN;
	}
}
#endif

VariableSolutions DefaultSolver(const VariableState * v)
{
	if (!v->hasSolution())
	{
		return VariableSolutions::VARIABLE_NO_SOLUTION;
	}
	else if (v->getSmallestNegativeClauseSize() == 1)
	{
		return VariableSolutions::MUST_NEGATIVE;
	}
	else if (v->getSmallestPositiveClauseSize() == 1)
	{
		return VariableSolutions::MUST_POSITIVE;
	}
	else if (v->getNegativesSize() == 0)
	{
		return VariableSolutions::MUST_POSITIVE;
	}
	else if (v->getPositivesSize() == 0)
	{
		return VariableSolutions::MUST_NEGATIVE;
	}
	else if (v->getPositivesSize() != v->getNegativesSize())
	{
		return v->getPositivesSize() > v->getNegativesSize()
			? VariableSolutions::VARIABLE_POSITIVE
			: VariableSolutions::VARIABLE_NEGATIVE;
	}
	else
	{
		return VariableSolutions::VARIABLE_UNKNOWN;
	}
}

VariableSolutions FlipSatSolver(const VariableState * v)
{
	if (v->getPositivesSize() == v->getNegativesSize())
	{
		return VariableSolutions::VARIABLE_UNKNOWN;
	}
	else
	{
		return v->getPositivesSize() > v->getNegativesSize()
			? VariableSolutions::VARIABLE_POSITIVE
			: VariableSolutions::VARIABLE_NEGATIVE;
	}
}

//******************************
//------------------------------
//
// Sort/Equality Function
//
//------------------------------
//******************************

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

static SortFunction MostLargeUsed_Functions =						{ LargeUsed, MostLargeUsed };
static SortFunction LeastLargeUsed_Functions =						{ LargeUsed, LeastLargeUsed };
static SortFunction MostSmallUsed_Functions =						{ SmallUsed, MostSmallUsed };
static SortFunction LeastSmallUsed_Functions =						{ SmallUsed, LeastSmallUsed };
static SortFunction MostDifference_Functions =						{ Difference, MostDifference };
static SortFunction LeastDifference_Functions =						{ Difference, LeastDifference };
static SortFunction MostSmallestClauseSize_Functions =				{ SmallestClauseSize, MostSmallestClauseSize };
static SortFunction LeastSmallestClauseSize_Functions =				{ SmallestClauseSize, LeastSmallestClauseSize };
static SortFunction MostSmallestClauseCount_Functions =				{ SmallestClauseCount, MostSmallestClauseCount };
static SortFunction LeastSmallestClauseCount_Functions =			{ SmallestClauseCount, LeastSmallestClauseCount };
static SortFunction MostLargestClauseSize_Functions =				{ LargestClauseSize, MostLargestClauseSize };
static SortFunction LeastLargestClauseSize_Functions =				{ LargestClauseSize, LeastLargestClauseSize };
static SortFunction MostLargestClauseCount_Functions =				{ LargestClauseCount, MostLargestClauseCount };
static SortFunction LeastLargestClauseCount_Functions =				{ LargestClauseCount, LeastLargestClauseCount };
static SortFunction MostSmallestNegativeClauseSize_Functions =		{ SmallestNegativeClauseSize, MostSmallestNegativeClauseSize };
static SortFunction LeastSmallestNegativeClauseSize_Functions =		{ SmallestNegativeClauseSize, LeastSmallestNegativeClauseSize };
static SortFunction MostSmallestNegativeClauseCount_Functions =		{ SmallestNegativeClauseCount, MostSmallestNegativeClauseCount };
static SortFunction LeastSmallestNegativeClauseCount_Functions =	{ SmallestNegativeClauseCount, LeastSmallestNegativeClauseCount };
static SortFunction MostLargestNegativeClauseSize_Functions =		{ LargestNegativeClauseSize, MostLargestNegativeClauseSize };
static SortFunction LeastLargestNegativeClauseSize_Functions =		{ LargestNegativeClauseSize, LeastLargestNegativeClauseSize };
static SortFunction MostLargestNegativeClauseCount_Functions =		{ LargestNegativeClauseCount, MostLargestNegativeClauseCount };
static SortFunction LeastLargestNegativeClauseCount_Functions =		{ LargestNegativeClauseCount, LeastLargestNegativeClauseCount };
static SortFunction MostSmallestPositiveClauseSize_Functions =		{ SmallestPositiveClauseSize, MostSmallestPositiveClauseSize };
static SortFunction LeastSmallestPositiveClauseSize_Functions =		{ SmallestPositiveClauseSize, LeastSmallestPositiveClauseSize };
static SortFunction MostSmallestPositiveClauseCount_Functions =		{ SmallestPositiveClauseCount, MostSmallestPositiveClauseCount };
static SortFunction LeastSmallestPositiveClauseCount_Functions =	{ SmallestPositiveClauseCount, LeastSmallestPositiveClauseCount };
static SortFunction MostLargestPositiveClauseSize_Functions =		{ LargestPositiveClauseSize, MostLargestPositiveClauseSize };
static SortFunction LeastLargestPositiveClauseSize_Functions =		{ LargestPositiveClauseSize, LeastLargestPositiveClauseSize };
static SortFunction MostLargestPositiveClauseCount_Functions =		{ LargestPositiveClauseCount, MostLargestPositiveClauseCount };
static SortFunction LeastLargestPositiveClauseCount_Functions =		{ LargestPositiveClauseCount, LeastLargestPositiveClauseCount };
static SortFunction MostTotalUsed_Functions =						{ TotalUsed, MostTotalUsed };
static SortFunction LeastTotalUsed_Functions =						{ TotalUsed, LeastTotalUsed };
static SortFunction MostClauseCountSmallestToLargest_Functions =	{ AllClauseCounts, MostClauseCountSmallestToLargest };
static SortFunction MostClauseCountLargestToSmallest_Functions =	{ AllClauseCounts, MostClauseCountLargestToSmallest };
static SortFunction LeastClauseCountSmallestToLargest_Functions =	{ AllClauseCounts, LeastClauseCountSmallestToLargest };
static SortFunction LeastClauseCountLargestToSmallest_Functions =	{ AllClauseCounts, LeastClauseCountLargestToSmallest };
static SortFunction HasNoSolution_Functions =						{ HasNoSolutionCompare, HasNoSolution };
static SortFunction HasRequired_Functions =							{ HasRequiredCompare, HasRequired };
static SortFunction HasQuickPath_Functions =						{ HasQuickPathCompare, HasQuickPath };

#ifdef STATISTICS_STEPS
static SortFunction MostAbsoluteScore_Functions =					{ AbsoluteScore, MostAbsoluteScore };
static SortFunction LeastAbsoluteScore_Functions =					{ AbsoluteScore, LeastAbsoluteScore };
static SortFunction MostPercent1Score_Functions =					{ Percent1Score, MostAbsoluteScore };
static SortFunction LeastPercent1Score_Functions =					{ Percent1Score, LeastAbsoluteScore };
#endif

SortFunction* getSortFunction(Sorter sorter)
{
	switch (sorter) {
		case Sorter::NoFunction:						return NULL;
		case Sorter::MostLargeUsed:						return &MostLargeUsed_Functions;
		case Sorter::LeastLargeUsed:					return &LeastLargeUsed_Functions;
		case Sorter::MostSmallUsed:						return &MostSmallUsed_Functions;
		case Sorter::LeastSmallUsed:					return &LeastSmallUsed_Functions;
		case Sorter::MostDifference:					return &MostDifference_Functions;
		case Sorter::LeastDifference:					return &LeastDifference_Functions;
		case Sorter::MostSmallestClauseSize:			return &MostSmallestClauseSize_Functions;
		case Sorter::LeastSmallestClauseSize:			return &LeastSmallestClauseSize_Functions;
		case Sorter::MostSmallestClauseCount:			return &MostSmallestClauseCount_Functions;
		case Sorter::LeastSmallestClauseCount:			return &LeastSmallestClauseCount_Functions;
		case Sorter::MostLargestClauseSize:				return &MostLargestClauseSize_Functions;
		case Sorter::LeastLargestClauseSize:			return &LeastLargestClauseSize_Functions;
		case Sorter::MostLargestClauseCount:			return &MostLargestClauseCount_Functions;
		case Sorter::LeastLargestClauseCount:			return &LeastLargestClauseCount_Functions;
		case Sorter::MostSmallestNegativeClauseSize:	return &MostSmallestNegativeClauseSize_Functions;
		case Sorter::LeastSmallestNegativeClauseSize:	return &LeastSmallestNegativeClauseSize_Functions;
		case Sorter::MostSmallestNegativeClauseCount:	return &MostSmallestNegativeClauseCount_Functions;
		case Sorter::LeastSmallestNegativeClauseCount:	return &LeastSmallestNegativeClauseCount_Functions;
		case Sorter::MostLargestNegativeClauseSize:		return &MostLargestNegativeClauseSize_Functions;
		case Sorter::LeastLargestNegativeClauseSize:	return &LeastLargestNegativeClauseSize_Functions;
		case Sorter::MostLargestNegativeClauseCount:	return &MostLargestNegativeClauseCount_Functions;
		case Sorter::LeastLargestNegativeClauseCount:	return &LeastLargestNegativeClauseCount_Functions;
		case Sorter::MostSmallestPositiveClauseSize:	return &MostSmallestPositiveClauseSize_Functions;
		case Sorter::LeastSmallestPositiveClauseSize:	return &LeastSmallestPositiveClauseSize_Functions;
		case Sorter::MostSmallestPositiveClauseCount:	return &MostSmallestPositiveClauseCount_Functions;
		case Sorter::LeastSmallestPositiveClauseCount:	return &LeastSmallestPositiveClauseCount_Functions;
		case Sorter::MostLargestPositiveClauseSize:		return &MostLargestPositiveClauseSize_Functions;
		case Sorter::LeastLargestPositiveClauseSize:	return &LeastLargestPositiveClauseSize_Functions;
		case Sorter::MostLargestPositiveClauseCount:	return &MostLargestPositiveClauseCount_Functions;
		case Sorter::LeastLargestPositiveClauseCount:	return &LeastLargestPositiveClauseCount_Functions;
		case Sorter::MostTotalUsed:						return &MostTotalUsed_Functions;
		case Sorter::LeastTotalUsed:					return &LeastTotalUsed_Functions;
		case Sorter::MostClauseCountSmallestToLargest:	return &MostClauseCountSmallestToLargest_Functions;
		case Sorter::MostClauseCountLargestToSmallest:	return &MostClauseCountLargestToSmallest_Functions;
		case Sorter::LeastClauseCountSmallestToLargest:	return &LeastClauseCountSmallestToLargest_Functions;
		case Sorter::LeastClauseCountLargestToSmallest:	return &LeastClauseCountLargestToSmallest_Functions;
		case Sorter::HasNoSolution:						return &HasNoSolution_Functions;
		case Sorter::HasRequired:						return &HasRequired_Functions;
		case Sorter::HasQuickPath:						return &HasQuickPath_Functions;

#ifdef STATISTICS_STEPS
		case Sorter::MostAbsoluteScore:					return &MostAbsoluteScore_Functions;
		case Sorter::LeastAbsoluteScore:				return &LeastAbsoluteScore_Functions;
		case Sorter::MostPercent1Score:					return &MostPercent1Score_Functions;
		case Sorter::LeastPercent1Score:				return &LeastPercent1Score_Functions;
#endif

		default:
			assert(false);
			return NULL;
	}
}

//******************************
//------------------------------
//
// Method Functions
//
//------------------------------
//******************************

class Methods
{
public:
	const char* name;
	Sorter* sorters;

	~Methods()
	{
		delete[] sorters;
	}

	Methods(const char* name, Sorter* sorters) {
		this->name = name;
		this->sorters = sorters;
	}
};

list<Methods*>* GetSortList()
{
	list<Methods*>* sortList = NULL;

	//******************************
	//------------------------------
	//
	// Debug Set
	//
	//------------------------------
	//******************************
#ifdef _DEBUG
	sortList = new list<Methods*>();
	/*
	sortList->push_back(new Methods("DebugA1", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountSmallestToLargest, Sorter::MostDifference, Sorter::NoFunction }));
	*/
	sortList->push_back(new Methods("DebugA2", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
#else
	//******************************
	//------------------------------
	//
	// Orginal Set
	//
	//------------------------------
	//******************************
#ifdef ORGINAL_SET
	sortList = new list<Methods*>();
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::MostLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[3]{ Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
#endif
	//******************************
	//------------------------------
	//
	// Secondary Set
	//
	//------------------------------
	//******************************
#ifdef SECONDARY_SET
	sortList = new list<Methods*>();
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::MostLargeUsed, Sorter::MostDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[7]{ Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::NoFunction }));
#endif
	//******************************
	//------------------------------
	//
	// Third Set
	//
	//------------------------------
	//******************************
#ifdef THIRD_SET
	sortList = new list<Methods*>();
	sortList->push_back(new Methods("", new Sorter[8]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[8]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[8]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[8]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[8]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[8]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("", new Sorter[8]{ Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
#endif
	//******************************
	//------------------------------
	//
	// Fourth Set
	//
	//------------------------------
	//******************************
#ifdef FOURTH_SET
	sortList = new list<Methods*>();
	sortList->push_back(new Methods("SmallToLarge-1", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountSmallestToLargest, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("SmallToLarge-2", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountSmallestToLargest, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("LargeToSmall-1", new Sorter[6]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostClauseCountLargestToSmallest, Sorter::MostDifference, Sorter::NoFunction }));
	/*
	sortList->push_back(new Methods("LargeToSmall-2", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountLargestToSmallest, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("SmallToLarge-Odd-1", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountSmallestToLargest, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("SmallToLarge-Odd-2", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountSmallestToLargest, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("LargeToSmall-Odd-1", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountLargestToSmallest, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("LargeToSmall-Odd-2", new Sorter[5]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountLargestToSmallest, Sorter::LeastDifference, Sorter::NoFunction }));
	*/
	sortList->push_back(new Methods("ClauseSize-1", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ClauseSize-2", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ClauseSize-3", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ClauseSize-4", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ClauseSize-5", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ClauseSize-6", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ClauseSize-7", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ClauseSize-8", new Sorter[10]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));


	// TODO: Investigate quick path function
	//		Should probably have a cutoff, so it only happens if it affects a certain number of values?
	//		Or if it will actually force another variable to take a value
	//		Check Negative Clauses Size if Decider says variable is negative
	//		Check Positive Clause Size if the Decider says variable is positive

#ifdef STATISTICS_STEPS
	sortList->push_back(new Methods("Score-1", new Sorter[6]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::MostClauseCountSmallestToLargest, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-2", new Sorter[6]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::MostClauseCountSmallestToLargest, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-3", new Sorter[6]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::MostClauseCountLargestToSmallest, Sorter::MostDifference, Sorter::NoFunction }));

	sortList->push_back(new Methods("Score-4", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-5", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-6", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-7", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-8", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-9", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-10", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("Score-11", new Sorter[11]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));

	sortList->push_back(new Methods("ScoreQuick-1", new Sorter[7]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::MostClauseCountSmallestToLargest, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-2", new Sorter[7]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::MostClauseCountSmallestToLargest, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-3", new Sorter[7]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::MostClauseCountLargestToSmallest, Sorter::MostDifference, Sorter::NoFunction }));

	sortList->push_back(new Methods("ScoreQuick-4", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-5", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-6", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-7", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-8", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-9", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-10", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
	sortList->push_back(new Methods("ScoreQuick-11", new Sorter[12]{ Sorter::HasNoSolution, Sorter::HasRequired, Sorter::HasQuickPath, Sorter::MostPercent1Score, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::NoFunction }));
#endif
#endif


#endif
	return sortList;
}

Methods* getMethod(const unsigned int count)
{
	list<Methods*>* SortFunctions = GetSortList();
	Methods* ret = NULL;

	unsigned int i = 0;
	for (list <Methods*>::const_iterator iter = SortFunctions->cbegin(); iter != SortFunctions->cend(); iter++)
	{
		if (i == count)
		{
			assert(ret == NULL);
			ret = (*iter);
		}
		else
		{
			delete (*iter);
		}
		i++;
	}
	delete SortFunctions;
	return ret;
}

const char* getName(const unsigned int count)
{
	Methods* method = getMethod(count);
	assert(method != NULL);
	const char* ret = method->name;
	delete method;
	return ret;
}

//******************************
//------------------------------
//
// Determining Functions
//
//------------------------------
//******************************
const VariableState* Sort(const VariableState* var1, const VariableState* var2, const list <SortFunction*>* SortFunctions)
{
	list <SortFunction*>::const_iterator iter = SortFunctions->cbegin();
	for (; iter != SortFunctions->cend(); iter++)
	{
		assert((*iter) != NULL);
		if (!((*iter)->Eqaulity)(var1, var2))
		{
			return ((*iter)->Sort)(var1, var2);
		}
	}
	return NULL;
}

const VariableState* NextVariable(const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const map <unsigned int, const VariableState*>* variables)
{
	map <unsigned int, const VariableState*>::const_iterator iter = variables->cbegin();
	while (!iter->second->isActive() && iter != variables->cend()) {
		iter++;
	}
	assert(iter != variables->cend());
	const VariableState* val = iter->second;

	VariableSolutions deciderInt = Decider(val);
	if (deciderInt == VariableSolutions::MUST_NEGATIVE || deciderInt == VariableSolutions::MUST_POSITIVE || deciderInt == VariableSolutions::VARIABLE_NO_SOLUTION) {
		return val;
	}

	map <unsigned int, const VariableState*>::const_iterator end = variables->cend();
	iter++;
	for (; iter != end; iter++)
	{
		const VariableState* val2 = iter->second;
		if (val2->isActive()) {
			VariableSolutions deciderInt2 = Decider(val2);
			if (deciderInt2 == VariableSolutions::MUST_NEGATIVE || deciderInt2 == VariableSolutions::MUST_POSITIVE || deciderInt2 == VariableSolutions::VARIABLE_NO_SOLUTION) {
				return val2;
			}

			const VariableState* newVal = Sort(val, val2, SortFunctions);
			if (newVal != NULL) {
				val = newVal;
				deciderInt = deciderInt2;
			}
			else if (deciderInt2 != VariableSolutions::VARIABLE_UNKNOWN) {
				val = val2;
				deciderInt = deciderInt2;
			}
		}
	}
	return val;
}

const VariableState* NextVariable(const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), const unordered_map <unsigned int, const VariableState*>* variables)
{
	unordered_map <unsigned int, const VariableState*>::const_iterator iter = variables->cbegin();
	while (!iter->second->isActive() && iter != variables->cend()) {
		iter++;
	}
	assert(iter != variables->cend());
	const VariableState* val = iter->second;

	VariableSolutions deciderInt = Decider(val);
	if (deciderInt == VariableSolutions::MUST_NEGATIVE || deciderInt == VariableSolutions::MUST_POSITIVE || deciderInt == VariableSolutions::VARIABLE_NO_SOLUTION) {
		return val;
	}

	unordered_map <unsigned int, const VariableState*>::const_iterator end = variables->cend();
	iter++;
	for (; iter != end; iter++)
	{
		const VariableState* val2 = iter->second;
		if (val2->isActive()) {
			VariableSolutions deciderInt2 = Decider(val2);
			if (deciderInt2 == VariableSolutions::MUST_NEGATIVE || deciderInt2 == VariableSolutions::MUST_POSITIVE || deciderInt2 == VariableSolutions::VARIABLE_NO_SOLUTION) {
				return val2;
			}

			const VariableState* newVal = Sort(val, val2, SortFunctions);
			if (newVal != NULL) {
				val = newVal;
				deciderInt = deciderInt2;
			}
			else if (deciderInt2 != VariableSolutions::VARIABLE_UNKNOWN) {
				val = val2;
				deciderInt = deciderInt2;
			}
		}
	}
	return val;
}

list <SortFunction*>* getSortFunctions(const unsigned int count)
{
	Methods* method = getMethod(count);
	assert(method != NULL);
	list <SortFunction*>* ret = new list <SortFunction*>();
	for (unsigned int y = 0; method->sorters[y] != Sorter::NoFunction; y++)
	{
		SortFunction* sorter = getSortFunction(method->sorters[y]);
		if (sorter != NULL) {
			ret->push_back(sorter);
		}
	}
	delete method;
	return ret;
}