#include "Sorters.h"
#include "Variable.h"
#include "VariableState.h"

//Memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Debug.h"

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif


//******************************
//------------------------------
//
// Sorting Functions
//
//------------------------------
//******************************
Variable * LeastDifference(Variable * v1, Variable * v2)
{
	if (v1->getCurrentState()->getDifference() > 0 && v2->getCurrentState()->getDifference() > 0)
	{
		return v1->getCurrentState()->getDifference() < v2->getCurrentState()->getDifference() ? v1 : v2;
	}
	return v1->getCurrentState()->getDifference() > 0 ? v1 : v2;
}

Variable * LeastSmallUsed(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestNumber() < v2->getCurrentState()->getSmallestNumber() ? v1 : v2;
}

Variable * LeastLargeUsed(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestNumber() < v2->getCurrentState()->getLargestNumber() ? v1 : v2;
}

Variable * MostDifference(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getDifference() > v2->getCurrentState()->getDifference() ? v1 : v2;
}

Variable * MostSmallUsed(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestNumber() > v2->getCurrentState()->getSmallestNumber() ? v1 : v2;
}

Variable * MostLargeUsed(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestNumber() > v2->getCurrentState()->getLargestNumber() ? v1 : v2;
}

Variable * LeastSmallestClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestClauseSize() < v2->getCurrentState()->getSmallestClauseSize() ? v1 : v2;
}

Variable * LeastSmallestClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestClauseCount() < v2->getCurrentState()->getSmallestClauseCount() ? v1 : v2;
}

Variable * LeastLargestClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestClauseSize() < v2->getCurrentState()->getLargestClauseSize() ? v1 : v2;
}

Variable * LeastLargestClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestClauseCount() < v2->getCurrentState()->getLargestClauseCount() ? v1 : v2;
}

Variable * MostSmallestClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestClauseSize() > v2->getCurrentState()->getSmallestClauseSize() ? v1 : v2;
}

Variable * MostSmallestClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestClauseCount() > v2->getCurrentState()->getSmallestClauseCount() ? v1 : v2;
}

Variable * MostLargestClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestClauseSize() > v2->getCurrentState()->getLargestClauseSize() ? v1 : v2;
}

Variable * MostLargestClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestClauseCount() > v2->getCurrentState()->getLargestClauseCount() ? v1 : v2;
}

Variable * MostSmallestNegativeClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestNegativeClauseSize() > v2->getCurrentState()->getSmallestNegativeClauseSize() ? v1 : v2;
}

Variable * LeastSmallestNegativeClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestNegativeClauseSize() < v2->getCurrentState()->getSmallestNegativeClauseSize() ? v1 : v2;
}

Variable * MostSmallestNegativeClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestNegativeClauseCount() > v2->getCurrentState()->getSmallestNegativeClauseCount() ? v1 : v2;
}

Variable * LeastSmallestNegativeClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestNegativeClauseCount() < v2->getCurrentState()->getSmallestNegativeClauseCount() ? v1 : v2;
}

Variable * MostLargestNegativeClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestNegativeClauseSize() > v2->getCurrentState()->getLargestNegativeClauseSize() ? v1 : v2;
}

Variable * LeastLargestNegativeClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestNegativeClauseSize() < v2->getCurrentState()->getLargestNegativeClauseSize() ? v1 : v2;
}

Variable * MostLargestNegativeClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestNegativeClauseCount() > v2->getCurrentState()->getLargestNegativeClauseCount() ? v1 : v2;
}

Variable * LeastLargestNegativeClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestNegativeClauseCount() < v2->getCurrentState()->getLargestNegativeClauseCount() ? v1 : v2;
}

Variable * MostSmallestPositiveClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestPositiveClauseSize() > v2->getCurrentState()->getSmallestPositiveClauseSize() ? v1 : v2;
}

Variable * LeastSmallestPositiveClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestPositiveClauseSize() < v2->getCurrentState()->getSmallestPositiveClauseSize() ? v1 : v2;
}

Variable * MostSmallestPositiveClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestPositiveClauseCount() > v2->getCurrentState()->getSmallestPositiveClauseCount() ? v1 : v2;
}

Variable * LeastSmallestPositiveClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getSmallestPositiveClauseCount() < v2->getCurrentState()->getSmallestPositiveClauseCount() ? v1 : v2;
}

Variable * MostLargestPositiveClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestPositiveClauseSize() > v2->getCurrentState()->getLargestPositiveClauseSize() ? v1 : v2;
}

Variable * LeastLargestPositiveClauseSize(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestPositiveClauseSize() < v2->getCurrentState()->getLargestPositiveClauseSize() ? v1 : v2;
}

Variable * MostLargestPositiveClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestPositiveClauseCount() > v2->getCurrentState()->getLargestPositiveClauseCount() ? v1 : v2;
}

Variable * LeastLargestPositiveClauseCount(Variable * v1, Variable * v2)
{
	return v1->getCurrentState()->getLargestPositiveClauseCount() < v2->getCurrentState()->getLargestPositiveClauseCount() ? v1 : v2;
}

