class VariableState;

#ifndef VARIABLE_STATE_H
#define VARIABLE_STATE_H

#include <vector>
#include "Variable.h"

using namespace std;

class VariableState
{
private:
	unsigned int NegativesSize;
	unsigned int PositivesSize;
	vector <unsigned int> * NegativeClauseSizes;
	vector <unsigned int> * PositiveClauseSizes;
public:
	VariableState();
	VariableState(const Variable * v);
	~VariableState();
	bool IsSet() const;
	void Reset();
	void Reset(const Variable * v);

	unsigned int getNegativesSize() const;
	unsigned int getPositivesSize() const;
	unsigned int getNegativeClauseCount() const;
	unsigned int getPositiveClauseCount() const;
	unsigned int getNegativeClauseCount(const unsigned int & ClauseSize) const;
	unsigned int getPositiveClauseCount(const unsigned int & ClauseSize) const;

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

	bool operator==(const VariableState & variable) const;
};

#endif