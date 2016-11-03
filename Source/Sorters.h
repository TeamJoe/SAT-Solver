#include "VariableState.h"
#include "SAT.h"
#include "Constants.h"

using namespace std;

const VariableState * LeastTotalUsed(const VariableState * v1, const VariableState * v2);
const VariableState * MostTotalUsed(const VariableState * v1, const VariableState * v2);
const VariableState * LeastDifference(const VariableState * v1, const VariableState * v2);
const VariableState * LeastSmallUsed(const VariableState * v1, const VariableState * v2);
const VariableState * LeastLargeUsed(const VariableState * v1, const VariableState * v2);
const VariableState * MostDifference(const VariableState * v1, const VariableState * v2);
const VariableState * MostSmallUsed(const VariableState * v1, const VariableState * v2);
const VariableState * MostLargeUsed(const VariableState * v1, const VariableState * v2);
const VariableState * LeastSmallestClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * LeastSmallestClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * LeastLargestClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * LeastLargestClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * MostSmallestClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * MostSmallestClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * MostLargestClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * MostLargestClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * MostSmallestNegativeClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * LeastSmallestNegativeClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * MostSmallestNegativeClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * LeastSmallestNegativeClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * MostLargestNegativeClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * LeastLargestNegativeClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * MostLargestNegativeClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * LeastLargestNegativeClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * MostSmallestPositiveClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * LeastSmallestPositiveClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * MostSmallestPositiveClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * LeastSmallestPositiveClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * MostLargestPositiveClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * LeastLargestPositiveClauseSize(const VariableState * v1, const VariableState * v2);
const VariableState * MostLargestPositiveClauseCount(const VariableState * v1, const VariableState * v2);
const VariableState * LeastLargestPositiveClauseCount(const VariableState * v1, const VariableState * v2);

bool TotalUsed(const VariableState * v1, const VariableState * v2);
bool LargeUsed(const VariableState * v1, const VariableState * v2);
bool Difference(const VariableState * v1, const VariableState * v2);
bool SmallUsed(const VariableState * v1, const VariableState * v2);
bool SmallestClauseSize(const VariableState * v1, const VariableState * v2);
bool SmallestClauseCount(const VariableState * v1, const VariableState * v2);
bool LargestClauseSize(const VariableState * v1, const VariableState * v2);
bool LargestClauseCount(const VariableState * v1, const VariableState * v2);
bool SmallestNegativeClauseSize(const VariableState * v1, const VariableState * v2);
bool SmallestNegativeClauseCount(const VariableState * v1, const VariableState * v2);
bool LargestNegativeClauseSize(const VariableState * v1, const VariableState * v2);
bool LargestNegativeClauseCount(const VariableState * v1, const VariableState * v2);
bool SmallestPositiveClauseSize(const VariableState * v1, const VariableState * v2);
bool SmallestPositiveClauseCount(const VariableState * v1, const VariableState * v2);
bool LargestPositiveClauseSize(const VariableState * v1, const VariableState * v2);
bool LargestPositiveClauseCount(const VariableState * v1, const VariableState * v2);

int DefaultSolver(const VariableState * v);
int FlipSatSolver(const VariableState * v);

//Naming Convention
//Least - Smallest number (So LeastSmallest is not the largest of the smallest, it is the smallest of the smallest numbers)
//Most - Largest number (So the opposite of least)
static SortFunction AllFunctions[] =
{
	{ LargeUsed, MostLargeUsed }, //0
	{ LargeUsed, LeastLargeUsed }, //1
	{ SmallUsed, MostSmallUsed }, //2
	{ SmallUsed, LeastSmallUsed }, //3
	{ Difference, MostDifference }, //4
	{ Difference, LeastDifference }, //5
	{ SmallestClauseSize, MostSmallestClauseSize }, //6
	{ SmallestClauseSize, LeastSmallestClauseSize }, //7
	{ SmallestClauseCount, MostSmallestClauseCount }, //8
	{ SmallestClauseCount, LeastSmallestClauseCount }, //9
	{ LargestClauseSize, MostLargestClauseSize }, //10
	{ LargestClauseSize, LeastLargestClauseSize }, //11
	{ LargestClauseCount, MostLargestClauseCount }, //12
	{ LargestClauseCount, LeastLargestClauseCount }, //13
	{ SmallestNegativeClauseSize, MostSmallestNegativeClauseSize }, //14
	{ SmallestNegativeClauseSize, LeastSmallestNegativeClauseSize }, //15
	{ SmallestNegativeClauseCount, MostSmallestNegativeClauseCount }, //16
	{ SmallestNegativeClauseCount, LeastSmallestNegativeClauseCount }, //17
	{ LargestNegativeClauseSize, MostLargestNegativeClauseSize }, //18
	{ LargestNegativeClauseSize, LeastLargestNegativeClauseSize }, //19
	{ LargestNegativeClauseCount, MostLargestNegativeClauseCount }, //20
	{ LargestNegativeClauseCount, LeastLargestNegativeClauseCount }, //21
	{ SmallestPositiveClauseSize, MostSmallestPositiveClauseSize }, //22
	{ SmallestPositiveClauseSize, LeastSmallestPositiveClauseSize }, //23
	{ SmallestPositiveClauseCount, MostSmallestPositiveClauseCount }, //24
	{ SmallestPositiveClauseCount, LeastSmallestPositiveClauseCount }, //25
	{ LargestPositiveClauseSize, MostLargestPositiveClauseSize }, //26
	{ LargestPositiveClauseSize, LeastLargestPositiveClauseSize }, //27
	{ LargestPositiveClauseCount, MostLargestPositiveClauseCount }, //28
	{ LargestPositiveClauseCount, LeastLargestPositiveClauseCount }, //29
	{ TotalUsed, MostTotalUsed }, //30
	{ TotalUsed, LeastTotalUsed } //31
};