#include <iostream>
#include <fstream>

#include "Tester.h"

//Memory leak detection
#include "Debug.h"

using namespace std;

/*
Ideas:

--Statistics Sorter--
* Can I improve the statistical function?
** Is the score scaling correctly?
* Scoring seems good at finding single solutions, but terrible at evaulating an entire tree

--Connection Sorter--
* Evalute the most connected variable (Highest sibling count)
* Go to sibling of last variable with most connections?
* Go to sibling of last variable with most connections to last variable?

--Split Graph--
* Split graph when graph has disconnections
** This can be done using the SIBLING_CALCULATIONS, and then check if there are variables without specific siblings and their siblings as don't have those siblings.
* Could look for maximum splits
** Function to check for varialbes with most siblings
** Function to check for variable with most `unique` (in-laws) siblings. Siblings your siblings don't also have

--Caching--
* What is the same cached value?
** Same clause remaining and same variables remaining
** Same clause remaining and a subset of variables remaining
** Superset clause remaining and same variables remaining
** Superset clause remaining and subset of variables remaining
* Is there a way to cache structure without caching values?

*/

int main(int argc, char * argv[])
{
	ofstream out;
	ofstream solutions;
	out.open("out\\InputOut.csv");
	solutions.open("out\\InputSolutions.csv");

	out << "Count,Fastest Methods,Per Function|Final State (Variable Evaluations) (Solutions Found)|,Expected Results,Variable Count,Clause Count,File Location,Result,Solution Count,Compile Time,Analyze Time,Solve Time,Output Time,Clean Time,Total Time" << endl;
	solutions << "Count,Expected Result,Final State,Variable Count,Clause Count,File Location,Result,Solution Count,Solutions |(Count)(Results)|" << endl;

	InputTest("data\\Input.txt", out, solutions, 1);

	out.close();
	solutions.close();
	system("pause");

	ofstream file;
	file.open("out\\Output.txt");
	out.open("out\\CNFOut.csv");
	solutions.open("out\\CNFSolutions.csv");

	out << "Count,Fastest Methods,Per Function|Final State (Variable Evaluations) (Solutions Found)|,Expected Results,Variable Count,Clause Count,File Location,Result,Solution Count,Compile Time,Analyze Time,Solve Time,Output Time,Clean Time,Total Time" << endl;
	solutions << "Count,Expected Result,Final State,Variable Count,Clause Count,File Location,Result,Solution Count,Solutions |(Count)(Results)|" << endl;

#ifdef _DEBUG
	file << "uf20" << endl;
	CNFTest("data\\uf20\\uf20-0", 1, 5, file, out, solutions, 1);

	file << "uf50" << endl;
	CNFTest("data\\uf50\\uf50-0", 1, 2, file, out, solutions, 1);

	file << "uuf50" << endl;
	CNFTest("data\\uuf50\\uuf50-0", 1, 2, file, out, solutions, -1);
#else
	//file << "comp" << endl;
	//CNFTest("data\\comp\\comp-0", 1, 1, file, out, solutions, 1);

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