#include "Reader.h"
#include "SAT.h"

//For checking that all output is correct
#include <assert.h>

using namespace std;

int CharToInt(const char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return (int)(c - 64);
	}
	else if (c >= 'a' && c <= 'z')
	{
		return (int)(-1 * (c - 96));
	}
	else
	{
		return (int)c;
	}
}

SAT* ReadFrom(ifstream& file)
{
	if (!file.is_open() || file.eof())
	{
		return NULL;
	}

	//Read Input
	unsigned int length;
	file >> length;

	if (file.eof())
	{
		return NULL;
	}

	SAT* sat = new SAT();
	list <int> clause;
	for (unsigned int i = 0; i < length; i++)
	{
		string s;
		file >> s;
		if (file.eof())
		{
			assert(false);
			delete sat;
			return NULL;
		}
		if (s.size() > 0)
		{
			clause.clear();
			for (unsigned int i = 0; i < s.size(); i++)
			{
				clause.push_back(CharToInt(s[i]));
			}
			sat->addClause(&clause);
		}
	}

	return sat;
}
SAT* ReadCNF(ifstream& file)
{
	if (!file.is_open() || file.eof())
	{
		return NULL;
	}

	while (true)
	{
		string s;
		file >> s;
		if (file.eof())
		{
			assert(false);
			return NULL;
		}
		if (s == "p")
		{
			file >> s;
			if (file.eof() || s != "cnf")
			{
				assert(false);
				return NULL;
			}
			break;
		}
	}

	unsigned int VarCount, ClauseCount;
	file >> VarCount >> ClauseCount;

	//Read inputs
	SAT* sat = new SAT();
	list <int> clause;
	unsigned int repeatCount = 0;
	for (unsigned int i = 0; i < ClauseCount; i++)
	{
		clause.clear();
		int var;
		file >> var;
		if (file.eof())
		{
			//unexpected end of file
			assert(false);
			delete sat;
			return NULL;
		}
		while (var != 0)
		{
			clause.push_back(var);
			file >> var;
		}
		if (!sat->addClause(&clause)) {
			repeatCount++;
		}
	}

	assert(VarCount == sat->VariableCount());
	assert(ClauseCount == sat->ClauseCount() + repeatCount);

	return sat;
}
