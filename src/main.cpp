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
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

#include <string>
#include <windows.h>

void CNFTest(const char * perfix, const unsigned int start, const unsigned int end, ofstream & file, ofstream & out, ofstream & solutions, const int result)
{
	unsigned int False = 0;
	unsigned int Unknown = 0;
	unsigned int True = 0;

	unsigned long long Trues = 0xFFFFFFFFFFFFFFFF;
	unsigned long long TrueAndUnknowns = 0xFFFFFFFFFFFFFFFF;
	unsigned long long Unknowns = 0xFFFFFFFFFFFFFFFF;
	unsigned long long FalseAndUnknowns = 0xFFFFFFFFFFFFFFFF;
	unsigned long long Falses = 0xFFFFFFFFFFFFFFFF;
	unsigned long long TrueAndFalses = 0xFFFFFFFFFFFFFFFF;
	unsigned long long All = 0xFFFFFFFFFFFFFFFF;
	Timer * totalTimer = new Timer();
	Timer * timer = new Timer();

	for (unsigned int i = start; i <= end; i++)
	{
		totalTimer->reset();

		double compileTime = 0;
		double analyzeTime = 0;
		double solveTime = 0;
		double outputTime = 0;
		double cleanTime = 0;
		double totalTime = 0;

		stringstream ss;
		ss << perfix;
		ss << i;
		ss << ".cnf";

		cout << i << ":(Compiling)";
		cout.flush();
		timer->reset();
		SAT * sat = new SAT(ss.str().c_str(), true);
		compileTime = timer->elapsed();
		assert(sat->isValid());

		cout << "(Analyzing)";
		cout.flush();

		timer->reset();
		SATSolver * solver = new SATSolver();
		analyzeTime = timer->elapsed();
		out << dec << i << ",";
		cout << "(Solving)";
		cout.flush();

		timer->reset();
		void * variable = VARIABLE_CREATOR(sat, 0, SET_SIZE, NULL);
#ifdef PARALLEL
		solver->runSolverParallel(sat, SET_SIZE, variable, &VARIABLE_CREATOR, &SOLVER);
#else
		solver->runSolver(sat, variable, SOLVER);
#endif
		solveTime = timer->elapsed();
		cout << "(Outtputing): ";
		cout.flush();
		timer->reset();
		Solution * output = &solver->analysisResults(out, &ANALYZER);

		out << result;
		out << "," << sat->VariableCount();
		out << "," << sat->ClauseCount();
		out << "," << ss.str();

		solutions << i;
		solutions << "," << result;
		solutions << "," << static_cast<int>(output->solved);
		solutions << "," << sat->VariableCount();
		solutions << "," << sat->ClauseCount();
		solutions << "," << ss.str();
		if (output->solved == SolvedStates::NOT_COMPLETED || output->solved == SolvedStates::COMPLETED_UNKNOWN)
		{
			out << ",(UNKNOWN),0";
			solutions << ",(UNKNOWN),0";
			cout << "Unknown";
			cout.flush();
		}
		else if (output->solved == SolvedStates::COMPLETED_NO_SOLUTION || output->solved == SolvedStates::NOT_COMPLETED_NO_SOLUTION)
		{
			out << ",(FALSE),0";
			solutions << ",(FALSE),0";
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
				solutions << "," << output->solutions->size();
				for (list<const int *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					solutions << ",(" << (*iter)[0] << ")(|";
					assert((*iter)[(*iter)[0] + 1] == NULL);
					for (int i = 1; i < ((*iter)[0] + 1); i++)
					{
						solutions << (*iter)[i] << "|";
					}
					solutions << ")";
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
		out << ",(Compile Time: " << compileTime << ")"
			<< ",(Analyze Time: " << analyzeTime << ")"
			<< ",(Solve Time: " << solveTime << ")"
			<< ",(Output Time: " << outputTime << ")";
		out.flush();

		cout << " :(Cleaning)";
		cout.flush();

		timer->reset();
		solver->cleanSolution();
		cleanTime = timer->elapsed();
		totalTime = totalTimer->elapsed();
		out << ",(Clean Time: " << cleanTime << ")"
			<< ",(Total Time: " << totalTime << ")"
			<< endl;
		out.flush();

		cout << "(" << totalTime << ")" << endl;
		cout.flush();

		delete output;
		delete solver;
		delete sat;
	}
	delete totalTimer;
	delete timer;
}

void InputTest(const char * fileName, ofstream & out, ofstream & solutions, const int result)
{
	ifstream file;
	file.open(fileName);
	Timer * totalTimer = new Timer();
	Timer * timer = new Timer();
	for (unsigned int i = 1; !file.eof(); i++)
	{
		totalTimer->reset();

		double compileTime = 0;
		double analyzeTime = 0;
		double solveTime = 0;
		double outputTime = 0;
		double cleanTime = 0;
		double totalTime = 0;

		cout << i << ":(Compiling)";
		cout.flush();
		timer->reset();
		SAT * sat = new SAT(file, false);
		compileTime = timer->elapsed();
		if (!sat->isValid())
		{
			cout << "(Invalid)" << endl;
			delete sat;
			break;
		}

		cout << "(Analyzing)";
		cout.flush();

		timer->reset();
		SATSolver * solver = new SATSolver();
		analyzeTime = timer->elapsed();
		out << dec << i << ",";
		cout << "(Solving)";
		cout.flush();

		timer->reset();
		void * variable = VARIABLE_CREATOR(sat, 0, SET_SIZE, NULL);
#ifdef PARALLEL
		solver->runSolverParallel(sat, SET_SIZE, variable, &VARIABLE_CREATOR, &SOLVER);
#else
		solver->runSolver(sat, variable, SOLVER);
#endif
		solveTime = timer->elapsed();
		cout << "(Outtputing): ";
		cout.flush();
		timer->reset();
		Solution * output = &solver->analysisResults(out, &ANALYZER);

		out << result;
		out << "," << sat->VariableCount();
		out << "," << sat->ClauseCount();

		solutions << i;
		solutions << "," << result;
		solutions << "," << static_cast<int>(output->solved);
		solutions << "," << sat->VariableCount();
		solutions << "," << sat->ClauseCount();
		if (output->solved == SolvedStates::NOT_COMPLETED || output->solved == SolvedStates::COMPLETED_UNKNOWN)
		{
			out << ",(UNKNOWN),0";
			solutions << ",(UNKNOWN),0";
			cout << "Unknown";
			cout.flush();
		}
		else if (output->solved == SolvedStates::COMPLETED_NO_SOLUTION || output->solved == SolvedStates::NOT_COMPLETED_NO_SOLUTION)
		{
			out << ",(FALSE),0";
			solutions << ",(FALSE),0";
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
				solutions << "," << output->solutions->size();
				for (list<const int *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					solutions << ",(" << (*iter)[0] << ")(|";
					assert((*iter)[(*iter)[0] + 1] == NULL);
					for (int i = 1; i < ((*iter)[0] + 1); i++)
					{
						solutions << (*iter)[i] << "|";
					}
					solutions << ")";
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
		out << ",(Compile Time: " << compileTime << ")"
			<< ",(Analyze Time: " << analyzeTime << ")"
			<< ",(Solve Time: " << solveTime << ")"
			<< ",(Output Time: " << outputTime << ")";
		out.flush();

		cout << " :(Cleaning)";
		cout.flush();

		timer->reset();
		solver->cleanSolution();
		cleanTime = timer->elapsed();
		totalTime = totalTimer->elapsed();
		out << ",(Clean Time: " << cleanTime << ")"
			<< ",(Total Time: " << totalTime << ")"
			<< endl;
		out.flush();

		cout << "(" << totalTime << ")" << endl;
		cout.flush();

		delete output;
		delete solver;
		delete sat;
	}
	delete totalTimer;
	delete timer;
	file.close();
}

int main(int argc, char * argv[])
{
	ofstream out;
	ofstream solutions;
	out.open("out\\InputOut.csv");
	solutions.open("out\\InputSolutions.csv");

	InputTest("data\\Input.txt", out, solutions, 1);

	out.close();
	solutions.close();
	system("pause");

	ofstream file;
	file.open("out\\Output.txt");
	out.open("out\\CNFOut.csv");
	solutions.open("out\\CNFSolutions.csv");

#ifdef _DEBUG
	file << "uf20" << endl;
	CNFTest("data\\uf20\\uf20-0", 1, 5, file, out, solutions, 1);

	file << "uf50" << endl;
	CNFTest("data\\uf50\\uf50-0", 1, 2, file, out, solutions, 1);

	file << "uuf50" << endl;
	CNFTest("data\\uuf50\\uuf50-0", 1, 2, file, out, solutions, -1);
#else
	/*file << "comp" << endl;
	CNFTest("..\\data\\comp\\comp-0", 1, 1, file, out, solutions, 1);*/

	file << "uf20" << endl;
	CNFTest("data\\uf20\\uf20-0", 1, 1000, file, out, solutions, 1);

	file << "uf50" << endl;
	CNFTest("data\\uf50\\uf50-0", 1, 1000, file, out, solutions, 1);

	file << "uuf50" << endl;
	CNFTest("data\\uuf50\\uuf50-0", 1, 1000, file, out, solutions, -1);

	file << "uf75" << endl;
	CNFTest("data\\uf75\\uf75-0", 1, 100, file, out, solutions, 1);

	file << "uuf75" << endl;
	CNFTest("data\\uuf75\\uuf75-0", 1, 100, file, out, solutions, -1);
	
	file << "uf100" << endl;
	CNFTest("data\\uf100\\uf100-0", 1, 1000, file, out, solutions, 1);

	file << "uuf100" << endl;
	CNFTest("data\\uuf100\\uuf100-0", 1, 1000, file, out, solutions, -1);
#endif

	out.close();
	solutions.close();
	system("pause");

	_CrtDumpMemoryLeaks();
}