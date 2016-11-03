#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

#include "SAT.h"
#include "SATState.h"
#include "SATSolver.h"
#include "Solvers.h"
#include "Constants.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#include "Debug.h"

using namespace std;

void CNFTest(char * perfix, unsigned int start, unsigned int end, ofstream & file, ofstream & out, int result)
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

	for (unsigned int i = start; i <= end; i++)
	{
		stringstream ss;
		ss << perfix;
		ss << i;
		ss << ".cnf";

		cout << i << ":(Compiling)";
		cout.flush();
		SAT * sat = new SAT(ss.str().c_str(), true);
		assert(sat->isValid());

		cout << "(Analyzing)";
		cout.flush();

		SATSolver * solver = new SATSolver();
		out << dec << i << ",";
		cout << "(Solving)";
		cout.flush();

		void * variable = createDepthSatVariable(sat, 0, SET_SIZE, NULL);
#ifdef PARALLEL
		Solution * output = &solver->runSolverParallel(out, sat, SET_SIZE, variable, &createDepthSatVariable, &DepthSat, &analyzeDepthSat);
#else
		list<list<int> *> * output = solve.StopAtFirstTruth(out);
#endif
		cout << "(Outtputing): ";
		cout.flush();
		out << result;
		out << "," << sat->VariableCount();
		out << "," << sat->ClauseCount();
		out << "," << ss.str();
		if (output->solved == NOT_COMPLETED || output->solved == COMPLETED_UNKNOWN)
		{
			out << ",(UNKNOWN)";
			cout << "Unknown" << endl;
			if (output->solutions != NULL)
			{
				for (list<const list<int> *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					delete (*iter);
				}
				delete output->solutions;
			}
		}
		else if (output->solved == COMPLETED_NO_SOLUTION || output->solved == NOT_COMPLETED_NO_SOLUTION)
		{
			out << ",(FALSE)";
			cout << "False" << endl;
			if (output->solutions != NULL)
			{
				for (list<const list<int> *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					delete (*iter);
				}
				delete output->solutions;
			}
		}
		else 
		{
			out << ",(TRUE)";
			cout << "True" << endl;

			if (output->solutions != NULL)
			{
				out << output->solutions->size();
				for (list<const list<int> *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					out << ",(" << (*iter)->size() << ")(|";
					for (list<int>::const_iterator list = (*iter)->cbegin(); list != (*iter)->cend(); list++)
					{
						out << (*list) << "|";
					}
					out << ")";
					delete (*iter);
				}
				delete output->solutions;
			}
		}
		out << endl;
		delete output;
		delete solver;
		delete sat;
	}
}

void InputTest(char * fileName, ofstream & out, int result)
{
	ifstream file;
	file.open(fileName);
	for (unsigned int i = 1; !file.eof(); i++)
	{
		cout << i << ":(Compiling)";
		cout.flush();
		SAT * sat = new SAT(file, false);
		if (!sat->isValid())
		{
			cout << "(Invalid)" << endl;
			delete sat;
			break;
		}

		cout << "(Analyzing)";
		cout.flush();

		SATSolver * solver = new SATSolver();
		out << i << ",";
		cout << "(Solving)";
		cout.flush();

		void * variable = createDepthSatVariable(sat, 0, SET_SIZE, NULL);
#ifdef PARALLEL
		Solution * output = &solver->runSolverParallel(out, sat, SET_SIZE, variable, &createDepthSatVariable, &DepthSat, &analyzeDepthSat);
#else
		list<list<int> *> * output = solve.StopAtFirstTruth(out);
#endif
		cout << "(Outtputing): ";
		cout.flush();
		out << result;
		out << "," << sat->VariableCount();
		out << "," << sat->ClauseCount();
		if (output->solved == NOT_COMPLETED || output->solved == COMPLETED_UNKNOWN)
		{
			out << ",(UNKNOWN)";
			cout << "Unknown" << endl;
			if (output->solutions != NULL)
			{
				for (list<const list<int> *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					delete (*iter);
				}
				delete output->solutions;
			}
		}
		else if (output->solved == COMPLETED_NO_SOLUTION || output->solved == NOT_COMPLETED_NO_SOLUTION)
		{
			out << ",(FALSE)";
			cout << "False" << endl;
			if (output->solutions != NULL)
			{
				for (list<const list<int> *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					delete (*iter);
				}
				delete output->solutions;
			}
		}
		else 
		{
			out << ",(TRUE)";
			cout << "True" << endl;

			if (output->solutions != NULL)
			{
				out << output->solutions->size();
				for (list<const list<int> *>::const_iterator iter = output->solutions->cbegin(); iter != output->solutions->cend(); iter++)
				{
					out << ",(" << (*iter)->size() << ")(|";
					for (list<int>::const_iterator list = (*iter)->cbegin(); list != (*iter)->cend(); list++)
					{
						out << (*list) << "|";
					}
					out << ")";
					delete (*iter);
				}
				delete output->solutions;
			}
		}
		out << endl;
		delete output;
		delete solver;
		delete sat;
	}
	file.close();
}

int main(int argc, char * argv[])
{
	ofstream out;
	out.open("InputOut.csv");

	InputTest("Input.txt", out, 1);

	out.close();
	system("pause");

	ofstream file;
	file.open("Output.txt");
	out.open("CNFOut.csv");

#ifdef _DEBUG
	file << "uf20" << endl;
	CNFTest("uf20\\uf20-0", 1, 5, file, out, 1);
#else
	/*file << "comp" << endl;
	CNFTest("comp\\comp-0", 1, 1, file, out, 1);*/

	file << "uf20" << endl;
	CNFTest("uf20\\uf20-0", 1, 1000, file, out, 1);

	file << "uf50" << endl;
	CNFTest("uf50\\uf50-0", 1, 1000, file, out, 1);

	file << "uuf50" << endl;
	CNFTest("uuf50\\uuf50-0", 1, 1000, file, out, -1);

	file << "uf75" << endl;
	CNFTest("uf75\\uf75-0", 1, 100, file, out, 1);

	file << "uuf75" << endl;
	CNFTest("uuf75\\uuf75-0", 1, 100, file, out, -1);

	file << "uf100" << endl;
	CNFTest("uf100\\uf100-0", 1, 1000, file, out, 1);

	file << "uuf100" << endl;
	CNFTest("uuf100\\uuf100-0", 1, 1000, file, out, -1);
#endif

	out.close();
	system("pause");

	_CrtDumpMemoryLeaks();
}