//******************************
//------------------------------
//
// More Complex Functions
//
//------------------------------
//******************************
Variable * ClauseCountSmallestToLargest(Variable * v1, Variable * v2)
{
	const VariableState * vs1 = v1->getCurrentState();
	const VariableState * vs2 = v2->getCurrentState();
	int size = vs1->getLargestClauseSize() < vs2->getLargestClauseSize() ? vs2->getLargestClauseSize() : vs1->getLargestClauseSize();
	for (int i = 0; i < size; i++)
	{
		int c1 = vs1->getNegativeClauseCount(i) + vs1->getPositiveClauseCount(i);
		int c2 = vs2->getNegativeClauseCount(i) + vs2->getPositiveClauseCount(i);
		if (c1 == c2) {
			continue;
		} else {
			return c1 > c2 ? v1 : v2;
		}
	}
	// TODO: Should say here that they are equal
	return v1;
}

//******************************
//------------------------------
//
// Equaility Functions
//
//------------------------------
//******************************

bool LargeUsed(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestNumber() == v2->getCurrentState()->getLargestNumber();
}

bool Difference(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getDifference() == v2->getCurrentState()->getDifference();
}

bool SmallUsed(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getSmallestNumber() == v2->getCurrentState()->getSmallestNumber();
}

bool SmallestClauseSize(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getSmallestClauseSize() == v2->getCurrentState()->getSmallestClauseSize();
}

bool SmallestClauseCount(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getSmallestClauseCount() == v2->getCurrentState()->getSmallestClauseCount();
}

bool LargestClauseSize(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestClauseSize() == v2->getCurrentState()->getLargestClauseSize();
}

bool LargestClauseCount(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestClauseCount() == v2->getCurrentState()->getLargestClauseCount();
}

bool SmallestNegativeClauseSize(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getSmallestNegativeClauseSize() == v2->getCurrentState()->getSmallestNegativeClauseSize();
}

bool SmallestNegativeClauseCount(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getSmallestNegativeClauseCount() == v2->getCurrentState()->getSmallestNegativeClauseCount();
}

bool LargestNegativeClauseSize(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestNegativeClauseSize() == v2->getCurrentState()->getLargestNegativeClauseSize();
}

bool LargestNegativeClauseCount(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestNegativeClauseCount() == v2->getCurrentState()->getLargestNegativeClauseCount();
}

bool SmallestPositiveClauseSize(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getSmallestPositiveClauseSize() == v2->getCurrentState()->getSmallestPositiveClauseSize();
}

bool SmallestPositiveClauseCount(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getSmallestPositiveClauseCount() == v2->getCurrentState()->getSmallestPositiveClauseCount();
}

bool LargestPositiveClauseSize(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestPositiveClauseSize() == v2->getCurrentState()->getLargestPositiveClauseSize();
}

bool LargestPositiveClauseCount(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestPositiveClauseCount() == v2->getCurrentState()->getLargestPositiveClauseCount();
}


bool LargestPositiveClauseCount(const Variable * v1, const Variable * v2)
{
	return v1->getCurrentState()->getLargestPositiveClauseCount() == v2->getCurrentState()->getLargestPositiveClauseCount();
}

//******************************
//------------------------------
//
// More Complex Functions
//
//------------------------------
//******************************
bool ClauseCountSmallestToLargest(const Variable * v1, const Variable * v2)
{
	const VariableState * vs1 = v1->getCurrentState();
	const VariableState * vs2 = v2->getCurrentState();
	int size = vs1->getLargestClauseSize() < vs2->getLargestClauseSize() ? vs2->getLargestClauseSize() : vs1->getLargestClauseSize();
	for (int i = 0; i < size; i++)
	{
		int c1 = vs1->getNegativeClauseCount(i) + vs1->getPositiveClauseCount(i);
		int c2 = vs2->getNegativeClauseCount(i) + vs2->getPositiveClauseCount(i);
		if (c1 == c2) {
			continue;
		} else {
			return false;
		}
	}
	return true;
}


//******************************
//------------------------------
//
// Find Variable Value
//
//------------------------------
//******************************
int DefaultSolver(Variable * v)
{
	if (v->getCurrentState()->getSmallestNegativeClauseSize() == 0 || v->getCurrentState()->getSmallestPositiveClauseSize() == 0)
	{
		return VARIABLE_NO_SOLUTION;
	}
	if (v->getCurrentState()->getSmallestNegativeClauseSize() == 1 && v->getCurrentState()->getSmallestPositiveClauseSize() == 1)
	{
		return VARIABLE_NO_SOLUTION;
	}
	else if (v->getCurrentState()->getSmallestNegativeClauseSize() == 1)
	{
		return MUST_NEGATIVE;
	}
	else if (v->getCurrentState()->getSmallestPositiveClauseSize() == 1)
	{
		return MUST_POSITIVE;
	}
	else if (v->getCurrentState()->getPositivesSize() == v->getCurrentState()->getNegativesSize())
	{
		return VARIABLE_UNKNOWN;
	}
	else
	{
		return v->getCurrentState()->getPositivesSize() > v->getCurrentState()->getNegativesSize()
			? VARIABLE_POSITIVE
			: VARIABLE_NEGATIVE;
	}
}
int FlipSatSolver(Variable * v)
{
	if (v->getCurrentState()->getPositivesSize() == v->getCurrentState()->getNegativesSize())
	{
		return VARIABLE_UNKNOWN;
	}
	else
	{
		return v->getCurrentState()->getPositivesSize() > v->getCurrentState()->getNegativesSize()
			? VARIABLE_POSITIVE
			: VARIABLE_NEGATIVE;
	}
}