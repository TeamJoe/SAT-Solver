#include <new>
#include <list>
#include <iostream>
#include <string.h>

#include "Sorters.h"
#include "SATState.h"
#include "SATSolverState.h"
#include "SATSolver.h"
#include "DepthSolver.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

class Methods
{
public:
	const char* name;
	Sorter* sorters;

	~Methods()
	{
		delete[] sorters;
	}

	Methods(const char * name, Sorter v1, Sorter v2) {
		this->name = name;
		sorters = new Sorter[3];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3) {
		this->name = name;
		sorters = new Sorter[4];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3, Sorter v4) {
		this->name = name;
		sorters = new Sorter[5];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = v4;
		sorters[4] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3, Sorter v4, Sorter v5) {
		this->name = name;
		sorters = new Sorter[6];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = v4;
		sorters[4] = v5;
		sorters[5] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3, Sorter v4, Sorter v5, Sorter v6) {
		this->name = name;
		sorters = new Sorter[7];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = v4;
		sorters[4] = v5;
		sorters[5] = v6;
		sorters[6] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3, Sorter v4, Sorter v5, Sorter v6, Sorter v7) {
		this->name = name;
		sorters = new Sorter[8];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = v4;
		sorters[4] = v5;
		sorters[5] = v6;
		sorters[6] = v7;
		sorters[7] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3, Sorter v4, Sorter v5, Sorter v6, Sorter v7, Sorter v8) {
		this->name = name;
		sorters = new Sorter[9];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = v4;
		sorters[4] = v5;
		sorters[5] = v6;
		sorters[6] = v7;
		sorters[7] = v8;
		sorters[8] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3, Sorter v4, Sorter v5, Sorter v6, Sorter v7, Sorter v8, Sorter v9) {
		this->name = name;
		sorters = new Sorter[10];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = v4;
		sorters[4] = v5;
		sorters[5] = v6;
		sorters[6] = v7;
		sorters[7] = v8;
		sorters[8] = v9;
		sorters[9] = Sorter::NoFunction;
	}

	Methods(const char* name, Sorter v1, Sorter v2, Sorter v3, Sorter v4, Sorter v5, Sorter v6, Sorter v7, Sorter v8, Sorter v9, Sorter v10) {
		this->name = name;
		sorters = new Sorter[11];
		sorters[0] = v1;
		sorters[1] = v2;
		sorters[2] = v3;
		sorters[3] = v4;
		sorters[4] = v5;
		sorters[5] = v6;
		sorters[6] = v7;
		sorters[7] = v8;
		sorters[8] = v9;
		sorters[9] = v10;
		sorters[10] = Sorter::NoFunction;
	}
};

struct DepthSatVariables
{
	const char* name;
	list <SortFunction *> * SortFunctions;
	VariableSolutions (*Decider)(const VariableState *);
	unsigned long long maxDepth;
	unsigned int sortCount;
};

const VariableState * Sort(const VariableState * var1, const VariableState * var2, const list <SortFunction *> * SortFunctions)
{
	list <SortFunction *>::const_iterator iter = SortFunctions->cbegin();
	for(; iter != SortFunctions->cend(); iter++)
	{
		if(!((*iter)->Eqaulity)(var1, var2))
		{
			return ((*iter)->Sort)(var1, var2);
		}
	}
	return NULL;
}

