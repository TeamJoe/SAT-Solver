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
// Sorting Function 
//
//------------------------------
//******************************

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
