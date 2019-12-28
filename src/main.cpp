#include <iostream>
#include <fstream>

#include "Tester.h"

//Memory leak detection
#include "Debug.h"

using namespace std;

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