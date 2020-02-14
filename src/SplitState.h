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
	SATState* _root;
	const SplitState* _parent;

	SplitState(SATState* root, const SplitState* _parent, unordered_map<unsigned int, VariableState*>* variables);

	list<unordered_map<unsigned int, VariableState*>*>* getTrees() const;
public:
	SplitState(SATState* root);
	~SplitState();
	list<SplitState*>* getSplit() const;
};

#endif