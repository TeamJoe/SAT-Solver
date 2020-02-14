#include <list>

#include "VariableState.h"
#include "Constants.h"

using namespace std;

struct SortFunction;

VariableSolutions StatisticSolver(const VariableState* v);
VariableSolutions DefaultSolver(const VariableState* v);
VariableSolutions FlipSatSolver(const VariableState* v);

const VariableState* Sort(const VariableState* var1, const VariableState* var2, const list <SortFunction*>* SortFunctions);
const VariableState* NextVariable(const list <SortFunction*>* SortFunctions, VariableSolutions(Decider)(const VariableState*), SATSolverState* solverState);

const char* getName(const unsigned int count);
list <SortFunction*>* getSortFunctions(const unsigned int count);

#ifndef SORTER_H
#define SORTER_H

#include "VariableState.h"
#include "SAT.h"
#include "Constants.h"

using namespace std;

//Naming Convention
//Least - Smallest number (So LeastSmallest is not the largest of the smallest, it is the smallest of the smallest numbers)
//Most - Largest number (So the opposite of least)

struct SortFunction
{
	bool (*Eqaulity)(const VariableState*, const VariableState*);
	const VariableState* (*Sort)(const VariableState*, const VariableState*);
};

#endif