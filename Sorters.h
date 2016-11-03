#include "Variable.h"
#include "SAT.h"

using namespace std;

enum VariableSoltuions { VARIABLE_NO_SOLUTION = 0x7FFFFFFF, MUST_NEGATIVE = -2, VARIABLE_NEGATIVE = -1, VARIABLE_UNKNOWN = 0, VARIABLE_POSITIVE = 1, MUST_POSITIVE = 2 };

Variable * LeastDifference(Variable * v1, Variable * v2);
Variable * LeastSmallUsed(Variable * v1, Variable * v2);
Variable * LeastLargeUsed(Variable * v1, Variable * v2);
Variable * MostDifference(Variable * v1, Variable * v2);
Variable * MostSmallUsed(Variable * v1, Variable * v2);
Variable * MostLargeUsed(Variable * v1, Variable * v2);
Variable * LeastSmallestClauseSize(Variable * v1, Variable * v2);
Variable * LeastSmallestClauseCount(Variable * v1, Variable * v2);
Variable * LeastLargestClauseSize(Variable * v1, Variable * v2);
Variable * LeastLargestClauseCount(Variable * v1, Variable * v2);
Variable * MostSmallestClauseSize(Variable * v1, Variable * v2);
Variable * MostSmallestClauseCount(Variable * v1, Variable * v2);
Variable * MostLargestClauseSize(Variable * v1, Variable * v2);
Variable * MostLargestClauseCount(Variable * v1, Variable * v2);
Variable * MostSmallestNegativeClauseSize(Variable * v1, Variable * v2);
Variable * LeastSmallestNegativeClauseSize(Variable * v1, Variable * v2);
Variable * MostSmallestNegativeClauseCount(Variable * v1, Variable * v2);
Variable * LeastSmallestNegativeClauseCount(Variable * v1, Variable * v2);
Variable * MostLargestNegativeClauseSize(Variable * v1, Variable * v2);
Variable * LeastLargestNegativeClauseSize(Variable * v1, Variable * v2);
Variable * MostLargestNegativeClauseCount(Variable * v1, Variable * v2);
Variable * LeastLargestNegativeClauseCount(Variable * v1, Variable * v2);
Variable * MostSmallestPositiveClauseSize(Variable * v1, Variable * v2);
Variable * LeastSmallestPositiveClauseSize(Variable * v1, Variable * v2);
Variable * MostSmallestPositiveClauseCount(Variable * v1, Variable * v2);
Variable * LeastSmallestPositiveClauseCount(Variable * v1, Variable * v2);
Variable * MostLargestPositiveClauseSize(Variable * v1, Variable * v2);
Variable * LeastLargestPositiveClauseSize(Variable * v1, Variable * v2);
Variable * MostLargestPositiveClauseCount(Variable * v1, Variable * v2);
Variable * LeastLargestPositiveClauseCount(Variable * v1, Variable * v2);

bool LargeUsed(const Variable * v1, const Variable * v2);
bool Difference(const Variable * v1, const Variable * v2);
bool SmallUsed(const Variable * v1, const Variable * v2);
bool SmallestClauseSize(const Variable * v1, const Variable * v2);
bool SmallestClauseCount(const Variable * v1, const Variable * v2);
bool LargestClauseSize(const Variable * v1, const Variable * v2);
bool LargestClauseCount(const Variable * v1, const Variable * v2);
bool SmallestNegativeClauseSize(const Variable * v1, const Variable * v2);
bool SmallestNegativeClauseCount(const Variable * v1, const Variable * v2);
bool LargestNegativeClauseSize(const Variable * v1, const Variable * v2);
bool LargestNegativeClauseCount(const Variable * v1, const Variable * v2);
bool SmallestPositiveClauseSize(const Variable * v1, const Variable * v2);
bool SmallestPositiveClauseCount(const Variable * v1, const Variable * v2);
bool LargestPositiveClauseSize(const Variable * v1, const Variable * v2);
bool LargestPositiveClauseCount(const Variable * v1, const Variable * v2);

int DefaultSolver(Variable * v);
int FlipSatSolver(Variable * v);

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
	{ LargestClauseCount, LeastLargestClauseCount } //13
	/*{SmallestNegativeClauseSize, MostSmallestNegativeClauseSize}, //14
	{SmallestNegativeClauseSize, LeastSmallestNegativeClauseSize}, //15
	{SmallestNegativeClauseCount, MostSmallestNegativeClauseCount}, //16
	{SmallestNegativeClauseCount, LeastSmallestNegativeClauseCount}, //17
	{LargestNegativeClauseSize, MostLargestNegativeClauseSize}, //18
	{LargestNegativeClauseSize, LeastLargestNegativeClauseSize}, //19
	{LargestNegativeClauseCount, MostLargestNegativeClauseCount}, //20
	{LargestNegativeClauseCount, LeastLargestNegativeClauseCount}, //21
	{SmallestPositiveClauseSize, MostSmallestPositiveClauseSize}, //22
	{SmallestPositiveClauseSize, LeastSmallestPositiveClauseSize}, //23
	{SmallestPositiveClauseCount, MostSmallestPositiveClauseCount}, //24
	{SmallestPositiveClauseCount, LeastSmallestPositiveClauseCount}, //25
	{LargestPositiveClauseSize, MostLargestPositiveClauseSize}, //26
	{LargestPositiveClauseSize, LeastLargestPositiveClauseSize}, //27
	{LargestPositiveClauseCount, MostLargestPositiveClauseCount}, //28
	{LargestPositiveClauseCount, LeastLargestPositiveClauseCount} //29*/
};