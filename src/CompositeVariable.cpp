#include <list>
#include "Variable.h"
#include "SAT.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

int getValue(Variable* v)
{
	return v->getIdentifier() < 1 ? (-1 * (v->getIdentifier() + 1)) : v->getIdentifier();
}

CompositeVariable::CompositeVariable(Variable* v1, Variable* v2, bool isOpposite, SAT* _parent): Variable(getValue(this), _parent)
{
	this->v1 = v1 > v2 ? v2 : v1;
	this->v2 = v1 > v2 ? v1 : v2;
	this->isOpposite = isOpposite;
	assert(this->GetVariable() != 0);
	assert(this->GetVariable() > 0);
}

CompositeVariable* CompositeVariable::copy(SAT* _parent) const
{
	return new CompositeVariable(_parent->getVariable(v1->GetVariable()), _parent->getVariable(v2->GetVariable()), this->isOpposite, _parent);
}