const VariableState * NextVariable(const list <SortFunction *> * SortFunctions, VariableSolutions (Decider)(const VariableState *), SATSolverState * solverState)
{
	const map <unsigned int, const VariableState *> * variables = solverState->getCurrentVariables();
	const VariableState * val = variables->cbegin()->second;
	
	VariableSolutions deciderInt = Decider(val);
	if (deciderInt == VariableSolutions::MUST_NEGATIVE || deciderInt == VariableSolutions::MUST_POSITIVE || deciderInt == VariableSolutions::VARIABLE_NO_SOLUTION) {
		return val;
	}

	map <unsigned int, const VariableState *>::const_iterator iter = variables->cbegin();
	map <unsigned int, const VariableState *>::const_iterator end = variables->cend();
	iter++;
	for(; iter != end; iter++)
	{
		const VariableState * val2 = iter->second;
		assert(val2->isActive());

		VariableSolutions deciderInt2 = Decider(val2);
		if (deciderInt2 == VariableSolutions::MUST_NEGATIVE || deciderInt2 == VariableSolutions::MUST_POSITIVE || deciderInt2 == VariableSolutions::VARIABLE_NO_SOLUTION) {
			return val2;
		}

		const VariableState * newVal = Sort(val, val2, SortFunctions);
		if (newVal != NULL) {
			val = newVal;
			deciderInt = deciderInt2;
		} else if (deciderInt2 != VariableSolutions::VARIABLE_UNKNOWN) {
			val = val2;
			deciderInt = deciderInt2;
		}
	}
	return val;
}

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
	sortList->push_back(new Methods("DebugA1", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountSmallestToLargest, Sorter::MostDifference));
	*/
	sortList->push_back(new Methods("DebugA2", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed));
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
	sortList->push_back(new Methods(Sorter::MostDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods(Sorter::MostDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods(Sorter::MostLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods(Sorter::LeastLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods(Sorter::MostLargeUsed, Sorter::LeastDifference));
	sortList->push_back(new Methods(Sorter::LeastLargeUsed, Sorter::LeastDifference));
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
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount, Sorter::LeastLargeUsed, Sorter::LeastDifference));
	sortList->push_back(new Methods(Sorter::MostDifference, Sorter::MostLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
	sortList->push_back(new Methods(Sorter::MostDifference, Sorter::LeastLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
	sortList->push_back(new Methods(Sorter::LeastDifference, Sorter::MostLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
	sortList->push_back(new Methods(Sorter::LeastDifference, Sorter::LeastLargeUsed, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
	sortList->push_back(new Methods(Sorter::MostLargeUsed, Sorter::MostDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
	sortList->push_back(new Methods(Sorter::LeastLargeUsed, Sorter::MostDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
	sortList->push_back(new Methods(Sorter::MostLargeUsed, Sorter::LeastDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
	sortList->push_back(new Methods(Sorter::LeastLargeUsed, Sorter::LeastDifference, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::MostLargestClauseCount));
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
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference));
	sortList->push_back(new Methods(Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargeUsed, Sorter::LeastDifference));
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
	sortList->push_back(new Methods("SmallToLarge-1", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountSmallestToLargest, Sorter::MostDifference));
	sortList->push_back(new Methods("SmallToLarge-2", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountSmallestToLargest, Sorter::LeastDifference));
	sortList->push_back(new Methods("LargeToSmall-1", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountLargestToSmallest, Sorter::MostDifference));
	/*
	sortList->push_back(new Methods("LargeToSmall-2",Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostClauseCountLargestToSmallest, Sorter::LeastDifference));
	sortList->push_back(new Methods("SmallToLarge-Odd-1", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountSmallestToLargest, Sorter::MostDifference));
	sortList->push_back(new Methods("SmallToLarge-Odd-2", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountSmallestToLargest, Sorter::LeastDifference));
	sortList->push_back(new Methods("LargeToSmall-Odd-1", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountLargestToSmallest, Sorter::MostDifference));
	sortList->push_back(new Methods("LargeToSmall-Odd-2", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastClauseCountLargestToSmallest, Sorter::LeastDifference));
	*/
	sortList->push_back(new Methods("ClauseSize-1", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods("ClauseSize-2", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods("ClauseSize-3", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods("ClauseSize-4", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods("ClauseSize-5", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods("ClauseSize-6", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods("ClauseSize-7", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference));
	sortList->push_back(new Methods("ClauseSize-8", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargeUsed, Sorter::LeastDifference));

	sortList->push_back(new Methods("Score-1", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::MostClauseCountSmallestToLargest, Sorter::MostDifference));
	sortList->push_back(new Methods("Score-2", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::MostClauseCountSmallestToLargest, Sorter::LeastDifference));
	sortList->push_back(new Methods("Score-3", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::MostClauseCountLargestToSmallest, Sorter::MostDifference));

	sortList->push_back(new Methods("Score-4", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods("Score-5", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods("Score-6", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::MostLargeUsed));
	sortList->push_back(new Methods("Score-7", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastDifference, Sorter::LeastLargeUsed));
	sortList->push_back(new Methods("Score-8", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods("Score-9", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::LeastLargeUsed, Sorter::MostDifference));
	sortList->push_back(new Methods("Score-10", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostLargeUsed, Sorter::LeastDifference));
	sortList->push_back(new Methods("Score-11", Sorter::HasNoSolution, Sorter::HasRequired, Sorter::MostAbsoluteScore, Sorter::LeastSmallestClauseSize, Sorter::MostSmallestClauseCount, Sorter::LeastLargestClauseSize, Sorter::LeastLargestClauseCount, Sorter::MostTotalUsed, Sorter::LeastLargeUsed, Sorter::LeastDifference));
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

list <SortFunction *> * getSortFunctions(const unsigned int count)
{
	Methods* method = getMethod(count);
	assert(method != NULL);
	list <SortFunction *> * ret = new list <SortFunction*>();
	for (unsigned int y = 0; method->sorters[y] != Sorter::NoFunction; y++)
	{
		ret->push_back(getSortFunction(method->sorters[y]));
	}
	delete method;
	return ret;
}

void analyzeDepthSat(ofstream & file, const ReturnValue * value)
{

}

void * createDepthSatVariable(const SAT * sat, const unsigned int currentCount, const unsigned int totalCount, const void * variables)
{
	DepthSatVariables * depthVariables = new DepthSatVariables;
	depthVariables->name = getName(currentCount);
	depthVariables->SortFunctions = getSortFunctions(currentCount);
	depthVariables->maxDepth = MAX_DEPTH_LIMIT;

#ifdef STATISTICS_STEPS
	depthVariables->Decider = &StatisticSolver;
#else
	depthVariables->Decider = &DefaultSolver;
#endif

	return depthVariables;
}


SolverState _solveDepthSat(ReturnValue * value, const SATSolver * solver, SATSolverState * solverState, const list <SortFunction *> * SortFunctions, VariableSolutions (Decider)(const VariableState *), const unsigned long long maxDepth);
ReturnValue * solveDepthSat(const SATSolver * solver, void * variables)
{
	DepthSatVariables * depthSatVariables = (DepthSatVariables *)variables;

	ReturnValue * value = new ReturnValue;
	value->name = depthSatVariables->name;
	value->solutions = new list<const int *>();
	value->state = new SATSolverState(solver->getSAT());

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << "(Start): ";
	const list <const list <int> *> * currentClauses = &value->state->getState()->getRemainingClauses();
	for(list <const list <int> *>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
	{
		cout << "(";
		for(list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
		{
			if (iter2 != (*(iter))->cbegin()) {
				cout << ",";
			}
			cout << (*(iter2));
		}
		cout << ")";
		delete (*(iter));
	}
	cout << endl;
	delete currentClauses;
#endif

	//Run the evaluation
	SolverState i = _solveDepthSat(value, solver, value->state, depthSatVariables->SortFunctions, depthSatVariables->Decider, depthSatVariables->maxDepth);
	if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND)
	{
		if (value->solutions->size() > 0)
		{
			value->solved = SolvedStates::NOT_COMPLETED_SOLUTION;
		}
		else
		{
			value->solved = SolvedStates::NOT_COMPLETED_NO_SOLUTION;
		}
	}
	else
	{
		if (value->solutions->size() > 0)
		{
			value->solved = SolvedStates::COMPLETED_SOLUTION;
		}
		else
		{
			value->solved = SolvedStates::COMPLETED_NO_SOLUTION;
		}
	}
	value->variables = NULL;


#ifndef COMPLETE
	value->terminateRemaingThreads = true;
#else
	value->terminateRemaingThreads = false;
#endif

	delete depthSatVariables->SortFunctions;
	delete depthSatVariables;

	return value;
}

SolverState _solveDepthSat(ReturnValue * value, const SATSolver * solver, SATSolverState * solverState, const list <SortFunction *> * SortFunctions, VariableSolutions (Decider)(const VariableState *), const unsigned long long maxDepth)
{
	//If all variable have a solution
	const SATState * satState = solverState->getState();
	if(satState->getRemainingVariableCount() == 0)
	{
		//If the solution is the opposite of what we currently have
		if (satState->getRemainingClauseCount() == 0)
		{
			try
			{
				value->solutions->push_back(satState->getState());
			}
			catch (std::bad_alloc& ba)
			{
				return SolverState::UNKNOWN_ERROR;
			}
#ifdef _DEBUG
			for (list<const int *>::const_iterator iter = value->solutions->cbegin(); iter != value->solutions->cend(); iter++)
			{
				assert(satState->getSAT()->Evaluate(*iter));
			}
#endif
			return SolverState::SOLUTION_FOUND;
		}
		if (solver->isTerminatingAllThreads) {
			return SolverState::TERMINATE_EARLY;
		}
		return SolverState::NO_SOLUTION_FOUND;
	}

	//exclude whole chains where every branch evaluates to true
	if (satState->getRemainingClauseCount() == 0)
	{
		try
		{
			value->solutions->push_back(satState->getState());
		}
		catch (std::bad_alloc& ba)
		{
			return SolverState::UNKNOWN_ERROR;
		}
#ifdef _DEBUG
		for (list<const int *>::const_iterator iter = value->solutions->cbegin(); iter != value->solutions->cend(); iter++)
		{
			assert(satState->getSAT()->Evaluate(*iter));
		}
#endif
		if (solver->isTerminatingAllThreads) {
			return SolverState::TERMINATE_EARLY;
		}
		return SolverState::SOLUTION_FOUND;
	}

	if (0 < maxDepth && maxDepth < satState->getVariableAttempts())
	{
		return SolverState::MAX_DEPTH_REACHED;
	}
	else if (solver->isTerminatingAllThreads)
	{
		return SolverState::TERMINATE_EARLY;
	}
	else if (!satState->canSolve()) 
	{
		return  SolverState::NO_SOLUTION_FOUND;
	}

	const VariableState * var1 = NextVariable(SortFunctions, Decider, solverState);
	assert(var1 != NULL);

	//Get best variable
	VariableSolutions solution = Decider(var1);
	assert(solution != VariableSolutions::VARIABLE_NO_SOLUTION);

	//Check if chain has a solution
	solverState->setVariable(var1->getVariable(), solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE);

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
	cout << var1->getValue() << "(" << solution << "): ";
	const list <const list <int> *> * currentClauses = &satState->getRemainingClauses();
	for(list <const list <int> *>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
	{
		cout << "(";
		for(list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
		{
			if (iter2 != (*(iter))->cbegin()) {
				cout << ",";
			}
			cout << (*(iter2));
		}
		cout << ")";
		delete (*(iter));
	}
	cout << endl;
	delete currentClauses;
#endif

	assert(var1->getValue() != 0);
	assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() > 0
		|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() < 0
		|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
		|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

	SolverState i = _solveDepthSat(value, solver, solverState, SortFunctions, Decider, maxDepth);
#ifdef END_ON_FIRST_SOLUTION
	if (i != SolverState::NO_SOLUTION_FOUND) {
#else
	if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND) {
#endif
		return i;
	}

	if (0 < maxDepth && maxDepth < satState->getVariableAttempts())
	{
		return SolverState::MAX_DEPTH_REACHED;
	}
	else if (solver->isTerminatingAllThreads)
	{
		return SolverState::TERMINATE_EARLY;
	}

	//Check if the opposite solution is plausible
	if(solution != VariableSolutions::MUST_POSITIVE && solution != VariableSolutions::MUST_NEGATIVE)
	{
		solverState->setVariable(var1->getVariable(), !(solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE));

#ifdef OUTPUT_INTERMEDIATE_SOLUTION
		cout << var1->getValue() << "(" << solution << "): ";
		const list <const list <int> *> * currentClauses = &satState->getRemainingClauses();
		for(list <const list <int> *>::const_iterator iter = currentClauses->cbegin(); iter != currentClauses->cend(); iter++)
		{
			cout << "(";
			for(list <int>::const_iterator iter2 = (*(iter))->cbegin(); iter2 != (*(iter))->cend(); iter2++)
			{
				if (iter2 != (*(iter))->cbegin()) {
					cout << ",";
				}
				cout << (*(iter2));
			}
			cout << ")";
			delete (*(iter));
		}
		cout << endl;
		delete currentClauses;
#endif

		assert(var1->getValue() != 0);
		assert((solution == VariableSolutions::VARIABLE_POSITIVE || solution == VariableSolutions::MUST_POSITIVE) && var1->getValue() < 0
			|| (solution == VariableSolutions::VARIABLE_NEGATIVE || solution == VariableSolutions::MUST_NEGATIVE) && var1->getValue() > 0
			|| (solution == VariableSolutions::VARIABLE_UNKNOWN && var1->getValue() != 0)
			|| (solution == VariableSolutions::VARIABLE_NO_SOLUTION && var1->getValue() != 0));

		SolverState i = _solveDepthSat(value, solver, solverState, SortFunctions, Decider, maxDepth);
#ifdef END_ON_FIRST_SOLUTION
		if (i != SolverState::NO_SOLUTION_FOUND) {
#else
		if (i != SolverState::NO_SOLUTION_FOUND && i != SolverState::SOLUTION_FOUND) {
#endif
			return i;
		}
	}

	//no solution found so step back and continue search
	solverState->unsetVariable(var1->getVariable());
	
	return SolverState::NO_SOLUTION_FOUND;
}