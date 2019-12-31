#include <list>
#include "Variable.h"
#include "SAT.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

int getValue(Variable* v1, Variable* v2)
{
	return v1 > v2 ? v1->GetVariable() + v2->GetVariable() * 100000 : v2->GetVariable() + v1->GetVariable() * 100000;
}

CompositeVariable::CompositeVariable(Variable* v1, Variable* v2, SAT* _parent): Variable(getValue(v1, v2), _parent)
{
	this->v1 = v1 > v2 ? v2 : v1;
	this->v2 = v1 > v2 ? v1 : v2;
}

CompositeVariable* CompositeVariable::copy(SAT* _parent) const
{
	return new CompositeVariable(_parent->getVariable(v1->GetVariable()), _parent->getVariable(v2->GetVariable()), _parent);
}