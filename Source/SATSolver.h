class SATSolver;

#ifndef SATSOLVER_H
#define SATSOLVER_H

#include <fstream>

#include "SAT.h"

using namespace std;

#define DEPTH
//#define DEPTH_USES_FLIP
#define DEPTH_FULL_EVAL
//#define FLIP
//#define FAST

//#define ORGINAL_SET
#define SECONDARY_SET

#ifdef _DEBUG
#define MAX_DEPTH_LIMIT 300
#else
#define MAX_DEPTH_LIMIT 100000000
#endif

struct ReturnValue
{
	unsigned long long Trues;
	unsigned long long Falses;
	unsigned long long Unknowns;
	list<list<int> *> * Result;
};

struct Pair
{
	bool Return;
	list<int> * Result;
	list<unsigned int> * Equalility;
	list<unsigned int> * Undecidable;
};

struct DepthPair
{
	int Return;
	unsigned long long EvalCount;
	unsigned long long AttemptCount;
	list<list<int> *> * Result;
};

class SATSolver
{
private:
	SAT * sat;
public:
	SATSolver(SAT * sat);
	~SATSolver();
	//list <string> & SlowSat();
	ReturnValue SolveTruth(ofstream & file) const;
	ReturnValue ParallelSolveTruth(ofstream & file) const;
	list<list<int> *> * ParallelStopAtFirstTruth(ofstream & file) const;
	list<list<int> *> * StopAtFirstTruth(ofstream & file) const;
};

#endif