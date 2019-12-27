#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

#include "SAT.h"
#include "SATSolver.h"

//For checking that all output is correct
#include <assert.h>

//Memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Debug.h"

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

#define PARALLEL
//#define COMPLETE

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

		SAT s(ss.str().c_str(), true);
		assert(s.isValid());
		SATSolver solve(&s);

		out << dec << i << ",";

#ifdef COMPLETE
#ifdef PARALLEL
		ReturnValue output = solve.ParallelSolveTruth(out);
#else
		ReturnValue output = solve.SolveTruth(out);
#endif
		s.Revert();
		out << result;
		out << "," << s.VariableCount();
		out << "," << s.ClauseCount();
		out << "," << ss.str();
		s.cleanVariables();
		s.cleanClauses();
		for (list<list<int> *>::const_iterator iter = output.Result->cbegin(); iter != output.Result->cend(); iter++)
		{
			out << ",(|";
			for (list<int>::const_iterator list = (*iter)->cbegin(); list != (*iter)->cend(); list++)
			{
				out << (*list) << "|";
			}
			out << ")";
			delete (*iter);
		}
		delete output.Result;
		out << endl;
		if (output.Trues != 0)
		{
			cout << i << ": True" << endl;
			True++;
		}
#ifndef DEPTH
		else if(output.Unknowns != 0)
		{
			cout << i << ": Unknown" << endl;
			Unknown++;
		}
#else
		else if (output.Falses == 0)
		{
			cout << i << ": Unknown" << endl;
			Unknown++;
		}
#endif
		else
		{
			cout << i << ": False" << endl;
			False++;
		}
		Trues = Trues & output.Trues;
		TrueAndUnknowns = TrueAndUnknowns & (output.Trues | output.Unknowns);
		TrueAndFalses = TrueAndFalses & (output.Trues | output.Falses);
		Unknowns = Unknowns & output.Unknowns;
		FalseAndUnknowns = FalseAndUnknowns & (output.Unknowns | output.Falses);
		Falses = Falses & output.Falses;
		All = All & (output.Trues | output.Unknowns | output.Falses);
	}

	file
		<< "Results" << endl
		<< "True    (" << True << ")" << endl
		<< "Unknown (" << Unknown << ")" << endl
		<< "False   (" << False << ")" << endl
		<< endl
		<< "Statisitics" << endl
		<< "T  : 0x" << hex << (Trues) << endl
		<< "TU : 0x" << hex << (TrueAndUnknowns & (All ^ Unknowns) & (All ^ Trues)) << endl
		<< "U  : 0x" << hex << (Unknowns) << endl
		<< "FU : 0x" << hex << (FalseAndUnknowns & (All ^ Unknowns) & (All ^ Falses)) << endl
		<< "F  : 0x" << hex << (Falses) << endl
		<< "TF : 0x" << hex << (TrueAndFalses & (All ^ Trues) & (All ^ Falses)) << endl
		<< "A  : 0x" << hex << (All & (All ^ TrueAndFalses) & (All ^ FalseAndUnknowns) & (All ^ TrueAndUnknowns)) << endl
		<< endl
		<< "Variables" << endl
		<< "T  : 0x" << hex << (Trues) << endl
		<< "TU : 0x" << hex << (TrueAndUnknowns) << endl
		<< "U  : 0x" << hex << (Unknowns) << endl
		<< "FU : 0x" << hex << (FalseAndUnknowns) << endl
		<< "F  : 0x" << hex << (Falses) << endl
		<< "TF : 0x" << hex << (TrueAndFalses) << endl
		<< "A  : 0x" << hex << (All) << endl
		<< endl
		<< dec;
#else
#ifdef PARALLEL
		list<list<int> *> * output = solve.ParallelStopAtFirstTruth(out);
#else
		list<list<int> *> * output = solve.StopAtFirstTruth(out);
