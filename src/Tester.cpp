#ifdef WIN32
#include <Windows.h>
#else
#include <time.h>
#include <ctime>
#endif

#include <chrono>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Reader.h"
#include "SAT.h"
#include "SATState.h"
#include "SATSolver.h"
#include "Constants.h"

#ifdef DEPTH_SAT
#include "DepthSolver.h"
#endif
#ifdef BOTTOM_ANALYZE_SAT
#include "BottomSolver.h"
#endif

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

class Timer
{
public:
	Timer() : beg_(clock_::now()) {}
	void reset() { beg_ = clock_::now(); }
	double elapsed() const {
		return std::chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};

#include <string>
#include <windows.h>

void outputTimers(double compileTime, double analyzeTime, double solveTime, double outputTime, double cleanTime, double totalTime, ofstream& out)
{
	out << "," << compileTime
		<< "," << analyzeTime
		<< "," << solveTime
		<< "," << outputTime
		<< "," << cleanTime
		<< "," << totalTime;
	out.flush();
}

void Clean(SATSolver* solver)
{
	solver->cleanSolution();
}

void OutputWinnerFile(SAT* sat, const char* fileName, SATSolver* solver, Solution* output, ofstream& winner, const int result)
{
	winner << "," << result;
	winner << "," << sat->VariableCount();
	winner << "," << sat->ClauseCount();

	unsigned int winningCount = INT32_MAX;
	for (list<FunctionResult>::const_iterator iter = output->individualResults.cbegin(); iter != output->individualResults.cend(); iter++)
	{
		if (iter->variableAttempts < winningCount)
		{
			winningCount = iter->variableAttempts;
		}
	}

	for (list<FunctionResult>::const_iterator iter = output->individualResults.cbegin(); iter != output->individualResults.cend(); iter++)
	{
		winner << "," << iter->name
			<< "," << (int)iter->solved
			<< "," << iter->variableAttempts
			<< "," << iter->solutions->size()
			<< "," << (((double)iter->variableAttempts) / ((double)winningCount));
	}
}

void OutputSolutionFile(SAT* sat, const char* fileName, SATSolver* solver, Solution* output, ofstream& solutions, const int result)
{
	solutions << "," << result;
	solutions << "," << static_cast<int>(output->solved);
	solutions << "," << sat->VariableCount();
	solutions << "," << sat->ClauseCount();
	solutions << "," << fileName;

	if (output->solved == SolvedStates::NOT_COMPLETED || output->solved == SolvedStates::COMPLETED_UNKNOWN)
	{
		solutions << ",(UNKNOWN),0,";
	}
	else if (output->solved == SolvedStates::COMPLETED_NO_SOLUTION || output->solved == SolvedStates::NOT_COMPLETED_NO_SOLUTION)
	{
		solutions << ",(FALSE),0,";
	}
	else
	{
		solutions << ",(TRUE)";
		if (output->solutions != NULL)
		{
			solutions << "," << output->solutions->size() << ",";
			for (list<const int*>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
			{
				solutions << "|(" << (*iter)[0] << ")(:";
				assert((*iter)[(*iter)[0] + 1] == NULL);
				for (int i = 1; i < ((*iter)[0] + 1); i++)
				{
					solutions << (*iter)[i] << ":";
				}
				solutions << ")|";
			}
		}
		else
		{
			solutions << "," << 0;
		}
	}
}

void OutputOutFile(SAT* sat, const char* fileName, SATSolver* solver, Solution* output, ofstream& out, const int result)
{
	out << "," << result;
	out << "," << sat->VariableCount();
	out << "," << sat->ClauseCount();
	out << "," << fileName;

	if (output->solved == SolvedStates::NOT_COMPLETED || output->solved == SolvedStates::COMPLETED_UNKNOWN)
	{
		out << ",(UNKNOWN),0";
	}
	else if (output->solved == SolvedStates::COMPLETED_NO_SOLUTION || output->solved == SolvedStates::NOT_COMPLETED_NO_SOLUTION)
	{
		out << ",(FALSE),0";
	}
	else
	{
		out << ",(TRUE)";
		if (output->solutions != NULL)
		{
			out << "," << output->solutions->size();
		}
		else
		{
			out << "," << 0;
		}
	}
}

void OutputConsole(SAT* sat, const char* fileName, SATSolver* solver, Solution* output, const int result)
{
	if (output->solved == SolvedStates::NOT_COMPLETED || output->solved == SolvedStates::COMPLETED_UNKNOWN)
	{
		cout << "Unknown";
	}
	else if (output->solved == SolvedStates::COMPLETED_NO_SOLUTION || output->solved == SolvedStates::NOT_COMPLETED_NO_SOLUTION)
	{
		cout << "False";
	}
	else
	{
		cout << "True";
	}
	cout.flush();
}

void OutputResult(SAT* sat, const char* fileName, SATSolver* solver, ofstream& out, ofstream& solutions, ofstream& winners, const int result)
{
	Solution* output = &solver->analysisResults(out, &ANALYZER);

	OutputConsole(sat, fileName, solver, output, result);
	OutputOutFile(sat, fileName, solver, output, out, result);
	OutputSolutionFile(sat, fileName, solver, output, solutions, result);
	OutputWinnerFile(sat, fileName, solver, output, winners, result);

	delete output;
}

void Solve(SAT* sat, SATSolver* solver)
{
	void* variable = VARIABLE_CREATOR(sat, 0, SET_SIZE, NULL);
#ifdef PARALLEL
	solver->runSolverParallel(sat, SET_SIZE, variable, &VARIABLE_CREATOR, &SOLVER);
#else
	solver->runSolver(sat, variable, SOLVER);
#endif
}

SATSolver* Analyze()
{
	return new SATSolver();
}

void Test(Timer* totalTimer, double compileTime, SAT* sat, const char* fileName, ofstream& out, ofstream& solutions, ofstream& winners, const int result) {
	assert(sat != NULL);
	SAT* sat2;
#ifdef _DEBUG
	sat2 = sat->copy();
	assert(*sat == *sat2);
	delete sat2;

	sat2 = sat->reduce(2);
	delete sat2;
#endif
	Timer* timer = new Timer();

	double analyzeTime = 0;
	double solveTime = 0;
	double outputTime = 0;
	double cleanTime = 0;
	double totalTime = 0;

	cout << "(Analyzing)";
	cout.flush();
	timer->reset();
	SATSolver* solver = Analyze();
	analyzeTime = timer->elapsed();

	cout << "(Solving)";
	cout.flush();
	timer->reset();
	Solve(sat, solver);
	solveTime = timer->elapsed();

	cout << "(Outtputing): ";
	cout.flush();
	timer->reset();
	OutputResult(sat, fileName, solver, out, solutions, winners, result);
	outputTime = timer->elapsed();

	cout << "(Cleaning): ";
	cout.flush();
	timer->reset();
	Clean(solver);
	cleanTime = timer->elapsed();

	totalTime = totalTimer->elapsed();
	cout << "(" << totalTime << ")";
	cout.flush();

	outputTimers(compileTime, analyzeTime, solveTime, outputTime, cleanTime, totalTime, out);

	cout << endl;
	cout.flush();
	out << endl;
	out.flush();
	solutions << endl;
	solutions.flush();
	winners << endl;
	winners.flush();

	delete solver;
	delete timer;
}

void CNFTest(const char* perfix, const unsigned int start, const unsigned int end, ofstream& file, ofstream& out, ofstream& solutions, ofstream& winners, const int result)
{
	Timer* totalTimer = new Timer();
	Timer* timer = new Timer();

	for (unsigned int i = start; i <= end; i++)
	{
		totalTimer->reset();

		double compileTime = 0;

		stringstream ss;
		ss << perfix;
		ss << i;
		ss << ".cnf";

		cout << i << ":(Compiling)";
		cout.flush();
		timer->reset();
		ifstream file;
		file.open(ss.str().c_str());
		SAT* sat = ReadCNF(file);
		file.close();
		compileTime = timer->elapsed();
		assert(sat != NULL);

		out << dec << i;
		solutions << i;
		winners << i;
		Test(totalTimer, compileTime, sat, ss.str().c_str(), out, solutions, winners, result);
		delete sat;
	}
	delete totalTimer;
	delete timer;
}

void InputTest(const char* fileName, ofstream& out, ofstream& solutions, ofstream& winners, const int result)
{
	Timer* totalTimer = new Timer();
	Timer* timer = new Timer();

	ifstream file;
	file.open(fileName);
	for (unsigned int i = 1; !file.eof(); i++)
	{
		totalTimer->reset();

		double compileTime = 0;

		cout << i << ":(Compiling)";
		cout.flush();
		timer->reset();
		SAT* sat = ReadFrom(file);
		compileTime = timer->elapsed();
		if (sat == NULL)
		{
			cout << "(Invalid)" << endl;
			delete sat;
			break;
		}

		out << dec << i;
		solutions << i;
		winners << i;
		Test(totalTimer, compileTime, sat, fileName, out, solutions, winners, result);
		delete sat;
	}
	delete totalTimer;
	delete timer;
	file.close();
}