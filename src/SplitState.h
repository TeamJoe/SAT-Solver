class SplitState;

#ifndef SPLITSTATE_H
#define SPLITSTATE_H

#include <unordered_map>

#include "SATState.h"
#include "Constants.h"

using namespace std;

class SplitState
{
private:
	unordered_map <unsigned int, VariableState*>* variables;
	const SATState* _root;
	const SplitState* _parent;

	SplitState(const SATState* root, const SplitState* _parent, unordered_map<unsigned int, VariableState*>* variables);

	list<unordered_map<unsigned int, VariableState*>*>* getTrees() const;
public:
	SplitState(const SATState* root);
	~SplitState();
	list<SplitState*>* getSplit() const;

	const unordered_map <unsigned int, const VariableState*>* getVariableMap() const;

	unsigned int getRemainingClauseCount() const;
	unsigned int getRemainingVariableCount() const;
	bool hasSolution() const;
	bool canSolve() const;
};

#endif