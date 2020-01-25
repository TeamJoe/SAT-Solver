#include "VariableState.h"
#include "SAT.h"
#include "Constants.h"

using namespace std;

const VariableState * HasNoSolution(const VariableState * v1, const VariableState * v2);
const VariableState * HasRequired(const VariableState* v1, const VariableState* v2);
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
const VariableState * MostClauseCountSmallestToLargest(const VariableState * v1, const VariableState * v2);
const VariableState * MostClauseCountLargestToSmallest(const VariableState * v1, const VariableState * v2);
const VariableState * LeastClauseCountSmallestToLargest(const VariableState * v1, const VariableState * v2);
const VariableState * LeastClauseCountLargestToSmallest(const VariableState * v1, const VariableState * v2);
const VariableState * MostAbsoluteScore(const VariableState* v1, const VariableState* v2);
const VariableState * LeastAbsoluteScore(const VariableState* v1, const VariableState* v2);

bool HasNoSolutionCompare(const VariableState * v1, const VariableState * v2);
bool HasRequiredCompare(const VariableState* v1, const VariableState* v2);
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
bool AllClauseCounts(const VariableState * v1, const VariableState * v2);
bool AbsoluteScore(const VariableState* v1, const VariableState* v2);

VariableSolutions StatisticSolver(const VariableState* v);
VariableSolutions DefaultSolver(const VariableState * v);
VariableSolutions FlipSatSolver(const VariableState * v);


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
	MostAbsoluteScore = 38,
	LeastAbsoluteScore = 39
};

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
	{ TotalUsed, LeastTotalUsed }, //31
	{ AllClauseCounts, MostClauseCountSmallestToLargest }, //32
	{ AllClauseCounts, MostClauseCountLargestToSmallest }, //33
	{ AllClauseCounts, LeastClauseCountSmallestToLargest }, //34
	{ AllClauseCounts, LeastClauseCountLargestToSmallest }, //35
	{ HasNoSolutionCompare, HasNoSolution }, //36
	{ HasRequiredCompare, HasRequired }, //37
	{ AbsoluteScore, MostAbsoluteScore }, //38
	{ AbsoluteScore, LeastAbsoluteScore } //39
};