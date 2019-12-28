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

void Test(Timer* totalTimer, double compileTime, SAT* sat, const char* fileName, ofstream& out, ofstream& solutions, const int result) {
	assert(sat != NULL);

	Timer* timer = new Timer();

	double analyzeTime = 0;
	double solveTime = 0;
	double outputTime = 0;
	double cleanTime = 0;
	double totalTime = 0;

	cout << "(Analyzing)";
	cout.flush();

	timer->reset();
	SATSolver* solver = new SATSolver();
	analyzeTime = timer->elapsed();
	cout << "(Solving)";
	cout.flush();

	timer->reset();
	void* variable = VARIABLE_CREATOR(sat, 0, SET_SIZE, NULL);
#ifdef PARALLEL
	solver->runSolverParallel(sat, SET_SIZE, variable, &VARIABLE_CREATOR, &SOLVER);
#else
	solver->runSolver(sat, variable, SOLVER);
#endif
	solveTime = timer->elapsed();
	cout << "(Outtputing): ";
	cout.flush();
	timer->reset();
	Solution* output = &solver->analysisResults(out, &ANALYZER);

	out << result;
	out << "," << sat->VariableCount();
	out << "," << sat->ClauseCount();
	out << "," << fileName;

	solutions << "," << result;
	solutions << "," << static_cast<int>(output->solved);
	solutions << "," << sat->VariableCount();
	solutions << "," << sat->ClauseCount();
	solutions << "," << fileName;
	if (output->solved == SolvedStates::NOT_COMPLETED || output->solved == SolvedStates::COMPLETED_UNKNOWN)
	{
		out << ",(UNKNOWN),0,";
		solutions << ",(UNKNOWN),0,";
		cout << "Unknown";
		cout.flush();
	}
	else if (output->solved == SolvedStates::COMPLETED_NO_SOLUTION || output->solved == SolvedStates::NOT_COMPLETED_NO_SOLUTION)
	{
		out << ",(FALSE),0,";
		solutions << ",(FALSE),0,";
		cout << "False";
		cout.flush();
	}
	else
	{
		out << ",(TRUE)";
		solutions << ",(TRUE)";
		cout << "True";
		cout.flush();

		if (output->solutions != NULL)
		{
			out << "," << output->solutions->size();
			solutions << "," << output->solutions->size() << ",";
			for (list<const int*>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
			{
				solutions << "[(" << (*iter)[0] << ")(|";
				assert((*iter)[(*iter)[0] + 1] == NULL);
				for (int i = 1; i < ((*iter)[0] + 1); i++)
				{
					solutions << (*iter)[i] << "|";
				}
				solutions << ")]";
			}
		}
		else
		{
			out << "," << 0;
			solutions << "," << 0;
		}
	}
	solutions << endl;
	solutions.flush();

	outputTime = timer->elapsed();
	out << "," << compileTime
		<< "," << analyzeTime
		<< "," << solveTime
		<< "," << outputTime;
	out.flush();

	cout << " :(Cleaning)";
	cout.flush();

	timer->reset();
	solver->cleanSolution();
	cleanTime = timer->elapsed();
	totalTime = totalTimer->elapsed();
	out << "," << cleanTime
		<< "," << totalTime
		<< endl;
	out.flush();

	cout << "(" << totalTime << ")" << endl;
	cout.flush();

	delete output;
	delete solver;
	delete timer;
}

void CNFTest(const char* perfix, const unsigned int start, const unsigned int end, ofstream& file, ofstream& out, ofstream& solutions, const int result)
{
	Timer* totalTimer = new Timer();
	Timer* timer = new Timer();

	out << "Count,Final State (Variable Evaluations) (Solutions Found),Expected Results,Variable Count,Clause Count,File Location,Result,Solution Count,Compile Time,Analyze Time,Solve Time,Output Time,Clean Time,Total Time" << endl;
	solutions << "Count,Expected Result,Final State,Variable Count,Clause Count,File Location,Result,Solution Count,Solutions" << endl;

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

		out << dec << i << ",";
		solutions << i;
		Test(totalTimer, compileTime, sat, ss.str().c_str(), out, solutions, result);
		delete sat;
	}
	delete totalTimer;
	delete timer;
}

void InputTest(const char* fileName, ofstream& out, ofstream& solutions, const int result)
{
	Timer* totalTimer = new Timer();
	Timer* timer = new Timer();

	out << "Count,Final State (Variable Evaluations) (Solutions Found),Expected Results,Variable Count,Clause Count,File Location,Result,Solution Count,Compile Time,Analyze Time,Solve Time,Output Time,Clean Time,Total Time" << endl;
	solutions << "Count,Expected Result,Final State,Variable Count,Clause Count,File Location,Result,Solution Count,Solutions" << endl;

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

		out << dec << i << ",";
		solutions << i;
		Test(totalTimer, compileTime, sat, fileName, out, solutions, result);
		delete sat;
	}
	delete totalTimer;
	delete timer;
	file.close();
}