#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DEPTH_SAT
//#define BOTTOM_ANALYZE_SAT

#ifdef DEPTH_SAT
	#define ANALYZER analyzeDepthSat
	#define VARIABLE_CREATOR createDepthSatVariable
	#define SOLVER solveDepthSat
	#define PARALLEL
	#define COMPLETE
	//#define END_ON_FIRST_SOLUTION
	#ifdef _DEBUG
		//#define OUTPUT_INTERMEDIATE_SOLUTION
		#define MAX_DEPTH_LIMIT 300
		#define SET_SIZE 1
	#else
		#define MAX_DEPTH_LIMIT -1
		//#define ORGINAL_SET 8
		//#define SECONDARY_SET 16
		//#define THIRD_SET 8
		#define FOURTH_SET 16
		#define SET_SIZE 11
	#endif
#else 
	#define MAX_DEPTH_LIMIT 0
#endif

#ifdef BOTTOM_ANALYZE_SAT
	#define ANALYZER analyzeBottomSat
	#define VARIABLE_CREATOR createBottomSatVariable
	#define SOLVER solveBottomSat
	#define SET_SIZE 1
	#ifdef _DEBUG
		//#define OUTPUT_INTERMEDIATE_SOLUTION
	#endif
#endif

enum VariableSoltuions { VARIABLE_NO_SOLUTION = 0x7FFFFFFF, MUST_NEGATIVE = -2, VARIABLE_NEGATIVE = -1, VARIABLE_UNKNOWN = 0, VARIABLE_POSITIVE = 1, MUST_POSITIVE = 2 };
enum SolvedStates { COMPLETED_UNKNOWN = 0x7FFFFFFF, COMPLETED_NO_SOLUTION = -2, NOT_COMPLETED_NO_SOLUTION = -1, NOT_COMPLETED = 0, NOT_COMPLETED_SOLUTION = 1, COMPLETED_SOLUTION = 2 };

#endif