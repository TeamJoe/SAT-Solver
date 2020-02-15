class VariableState;
class SplitState;

#ifndef VARIABLE_STATE_H
#define VARIABLE_STATE_H

#include <vector>
#include <map>
#include <unordered_map>

#include "Variable.h"
#include "SATState.h"

#include "Constants.h"

using namespace std;

class VariableState
{
private:
	bool Active;
	bool True;
	SATState * satState;
	const Variable * variable;

	map <unsigned int, ClauseState *> * ActiveClauses;
	map <unsigned int, ClauseState *> * InactiveClauses;

	unsigned int NegativesSize;
	unsigned int PositivesSize;
	vector <unsigned int> * NegativeClauseSizes;
	vector <unsigned int> * PositiveClauseSizes;

#ifdef SIBLING_CALCULATIONS
	// Could become vector that is 2*variable count
	unordered_map <int, int>* positiveSiblingCount;
	unordered_map <int, int>* negativeSiblingCount;
	unordered_map <int, int>* siblingCount;
#endif
#ifdef STATISTICS_STEPS
	double* score;
	double* probabiltyPositive;
#endif
protected:
	VariableState(SATState * sat, const Variable * v);
	VariableState(SATState * sat, VariableState * v);

	void reset();
#ifdef _DEBUG
	void checkState() const;
#endif
	void addClause(const ClauseState * clauseState, const unsigned int clauseCount);
	void removeClause(const ClauseState * clauseState, const unsigned int clauseCount);

#ifdef STATISTICS_STEPS
	void updateStatistics(int step);
#endif
public:
	~VariableState();

	const Variable * getVariable() const;

	bool isActive() const;
	int getValue() const;
	bool hasSolution() const;
	unsigned int getSiblingCount(const VariableState* v) const;

#ifdef STATISTICS_STEPS
	//Statistics Functions
	double getProbabiltyPositive(int step) const;
	double getScore(int step) const;
	double getAbsoluteScore(int step) const;
#endif

	//State Helper Functions
	unsigned int getNegativesSize() const;
	unsigned int getPositivesSize() const;
	unsigned int getNegativeClauseCount(const unsigned int & ClauseSize) const;
	unsigned int getPositiveClauseCount(const unsigned int & ClauseSize) const;

	unsigned int getTotalNumber() const;
	unsigned int getDifference() const;
	unsigned int getLargestNumber() const;
	unsigned int getSmallestNumber() const;

	unsigned int getLargestPositiveClauseSize() const;
	unsigned int getLargestNegativeClauseSize() const;
	unsigned int getSmallestPositiveClauseSize() const;
	unsigned int getSmallestNegativeClauseSize() const;

	unsigned int getLargestPositiveClauseCount() const;
	unsigned int getLargestNegativeClauseCount() const;
	unsigned int getSmallestPositiveClauseCount() const;
	unsigned int getSmallestNegativeClauseCount() const;

	unsigned int getSmallestClauseSize() const;
	unsigned int getLargestClauseSize() const;
	unsigned int getSmallestClauseCount() const;
	unsigned int getLargestClauseCount() const;

	friend SATState;
	friend SplitState;
};

#endif