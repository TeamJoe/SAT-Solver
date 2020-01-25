#include "Sorters.h"
#include "VariableState.h"
#include "VariableState.h"
#include "Constants.h"

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
	return v1->getSmallestClauseSize() == 1 ? v1 : v2;
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
	return v1->getSmallestClauseSize() != 1 && v2->getSmallestClauseSize() != 1;
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
	else if (v->getProbabiltyPositive(STATISTICS_STEPS - 1) != 0.5)
	{
		return v->getProbabiltyPositive(STATISTICS_STEPS - 1) > 0.5
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