#include "SplitState.h"

using namespace std;

#include <assert.h>

SplitState::SplitState(const SATState* root)
{
	this->_root = root;
	this->_parent = NULL;
	this->variables = new unordered_map<unsigned int, VariableState*>(root->getVariableMap()->size() * 2);
	for (map<unsigned int, VariableState*>::const_iterator iter = root->variables->cbegin(); iter != root->variables->cend(); iter++)
	{
		this->variables->insert_or_assign(iter->second->getVariable()->GetVariable(), iter->second);
	}
}

SplitState::SplitState(const SATState* root, const SplitState* _parent, unordered_map<unsigned int, VariableState*>* variables)
{
	this->_root = root;
	this->_parent = _parent;
	this->variables = variables;
}

SplitState::~SplitState()
{
	delete this->variables;
	this->variables = NULL;
}

list<unordered_map<unsigned int, VariableState*>*>* SplitState::getTrees() const
{
	list<unordered_map<unsigned int, VariableState*>*>* trees = new list<unordered_map<unsigned int, VariableState*>*>();
	for (unordered_map <unsigned int, VariableState*>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if (iter->second->isActive())
		{
			unordered_map<unsigned int, VariableState*>* firstMatch = NULL;
			for (list<unordered_map<unsigned int, VariableState*>*>::iterator treeIter = trees->begin(); treeIter != trees->end(); treeIter++)
			{
				if ((*treeIter)->find(iter->first) != (*treeIter)->cend())
				{
					if (firstMatch == NULL)
					{
						firstMatch = *treeIter;
					}
					else
					{
						list<unordered_map<unsigned int, VariableState*>*>::iterator remove = treeIter;
						for (unordered_map <unsigned int, VariableState*>::const_iterator copyIter = (*remove)->cbegin(); copyIter != (*remove)->cend(); copyIter++)
						{
							firstMatch->insert_or_assign(copyIter->first, copyIter->second);
						}
						treeIter--;
						delete (*remove);
						trees->erase(remove);
					}
				}
			}

			if (firstMatch == NULL)
			{
				firstMatch = new unordered_map<unsigned int, VariableState*>(this->variables->size() * 2);
				trees->push_back(firstMatch);
			}
			
			firstMatch->insert_or_assign(iter->first, iter->second);
			for (unordered_map <int, int>::const_iterator copyIter = iter->second->siblingCount->cbegin(); copyIter != iter->second->siblingCount->cend(); copyIter++)
			{
				assert(this->variables->find(copyIter->first) != this->variables->cend());
				firstMatch->insert_or_assign(copyIter->first, this->variables->find(copyIter->first)->second);
			}
		}
	}
	return trees;
}
list<SplitState*>* SplitState::getSplit() const
{
	list<unordered_map<unsigned int, VariableState*>*>* trees = this->getTrees();
	if (trees->size() < 2) {
		delete* trees->cbegin();
		delete trees;
		return NULL;
	}
	else
	{
		list<SplitState*>* ret = new list<SplitState*>();
		for (list<unordered_map<unsigned int, VariableState*>*>::const_iterator iter = trees->cbegin(); iter != trees->cend(); iter++)
		{
			ret->push_back(new SplitState(this->_root, this, *iter));
		}
		delete trees;
		return ret;
	}
}
const unordered_map <unsigned int, const VariableState*>* SplitState::getVariableMap() const
{
	return (unordered_map <unsigned int, const VariableState*>*)this->variables;
}
unsigned int SplitState::getRemainingClauseCount() const
{
	unsigned int count = 0;
	for (unordered_map <unsigned int, ClauseState*>::const_iterator iter = this->_root->activeClauses->cbegin(); iter != this->_root->activeClauses->cend(); iter++)
	{
		assert(iter->second->isActive());
		for (unsigned int i = 0; i < iter->second->clause->Size(); i++) {
			if (this->variables->find(iter->second->variables[i]->variable->GetVariable()) != this->variables->cend()) {
				count++;
			}
		}
	}
	return count;
}
unsigned int SplitState::getRemainingVariableCount() const
{
	unsigned int count = 0;
	for (unordered_map <unsigned int, VariableState*>::const_iterator iter = this->variables->cbegin(); iter != this->variables->cend(); iter++)
	{
		if (iter->second->isActive()) {
			count++;
		}
	}
	return count;
}
bool SplitState::hasSolution() const
{
	return this->getRemainingClauseCount() == 0;
}
bool SplitState::canSolve() const
{
	return this->_root->canSolve();
}