#endif
		s.Revert();
		out << result;
		out << "," << s.VariableCount();
		out << "," << s.ClauseCount();
		out << "," << ss.str();
		s.cleanVariables();
		s.cleanClauses();
		if (output == NULL) {
			out << ",(UNKNOWN)";
			cout << i << ": Unknown" << endl;
		}
		else if (output->size() < 1) {
			out << ",(FALSE)";
			cout << i << ": False" << endl;
		}
		else {
			cout << i << ": True" << endl;
			out << "," << output->size();
			for (list<list<int> *>::const_iterator iter = output->cbegin(); iter != output->cend(); iter++)
			{
				out << ",(" << (*iter)->size() << ")(|";
				for (list<int>::const_iterator list = (*iter)->cbegin(); list != (*iter)->cend(); list++)
				{
					out << (*list) << "|";
				}
				out << ")";
				delete (*iter);
			}
		}
		out << endl;
		delete output;
	}
#endif
}

void InputTest(char * fileName, ofstream & out, int result)
{
	ifstream file;
	file.open(fileName);
	for (unsigned int i = 1; !file.eof(); i++)
	{
		SAT s(file, false);
		if (s.ClauseCount() == 0) {
			break;
		}
		SATSolver solve(&s);
		out << i << ",";

#ifdef COMPLETE
#ifdef PARALLEL
		ReturnValue output = solve.ParallelSolveTruth(out);
#else
		ReturnValue output = solve.SolveTruth(out);
#endif
		s.Revert();
		out << result;
		out << "," << s.VariableCount();
		out << "," << s.ClauseCount();
		s.cleanVariables();
		s.cleanClauses();
		for (list<list<int> *>::const_iterator iter = output.Result->cbegin(); iter != output.Result->cend(); iter++)
		{
			out << ",(|";
			for (list<int>::const_iterator list = (*iter)->cbegin(); list != (*iter)->cend(); list++)
			{
				out << (*list) << "|";
			}
			out << ",)";
			delete (*iter);
		}
		delete output.Result;
		out << endl;
		if (output.Trues != 0)
		{
			cout << "True" << endl;
		}
#ifndef DEPTH
		else if (output.Unknowns != 0)
		{
			cout << "Unknown" << endl;
		}
#else
		else if (output.Falses == 0)
		{
			cout << "Unknown" << endl;
		}
#endif
		else
		{
			cout << "False" << endl;
		}
#else
#ifdef PARALLEL
		list<list<int> *> * output = solve.ParallelStopAtFirstTruth(out);
#else
		list<list<int> *> * output = solve.StopAtFirstTruth(out);
#endif
		s.Revert();
		out << result;
		out << "," << s.VariableCount();
		out << "," << s.ClauseCount();
		s.cleanVariables();
		s.cleanClauses();
		cout << i << ": ";
		if (output == NULL) {
			out << ",(UNKNOWN)";
			cout << "Unknown" << endl;
		}
		else if (output->size() < 1) {
			out << ",(FALSE)";
			cout << "False" << endl;
		}
		else {
			cout << "True";
			out << "," << output->size();
			cout << "," << output->size();
			for (list<list<int> *>::const_iterator iter = output->cbegin(); iter != output->cend(); iter++)
			{
				out << ",(" << (*iter)->size() << ")(|";
				cout << ",(" << (*iter)->size() << ")(|";
				for (list<int>::const_iterator list = (*iter)->cbegin(); list != (*iter)->cend(); list++)
				{
					out << (*list) << "|";
					cout << (*list) << "|";
				}
				out << ")";
				cout << ")";
				delete (*iter);
			}
			cout << endl;
		}
		out << endl;
		delete output;
#endif
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
	//file << "uf20" << endl;
	//CNFTest("uf20\\uf20-0", 1, 1000, file, out, 1);

	//file << "uf50" << endl;
	//CNFTest("uf50\\uf50-0", 1, 1000, file, out, 1);

	//file << "uuf50" << endl;
	//CNFTest("uuf50\\uuf50-0", 1, 1000, file, out, -1);

	//file << "uf75" << endl;
	//CNFTest("uf75\\uf75-0", 1, 100, file, out, 1);

	//file << "uuf75" << endl;
	//CNFTest("uuf75\\uuf75-0", 1, 100, file, out, -1);

	file << "uf100" << endl;
	CNFTest("uf100\\uf100-0", 2, 1000, file, out, 1);

	file << "uuf100" << endl;
	CNFTest("uuf100\\uuf100-0", 1, 1000, file, out, -1);
#endif

	out.close();

	_CrtDumpMemoryLeaks();
}